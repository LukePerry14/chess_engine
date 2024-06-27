#include <array>
#include <iostream>


using namespace std;

bool notOnFriendly(char (&board)[8][8], const array<int, 4>& posIndices) {
    if (board[posIndices[2]][posIndices[3]] == '.') {
        return true;
    }
    return ((isupper(board[posIndices[0]][posIndices[1]]) && islower(board[posIndices[2]][posIndices[3]])) || (islower(board[posIndices[0]][posIndices[1]]) && isupper(board[posIndices[2]][posIndices[3]])));
}

bool validateWhitePawnMove(char (&board)[8][8], const array<int, 4>& posIndices) {
    if (posIndices[2] == posIndices[0] - 1 && posIndices[3] == posIndices[1] && board[posIndices[2]][posIndices[3]] == '.') {
        return true;
    } else if (posIndices[2] == posIndices[0] - 2 && posIndices[3] == posIndices[1] && posIndices[0] == 6 && board[posIndices[2]][posIndices[3]] == '.' && board[posIndices[0] - 1][posIndices[1]] == '.') {
        return true;
    } else if (posIndices[2] == posIndices[0] - 1 && abs(posIndices[3] - posIndices[1]) == 1 && islower(board[posIndices[2]][posIndices[3]])) {
        return true;
    }
    return false;
}

bool validateBlackPawnMove(char (&board)[8][8], const array<int, 4>& posIndices) {
    if (posIndices[2] == posIndices[0] + 1 && posIndices[3] == posIndices[1] && board[posIndices[2]][posIndices[3]] == '.') {
        return true;
    } else if (posIndices[2] == posIndices[0] + 2 && posIndices[3] == posIndices[1] && posIndices[0] == 1 && board[posIndices[2]][posIndices[3]] == '.' && board[posIndices[0] + 1][posIndices[1]] == '.') {
        return true;
    } else if (posIndices[2] == posIndices[0] + 1 && abs(posIndices[3] - posIndices[1]) == 1 && isupper(board[posIndices[2]][posIndices[3]])) {
        return true;
    }
    return false;
}

bool validateRookMove(char (&board)[8][8], const array<int, 4>& posIndices) {
    if (notOnFriendly(board, posIndices)){
        if (posIndices[0] == posIndices[2]) {
            int colStep = (posIndices[1] < posIndices[3]) ? 1 : -1;
            for (int col = posIndices[1] + colStep; col != posIndices[3]; col += colStep) {
                if (board[posIndices[0]][col] != '.') return false; // Check if path is clear
            }
        } else if (posIndices[1] == posIndices[3]) {
            int rowStep = (posIndices[0] < posIndices[2]) ? 1 : -1;
            for (int row = posIndices[0] + rowStep; row != posIndices[2]; row += rowStep) {
                if (board[row][posIndices[1]] != '.') return false; // Check if path is clear
            }
        } else {
            return false;
        }
        return true;
    }
    return false;
}

bool validateKnightMove(char (&board)[8][8], const array<int, 4>& posIndices) {
    if (notOnFriendly(board, posIndices)) {
        int rowDiff = abs(posIndices[0] - posIndices[2]);
        int colDiff = abs(posIndices[1] - posIndices[3]);
        return (rowDiff == 2 && colDiff == 1) || (rowDiff == 1 && colDiff == 2);
    }
    return false;
}

bool validateBishopMove(char (&board)[8][8], const array<int, 4>& posIndices) {
    if (notOnFriendly(board, posIndices)) {
        if (abs(posIndices[0] - posIndices[2]) != abs(posIndices[1] - posIndices[3])) return false;

        int rowStep = (posIndices[0] < posIndices[2]) ? 1 : -1;
        int colStep = (posIndices[1] < posIndices[3]) ? 1 : -1;
        for (int row = posIndices[0] + rowStep, col = posIndices[1] + colStep; row != posIndices[2]; row += rowStep, col += colStep) {
            if (board[row][col] != '.') return false; // Check if path is clear
        }
        return true;
    }
    return false;
}

// Function to validate queen moves
bool validateQueenMove(char (&board)[8][8], const array<int, 4>& posIndices) {
    if (notOnFriendly(board, posIndices)) {
        return validateRookMove(board, posIndices) || validateBishopMove(board, posIndices);
    }
    return false;
}

// Function to validate king moves
bool validateKingMove(char (&board)[8][8], const array<int, 4>& posIndices) {
    if (notOnFriendly(board, posIndices)) {
        int rowDiff = abs(posIndices[0] - posIndices[2]);
        int colDiff = abs(posIndices[1] - posIndices[3]);
        return (rowDiff <= 1 && colDiff <= 1);
    }
    return false;
}