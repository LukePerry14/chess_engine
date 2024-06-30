#include <array>
using namespace std;

#ifndef MOVEVALIDATIONFUNCTIONS_H
#define MOVEVALIDATIONFUNCTIONS_H

bool validateWhitePawnMove(char (&board)[8][8], const array<int, 4>& posIndices);

bool validateBlackPawnMove(char (&board)[8][8], const array<int, 4>& posIndices);

bool validateRookMove(char (&board)[8][8], const array<int, 4>& posIndices);

bool validateKnightMove(char (&board)[8][8], const array<int, 4>& posIndices);

bool validateBishopMove(char (&board)[8][8], const array<int, 4>& posIndices);

bool validateQueenMove(char (&board)[8][8], const array<int, 4>& posIndices);

bool validateKingMove(char (&board)[8][8], const array<int, 4>& posIndices);

bool notOnFriendly(char (&board)[8][8], const array<int, 4>& posIndices);

bool checkCheck(char (&board)[8][8], const array<int, 4>& posIndices, const char& piece);

bool checkMate(char (&board)[8][8], const array<int, 2> kingCoords);

array<int, 2> findOpposingKing(char (&board)[8][8], bool white);



#endif
