#include <vector>
#include <iostream>
#include <stdlib.h>
#include <string.h>

const int ENGINE_DEPTH = 10;
const int BOARD_SIZE = 14;
const int STONES = 4;
const int WHITE_POCKET = 6;
const int BLACK_POCKET = 13;

class Node {
    public:
        Node();
        Node(std::vector<int>& board, bool whiteTurn, bool terminal);
        ~Node();
        int minimax(int depth); // well you know what this is,
        // but it also sets bestMove to the optimal child node.
        int evaluate(); // evaluation of current node
        std::vector<Node*> generateChildren(); // creates the children nodes and adds it to the vector.
        std::vector<Node*> generateChildren(const std::vector<int>& b, const std::vector<int>& m); // for cases where more than one "move" per move
        std::vector<int> getBestMove();

        void print(); // for debugging purposes
    private:
        std::vector<int> board;
        std::vector<Node*> children;
        bool whiteTurn;
        std::vector<int> fromMove;
        Node* bestMove;
        bool terminal;

        int minimax(int depth, bool whiteTurn, int alpha, int beta);
};
