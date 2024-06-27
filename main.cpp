#include <iostream>
#include <array>
#include <utility>
#include "boardSnapshot.h"
#include "moveValidationFunctions.h"

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

array<int, 4> positionToIndices(const string& move) {
    int startRow = 8 - (move[1] - '0');
    int startCol = move[0] - 'a';
    int endRow = 8 - (move[3] - '0');
    int endCol = move[2] - 'a';
    
    return {startRow, startCol, endRow, endCol};
}

bool validMove(const string& move, char (&board)[8][8], bool white, bool& unwon) {
    // check for correct syntax
    if (move.length() != 4) {
        cout << "invalid move syntax" << endl;
        return false;
    }
    for (int x=0; x < 4; x++) {
        if (x % 2 == 0) {
            if (not ((96 < move[x]) and (move[x] < 105))) {
                cout << "invalid move snytax" << endl;
                return false;
            }
        }
        else if (not ((48 < move[x]) and (move[x] < 57))) {
            cout << "invalid move syntax" << endl;
            return false;
        }
    }

    // Discover selected piece
    array<int, 4> posIndices = positionToIndices(move);
    char piece = board[posIndices[0]][posIndices[1]];

    // Check correct colour piece is being moved
    if (not (white ? isupper(piece) : islower(piece))) {
        cout << "Wrong colour piece chosen" << endl;
        return false;
    }

    // Handle validation for piece type
    switch (piece) {
        case 'P': // White pawn
            if (not validateWhitePawnMove(board, posIndices)) {
                cout << "Invalid move for white pawn" << endl;
                return false;
            }
            break;
        case 'p': // Black pawn
            if (not validateBlackPawnMove(board, posIndices)) {
                cout << "Invalid move for black pawn" << endl;
                return false;
            }
            break;
            
        case 'R':
        case 'r':
            if (not validateRookMove(board, posIndices)) {
                cout << "Invalid move for rook" << endl;
                return false;
            }
            break;
        case 'N':
        case 'n':
            if (not validateKnightMove(board, posIndices)) {
                cout << "Invalid move for knight" << endl;
                return false;
            }
            break;
        case 'B':
        case 'b':
            if (not validateBishopMove(board, posIndices)) {
                cout << "Invalid move for bishop" << endl;
                return false;
            }
            break;
        case 'Q':
        case 'q':
            if (not validateQueenMove(board, posIndices)) {
                cout << "Invalid move for queen" << endl;
                return false;
            }
            break;
        case 'K':
        case 'k':
            if (not validateKingMove(board, posIndices)) {
                cout << "Invalid move for king" << endl;
                return false;
            }
            break;
        default:
            cout << "Invalid chosen piece" << endl;
            return false;
    }

    // Check win condition
    if (tolower(board[posIndices[2]][posIndices[3]]) == 'k') {
        unwon = false;
    }

    // Swap pieces
    board[posIndices[2]][posIndices[3]] = board[posIndices[0]][posIndices[1]];
    board[posIndices[0]][posIndices[1]] = '.';


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
    bool white = true;
    while(unwon) {
        printBoard(board);

        do {
            cout << "Enter your move: ";
            cin >> move;
        } while (!validMove(move, board, white, unwon)); // Validates move and updates board if valid
        white = not white;
    }

    cout << "Game Over" << endl;

    return 0;
}