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

#endif
