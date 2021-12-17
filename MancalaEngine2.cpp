#include "Mancala2.h"

int main(int argc, char *argv[]) {
    if (argc != BOARD_SIZE + 2) {
        std::cout << "Invalid input" << std::endl;
        return 1;
    }
    std::vector<int> board;
    bool whiteTurn = (strcmp(argv[1], "w") == 0);
    for (int i = 2; i < BOARD_SIZE + 2; i++) {
        char * asdf;
        board.push_back(strtol(argv[i], &asdf, 10));
    }
    Node initNode (board, whiteTurn);
    int eval = initNode.minimax(ENGINE_DEPTH);
    if (Node::isTerminal(board, whiteTurn)) {
        std::cout << "Terminal state" << std::endl;
    }
    else {
        std::cout << "Best move: ";
        for (int i : initNode.getBestMove()) {
            std::cout << i << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "Evaluation: " << eval << std::endl;
    return 0;
}
