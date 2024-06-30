#include <array>
#include <vector>
#include <cctype>
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
        // Single forward move
        return true;
    } else if (posIndices[2] == posIndices[0] - 2 && posIndices[3] == posIndices[1] && posIndices[0] == 6 && board[posIndices[2]][posIndices[3]] == '.' && board[posIndices[0] - 1][posIndices[1]] == '.') {
        // Double forward move
        return true;
    } else if (posIndices[2] == posIndices[0] - 1 && abs(posIndices[3] - posIndices[1]) == 1 && islower(board[posIndices[2]][posIndices[3]])) {
        // Capture move
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

bool checkRookLike(char (&board)[8][8], const std::array<int, 4>& posIndices, char king) {
    int channel, reference;
    if (posIndices[0] == posIndices[2]) {
        channel = posIndices[3];
        reference = posIndices[2];

        for (int i = reference + 1; i <= 7; ++i) {
            if (board[i][channel] == '.') {
                continue;
            } else if (board[i][channel] == king) {
                return true;
            } else {
                break;
            }
        }
        for (int i = reference - 1; i >= 0; --i) {
            if (board[i][channel] == '.') {
                continue;
            } else if (board[i][channel] == king) {
                return true;
            } else {
                break;
            }
        }
    } else {
        channel = posIndices[2];
        reference = posIndices[3];

        for (int i = reference + 1; i <= 7; ++i) {
            if (board[channel][i] == '.') {
                continue;
            } else if (board[channel][i] == king) {
                return true;
            } else {
                break;
            }
        }
        for (int i = reference - 1; i >= 0; --i) {
            if (board[channel][i] == '.') {
                continue;
            } else if (board[channel][i] == king) {
                return true;
            } else {
                break;
            }
        }
    }
    return false;
}

bool checkBishopLike(char (&board)[8][8], const std::array<int, 4>& posIndices, char king) {
    int colDirection;
    int rowDirection;

    if ((posIndices[0] > posIndices[2] && posIndices[1] > posIndices[3]) || (posIndices[0] < posIndices[2] && posIndices[1] < posIndices[3])) {
        colDirection = 1;
        rowDirection = -1;
    } else {
        colDirection = 1;
        rowDirection = 1;
    }

    for (int row = posIndices[2] + rowDirection, col = posIndices[3] + colDirection; row <= 7 && row >= 0 && col >= 0 && col <= 7; row += rowDirection, col += colDirection) {
        if (board[row][col] == '.') {
            continue;
        } else if (board[row][col] == king) {
            return true;
        } else {
            break;
        }
    }
    for (int row = posIndices[2] - rowDirection, col = posIndices[3] - colDirection; row <= 7 && row >= 0 && col >= 0 && col <= 7; row -= rowDirection, col -= colDirection) {
        if (board[row][col] == '.') {
            continue;
        } else if (board[row][col] == king) {
            return true;
        } else {
            break;
        }
    }
    return false;
}

bool checkCheck(char (&board)[8][8], const std::array<int, 4>& posIndices, const char& piece) {
    char king = std::isupper(piece) ? 'k' : 'K';

    switch (piece) {
        case 'p':
            if ((posIndices[2] + 1 >= 0 && posIndices[3] + 1 <= 7 && board[posIndices[2] + 1][posIndices[3] + 1] == 'K') ||
                (posIndices[2] + 1 >= 0 && posIndices[3] - 1 >= 0 && board[posIndices[2] + 1][posIndices[3] - 1] == 'K')) {
                return true;
            }
            return false;

        case 'P':
            if ((posIndices[2] - 1 <= 7 && posIndices[3] + 1 <= 7 && board[posIndices[2] - 1][posIndices[3] + 1] == 'k') ||
                (posIndices[2] - 1 <= 7 && posIndices[3] - 1 >= 0 && board[posIndices[2] - 1][posIndices[3] - 1] == 'k')) {
                return true;
            }
            return false;

        case 'R':
        case 'r':
            return checkRookLike(board, posIndices, king);

        case 'N':
        case 'n': {
            int nVertMoves[4][2] = {{2, 1}, {-2, 1}, {-2, -1}, {2, -1}};
            int nHorzMoves[4][2] = {{1, 2}, {-1, 2}, {-1, -2}, {1, -2}};
            int nCol, nRow;
            for (auto move : nVertMoves) {
                nCol = posIndices[2] + move[0];
                nRow = posIndices[3] + move[1];
                if (nCol >= 0 && nCol < 8 && nRow >= 0 && nRow < 8) {
                    if (board[nCol][nRow] == king) {
                        return true;
                    }
                }
            }
            for (auto move : nHorzMoves) {
                nCol = posIndices[2] + move[0];
                nRow = posIndices[3] + move[1];
                if (nCol >= 0 && nCol < 8 && nRow >= 0 && nRow < 8) {
                    if (board[nCol][nRow] == king) {
                        return true;
                    }
                }
            }
            return false;
        }

        case 'B':
        case 'b':
            return checkBishopLike(board, posIndices, king);

        case 'Q':
        case 'q':
            return checkRookLike(board, posIndices, king) || checkBishopLike(board, posIndices, king);

        default:
            return false;
    }
    return false;
}
bool friendly(char piece, char kingPiece) {
    if (isupper(kingPiece)) {
        return isupper(piece);
    } else {
        return islower(piece);
    }
}

char recase(char king, char piece) {
    if (isupper(king)) {
        return tolower(piece);
    } else {
        return toupper(piece);
    }
}


bool checkMate(char (&board)[8][8], const array<int, 2> kingCoords) {

    char king = board[kingCoords[0]][kingCoords[1]];

    vector<array<int, 2>> moveSetStack1;

    // Possible directions for the king
    vector<array<int, 2>> moveSetStack2 = {{
        {1, 0}, {1, 1}, {0, 1}, {-1, 1},
        {-1, 0}, {-1, -1}, {0, -1}, {1, -1}
    }};


    for (auto direction : moveSetStack2) {
        int newRow = kingCoords[0] + direction[0];
        int newCol = kingCoords[1] + direction[1];

        if (newRow >= 0 && newRow < 8 && newCol >= 0 && newCol < 8) {

            if (!friendly(board[newRow][newCol], king) || board[newRow][newCol] == '.') {
                moveSetStack1.push_back({newRow, newCol});
            }
        }
    }

    moveSetStack2.clear();

    // Check if there's an enemy pawn attacking the square
    for (const auto& move : moveSetStack1) {
        int checkRow = move[0];
        int checkCol = move[1];

        if (isupper(board[kingCoords[0]][kingCoords[1]])) { // White king
            if ((checkRow - 1 >= 0 && checkCol - 1 >= 0 && board[checkRow - 1][checkCol - 1] == recase(king, 'p')) ||
                (checkRow - 1 >= 0 && checkCol + 1 < 8 && board[checkRow - 1][checkCol + 1] == recase(king, 'p'))) {
                continue;
            }
        } else { // Black king
            if ((checkRow + 1 < 8 && checkCol - 1 >= 0 && board[checkRow + 1][checkCol - 1] == recase(king, 'P')) ||
                (checkRow + 1 < 8 && checkCol + 1 < 8 && board[checkRow + 1][checkCol + 1] == recase(king, 'P'))) {
                continue;
            }
        }
        moveSetStack2.push_back(move);
    }


    moveSetStack1.clear();

    // Check for threats from rooks and queens on horizontal and vertical channels
    for (const auto& move : moveSetStack2) {
        bool add = true;

        int row = move[0];
        int col = move[1];

        // Check to the right
        for (int i = col + 1; i <= 7; i++) {
            if (board[row][i] == recase(king, 'r') || board[row][i] == recase(king, 'q')) {
                add = false;
                goto endChannelMove;
            } else if ((board[row][i] == '.' || board[row][i] == king)){
                continue;
            }
        }
        
        // Check to the left
        for (int i = col - 1; i >= 0; i--) {
            if (board[row][i] == recase(king, 'r') || board[row][i] == recase(king, 'q')) {
                add = false;
                goto endChannelMove;
            } else  if ((board[row][i] == '.' || board[row][i] == king)){
                continue;
            } else {
                break;
            }
        }

        // Check downwards
        for (int i = row + 1; i <= 7; i++) {
            if (board[i][col] == recase(king, 'r') || board[i][col] == recase(king, 'q')) {
                add = false;

                goto endChannelMove;
            } else if ((board[i][col] == '.' || board[i][col] == king)){
                continue;
            } else {
                break;
            }
        }
        
        // Check upwards
        for (int i = row - 1; i >= 0; i--) {
            if (board[i][col] == recase(king, 'r') || board[i][col] == recase(king, 'q')) {
                add = false;

                goto endChannelMove;
            } else if ((board[i][col] == '.' || board[i][col] == king)){
                continue;
            } else {
                break;
            }
        }

    endChannelMove:
        if (add) {
            moveSetStack1.push_back(move);
        }        
    }

    moveSetStack2.clear();

    // Check for bishop like moves
    for (const auto& move : moveSetStack1) {
        bool add = true;
        int directions[4][2] = {{1, 1}, {1, -1}, {-1, 1}, {-1, -1}};

        for (const auto& direction : directions) {
            int rowDirection = direction[0];
            int colDirection = direction[1];
            
            for (int row = move[0] + rowDirection, col = move[1] + colDirection; row <= 7 && row >= 0 && col >= 0 && col <= 7; row += rowDirection, col += colDirection) {
                if (board[row][col] == '.' || board[row][col] == king) {
                    continue;
                } else if (board[row][col] == recase(king, 'b') || board[row][col] == recase(king, 'q')) {
                    add = false;
                    goto endDiagMove;
                } else {
                    break;
                }
            }
        }
    endDiagMove:
        if (add) {
            moveSetStack2.push_back(move);
        }
    }


    moveSetStack1.clear();

    // Check knight moves
    for (const auto&move : moveSetStack2) {
        bool add = true;
        int nVertMoves[4][2] = {{2, 1}, {-2, 1}, {-2, -1}, {2, -1}};
        int nHorzMoves[4][2] = {{1, 2}, {-1, 2}, {-1, -2}, {1, -2}};
        int nCol, nRow;
        for (auto attacking : nVertMoves) {
            nCol = move[0] + attacking[0];
            nRow = move[1] + attacking[1];
            if (nCol >= 0 && nCol < 8 && nRow >= 0 && nRow < 8) {
                if (board[nCol][nRow] == recase(king, 'k')) {
                    add = false;
                    goto endKnightMove;
                }
            }
        }
        for (auto attacking : nHorzMoves) {
            nCol = move[0] + attacking[0];
            nRow = move[1] + attacking[1];
            if (nCol >= 0 && nCol < 8 && nRow >= 0 && nRow < 8) {
                if (board[nCol][nRow] == king) {
                    add = false;
                    goto endKnightMove;
                }
            }
        }
    endKnightMove:
        if (add) {
            moveSetStack1.push_back(move);
        }
    }

    // If no threats are found, the king is not in checkmate
    return moveSetStack1.empty();
}

array<int, 2> findOpposingKing(char (&board)[8][8], bool white) {
    char opposingKing = white ? 'k' : 'K';
    array<int, 2> kingCoords = {-1, -1}; // Initialize with invalid coordinates
    
    // Iterate through the board to find the opposing king
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            if (board[row][col] == opposingKing) {
                kingCoords = {row, col};
                return kingCoords; // Found the opposing king, return its coordinates
            }
        }
    }
    
    return kingCoords;
}