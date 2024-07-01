#include <array>
#include <vector>
#include <cctype>
#include <iostream>
#include <variant>


using namespace std;

bool isBlockable(char (&board)[8][8], array<int, 2> location, char enemyKing) {
    int row = location[0];
    int col = location[1];
    // Directions for knight moves
    int knightMoves[8][2] = {
        {2, 1}, {2, -1}, {-2, 1}, {-2, -1},
        {1, 2}, {1, -2}, {-1, 2}, {-1, -2}
    };

    // Check for knight threats
    for (auto &move : knightMoves) {
        int newRow = row + move[0];
        int newCol = col + move[1];
        if (newRow >= 0 && newRow < 8 && newCol >= 0 && newCol < 8) {
            if (board[newRow][newCol] == (islower(enemyKing) ? 'n' : 'N')) {
                return true;
            }
        }
    }

    // Check for pawn threats
    if (islower(enemyKing)) { // White king, check for black pawns
        if ((row - 1 >= 0 && col - 1 >= 0 && board[row - 1][col - 1] == 'p') ||
            (row - 1 >= 0 && col + 1 < 8 && board[row - 1][col + 1] == 'p')) {
            return true;
        }
    } else { // Black king, check for white pawns
        if ((row + 1 < 8 && col - 1 >= 0 && board[row + 1][col - 1] == 'P') ||
            (row + 1 < 8 && col + 1 < 8 && board[row + 1][col + 1] == 'P')) {
            return true;
        }
    }


    // Check for rook/queen threats along rows and columns
    for (int i = 1; i < 8; ++i) {
        if (row + i < 8 && (board[row + i][col] == (islower(enemyKing) ? 'r' : 'R') || board[row + i][col] == (islower(enemyKing) ? 'q' : 'Q'))) return true;
        if (row - i >= 0 && (board[row - i][col] == (islower(enemyKing) ? 'r' : 'R') || board[row - i][col] == (islower(enemyKing) ? 'q' : 'Q'))) return true;
        if (col + i < 8 && (board[row][col + i] == (islower(enemyKing) ? 'r' : 'R') || board[row][col + i] == (islower(enemyKing) ? 'q' : 'Q'))) return true;
        if (col - i >= 0 && (board[row][col - i] == (islower(enemyKing) ? 'r' : 'R') || board[row][col - i] == (islower(enemyKing) ? 'q' : 'Q'))) return true;
    }


    // Check for bishop/queen threats along diagonals
    for (int i = 1; i < 8; ++i) {
        if (row + i < 8 && col + i < 8 && (board[row + i][col + i] == (islower(enemyKing) ? 'b' : 'B') || board[row + i][col + i] == (islower(enemyKing) ? 'q' : 'Q'))) return true;
        if (row - i >= 0 && col + i < 8 && (board[row - i][col + i] == (islower(enemyKing) ? 'b' : 'B') || board[row - i][col + i] == (islower(enemyKing) ? 'q' : 'Q'))) return true;
        if (row + i < 8 && col - i >= 0 && (board[row + i][col - i] == (islower(enemyKing) ? 'b' : 'B') || board[row + i][col - i] == (islower(enemyKing) ? 'q' : 'Q'))) return true;
        if (row - i >= 0 && col - i >= 0 && (board[row - i][col - i] == (islower(enemyKing) ? 'b' : 'B') || board[row - i][col - i] == (islower(enemyKing) ? 'q' : 'Q'))) return true;
    }

    return false;
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

vector<array<int,2>> checkRookLike(char (&board)[8][8], const std::array<int, 4>& posIndices, char king) {
    vector<array<int,2>> attackVector;

    int channel, reference;
    if (posIndices[0] == posIndices[2]) {
        channel = posIndices[3];
        reference = posIndices[2];

        for (int i = reference + 1; i <= 7; ++i) {
            if (board[i][channel] == '.') {
                attackVector.push_back({i, channel});
                continue;
            } else if (board[i][channel] == king) {
                return attackVector;
            } else {
                break;
            }
        }
        attackVector.clear();
        for (int i = reference - 1; i >= 0; --i) {
            if (board[i][channel] == '.') {
                attackVector.push_back({i, channel});
                continue;
            } else if (board[i][channel] == king) {
                return attackVector;
            } else {
                break;
            }
        }
    } else {
        channel = posIndices[2];
        reference = posIndices[3];

        for (int i = reference + 1; i <= 7; ++i) {
            if (board[channel][i] == '.') {
                attackVector.push_back({channel, i});
                continue;
            } else if (board[channel][i] == king) {
                return attackVector;
            } else {
                break;
            }
        }
        attackVector.clear();
        for (int i = reference - 1; i >= 0; --i) {
            if (board[channel][i] == '.') {
                attackVector.push_back({channel, i});
                continue;
            } else if (board[channel][i] == king) {
                return attackVector;
            } else {
                break;
            }
        }
    }
    return {};
}

vector<array<int, 2>> checkBishopLike(char (&board)[8][8], const array<int, 4>& posIndices, char king) {
    vector<array<int, 2>> attackVector;
    array<array<int, 2>, 2> diagonalChannels = {{{1, 1}, {1, -1}}};

    for (auto direction : diagonalChannels) {
        int rowDirection = direction[0];
        int colDirection = direction[1];

        // Forward diagonal
        for (int row = posIndices[2] + rowDirection, col = posIndices[3] + colDirection; row <= 7 && row >= 0 && col >= 0 && col <= 7; row += rowDirection, col += colDirection) {
            if (board[row][col] == '.') {
                attackVector.push_back({row, col});
            } else if (board[row][col] == king) {
                return attackVector;
            } else {
                break;
            }
        }

        // Backward diagonal
        attackVector.clear(); // Clear attack vector for the next diagonal direction
        for (int row = posIndices[2] - rowDirection, col = posIndices[3] - colDirection; row <= 7 && row >= 0 && col >= 0 && col <= 7; row -= rowDirection, col -= colDirection) {
            if (board[row][col] == '.') {
                attackVector.push_back({row, col});
            } else if (board[row][col] == king) {
                return attackVector;
            } else {
                break;
            }
        }
    }

    return {}; // Return empty vector if no attack vectors found
}

vector<array<int,2>> checkCheck(char (&board)[8][8], const std::array<int, 4>& posIndices, const char& piece) {
    char king = std::isupper(piece) ? 'k' : 'K';
    vector<array<int,2>> attackVector;
    attackVector.push_back({posIndices[2], posIndices[3]});

    switch (piece) {
        case 'p':
            if ((posIndices[2] + 1 >= 0 && posIndices[3] + 1 <= 7 && board[posIndices[2] + 1][posIndices[3] + 1] == 'K') ||
                (posIndices[2] + 1 >= 0 && posIndices[3] - 1 >= 0 && board[posIndices[2] + 1][posIndices[3] - 1] == 'K')) {
                return attackVector;
            }
            return {};

        case 'P':
            if ((posIndices[2] - 1 <= 7 && posIndices[3] + 1 <= 7 && board[posIndices[2] - 1][posIndices[3] + 1] == 'k') ||
                (posIndices[2] - 1 <= 7 && posIndices[3] - 1 >= 0 && board[posIndices[2] - 1][posIndices[3] - 1] == 'k')) {
                attackVector.push_back({posIndices[2], posIndices[3]});
                return attackVector;
            }
            return {};

        case 'R':
        case 'r':
            attackVector = checkRookLike(board,posIndices,king);
            return attackVector;

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
                        attackVector.push_back({posIndices[2], posIndices[3]});
                        return attackVector;
                    }
                }
            }
            for (auto move : nHorzMoves) {
                nCol = posIndices[2] + move[0];
                nRow = posIndices[3] + move[1];
                if (nCol >= 0 && nCol < 8 && nRow >= 0 && nRow < 8) {
                    if (board[nCol][nRow] == king) {
                        attackVector.push_back({posIndices[2], posIndices[3]});
                        return attackVector;
                    }
                }
            }
            return {};
        }

        case 'B':
        case 'b':
            attackVector = checkBishopLike(board,posIndices,king);
            attackVector.push_back({posIndices[2], posIndices[3]});
            return attackVector;

        case 'Q':
        case 'q':
            attackVector = checkRookLike(board,posIndices,king);
            if (attackVector.empty()){
                attackVector = checkBishopLike(board, posIndices, king);
            }
            if (not attackVector.empty()) {
                attackVector.push_back({posIndices[2], posIndices[3]});
                return attackVector;
            }
            return {};

        default:
            return {};
    }
    return {};
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

bool checkMate(char (&board)[8][8], const vector<array<int,2>> attackVector, const array<int, 2> kingCoords) {


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

    moveSetStack2.clear();

    array<int,2> enemyKingCoords = findOpposingKing(board, isupper(king));

    // Check if move is covered by enemy king
    for (const auto& move : moveSetStack1) {

        if (!(abs(move[0]- enemyKingCoords[0]) - abs(move[1] - enemyKingCoords[1]) == 0)) {
            moveSetStack2.push_back(move);
        }
    }


    // If there are available moves, return false, otherwise check if attack vector can be blocked by any pieces
    if (moveSetStack2.empty()) {
        for (auto location : attackVector) {
            if (isBlockable(board, location, king)) {
                return false;
            }
        }
        return true;
    } else {
        return false;
    }
}