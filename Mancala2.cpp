#include "Mancala2.h"

Node::Node() {
    board = std::vector<int> (BOARD_SIZE, STONES);
    board[WHITE_POCKET] = 0;
    board[BLACK_POCKET] = 0;
    children = std::vector<Node*> ();
    whiteTurn = true;
    fromMove = std::vector<int> ();
    bestMove = nullptr;
    terminal = false;
}

bool Node::isTerminal(std::vector<int>& board, bool whiteTurn) {
    int startIndex;
    int endIndex;
    if (whiteTurn) {
        startIndex = 0;
        endIndex = WHITE_POCKET;
    }
    else {
        startIndex = WHITE_POCKET + 1;
        endIndex = BLACK_POCKET;
    }
    for (int i = startIndex; i < endIndex; i++) {
        if (board[i] > 0) {
            return false;
        }
    }
    return true;
}

Node::Node(std::vector<int>& board, bool whiteTurn) {
    this->board = board;
    this->whiteTurn = whiteTurn;
    this->terminal = isTerminal(board, whiteTurn);
    children = std::vector<Node*>();
    fromMove = std::vector<int>();
    bestMove = nullptr;
}

Node::~Node() {
    for (Node* child : children) {
        delete child;
    }
}

int Node::evaluate() {
    if (terminal) {
        int enemyStartIndex;
        int enemyEndIndex;
        int enemyPocket;
        if (whiteTurn) {
            enemyStartIndex = WHITE_POCKET + 1;
            enemyEndIndex = BLACK_POCKET;
            enemyPocket = BLACK_POCKET;
        }
        else {
            enemyStartIndex = 0;
            enemyEndIndex = WHITE_POCKET;
            enemyPocket = WHITE_POCKET;
        }
        for (int i = enemyStartIndex; i < enemyEndIndex; i++) {
            board[enemyPocket] += board[i];
            board[i] = 0;
        }
        /*
        if (board[WHITE_POCKET] > board[BLACK_POCKET]) {
            return INT_MAX;
        }
        else if (board[BLACK_POCKET] > board[WHITE_POCKET]) {
            return INT_MIN;
        }
        else {
            return 0;
        }
        */
        return board[WHITE_POCKET] - board[BLACK_POCKET];

    }
    return board[WHITE_POCKET] - board[BLACK_POCKET];
}

std::vector<Node*> Node::generateChildren() {
    return generateChildren(board, std::vector<int> ());
}

std::vector<Node*> Node::generateChildren(const std::vector<int>& b, const std::vector<int>& m) {
    int ownPocket;
    int enemyPocket;
    int startIndex;
    int endIndex;
    if (whiteTurn) {
        ownPocket = WHITE_POCKET;
        enemyPocket = BLACK_POCKET;
        startIndex = 0;
        endIndex = WHITE_POCKET;
    }
    else {
        ownPocket = BLACK_POCKET;
        enemyPocket = WHITE_POCKET;
        startIndex = WHITE_POCKET + 1;
        endIndex = BLACK_POCKET;
    }
    std::vector<Node*> result;
    for (int i = startIndex; i < endIndex; i++) {
        if (b[i] > 0) {
            std::vector<int> newBoard = std::vector<int> (b);
            std::vector<int> newMove = std::vector<int> (m);
            newMove.push_back(i);
            int x = i;
            int count = newBoard[i];
            newBoard[i] = 0;
            while (count > 0) {
                x = (x + 1) % BOARD_SIZE;
                if (x != enemyPocket) {
                    newBoard[x]++;
                    count--;
                }
            }
            if (x == ownPocket) {
                std::vector<Node*> nodeSubset = generateChildren(newBoard, newMove);
                for (Node* newNode : nodeSubset) {
                    // put longest chains at the beginning of children vector
                    // for better alpha pruning
                    result.insert(result.begin(), newNode);
                }
            }
            else {
                // enforcing capture
                bool prioritize = false;
                if (newBoard[x] == 1 && startIndex <= x && x < endIndex) {
                    int opposite = (BOARD_SIZE - 2) - x;
                    newBoard[ownPocket] += newBoard[opposite];
                    newBoard[ownPocket] += 1;
                    newBoard[opposite] = 0;
                    newBoard[x] = 0;
                    prioritize = true;
                }
                Node* newNode = new Node(newBoard, !whiteTurn);
                newNode->fromMove = newMove;
                if (prioritize) {
                    result.insert(result.begin(), newNode);
                }
                else {
                    result.push_back(newNode);
                }
            }
        }
    }
    return result;
}

int Node::minimax(int depth) {
    return minimax(depth, whiteTurn, INT_MIN, INT_MAX);
}

int Node::minimax(int depth, bool whiteTurn, int alpha, int beta) {
    if (depth == 0 || terminal) {
        return evaluate();
    }
    if (whiteTurn) {
        int maxEva = INT_MIN;
        children = generateChildren();
        if (!children.empty()) {
            bestMove = children[0];
        }
        for (Node* child : children) {
            int eva = child->minimax(depth-1, false, alpha, beta);
            if (maxEva < eva) {
                maxEva = eva;
                bestMove = child;
            }
            alpha = std::max(alpha, maxEva);
            if (beta <= alpha) {
                break;
            }
        }
        return maxEva;
    }
    else {
        int minEva = INT_MAX;
        children = generateChildren();
        if (!children.empty()) {
            bestMove = children[0];
        }
        for (Node* child : children) {
            int eva = child->minimax(depth-1, true, alpha, beta);
            if (minEva > eva) {
                minEva = eva;
                bestMove = child;
            }
            beta = std::min(beta, minEva);
            if (beta <= alpha) {
                break;
            }
        }
        return minEva;
    }
}

std::vector<int> Node::getBestMove() {
    return bestMove->fromMove;
}

void Node::print() {
    for (int i : board) {
        std::cout << i << " ";
    }
    if (whiteTurn) std::cout << " (White to move)";
    else std::cout << " (Black to move)";
    std::cout << " (" << evaluate() << ")";
    if (terminal) std::cout << "TERM";
    std::cout << std::endl;
    std::cout << "via ";
    for (int i : fromMove) {
        std::cout << i << " ";
    }
    std::cout << std::endl << std::endl;
    if (children.size() > 0) {
        std::cout << "{" << std::endl;
        for (Node* n : children) {
            n->print();
        }
        std::cout << "}" << std::endl;
    }
}
