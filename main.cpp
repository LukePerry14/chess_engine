#include <iostream>
#include "boardSnapshot.h"
using namespace std;

void printBoard(char (&board)[8][8]) {
    cout << endl;
    for (int i = 0; i < 8; ++i) {
        cout << 8 - i << "| ";
        for (int j = 0; j < 8; ++j) {
            cout << board[i][j] << " ";
        }
        cout << endl;
    }
    cout << " └________________" << endl;
    cout << "   a b c d e f g h" << endl << endl;
}

void positionToIndices(const string& pos, int& row, int& col) {
    col = pos[0] - 'a';
    row = 8 - (pos[1] - '0');
}

void updateBoard(char (&board)[8][8], const string& move) {
    // Extract start and end positions from the move string
    string startPos = move.substr(0, 2);
    string endPos = move.substr(2, 2);
    
    // Convert positions to indices
    int startRow, startCol, endRow, endCol;
    positionToIndices(startPos, startRow, startCol);
    positionToIndices(endPos, endRow, endCol);
    
    // Move the piece
    board[endRow][endCol] = board[startRow][startCol];
    board[startRow][startCol] = '.';
}

bool validMove(const string& move) {
    return true;
}

int main() {
    char board[8][8] = {
        {'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r'},
        {'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'},
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'},
        {'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R'}
    };
    bool unwon = true;
    string move;

    while(unwon) {
        printBoard(board);

        do {
            cout << "Enter your move: ";
            cin >> move;
        } while (!validMove(move));

        updateBoard(board, move);
    }

    return 0;
}