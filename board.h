#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <cstdint>
#include <vector>
#include <array>
#include <bitset>
#include <unordered_map>

typedef uint64_t Bitboard;
using namespace std;

// Define piece types
enum Piece {
    EMPTY, WPAWN, WKNIGHT, WBISHOP, WROOK, WQUEEN, WKING, BPAWN, BKNIGHT, BBISHOP, BROOK, BQUEEN, BKING
};

const int knight_offsets[8][2] = {
    {-2, -1}, {-1, -2}, {1, -2}, {2, -1},
    {2, 1}, {1, 2}, {-1, 2}, {-2, 1}
};
// Define class for the chess board
class Board {
private:
    // Bitboards for each piece type and color
    uint64_t white_pawns;
    uint64_t black_pawns;
    uint64_t white_knights;
    uint64_t black_knights;
    uint64_t white_bishops;
    uint64_t black_bishops;
    uint64_t white_rooks;
    uint64_t black_rooks;
    uint64_t white_queens;
    uint64_t black_queens;
    uint64_t white_kings;
    uint64_t black_kings;

    bool whiteTurn;

public:
    // Constructor
    Board();

    // Method to print the board (for debugging)
    void print_board() const;

    // Method to generate legal moves
    vector<pair<uint16_t, uint8_t>> generate_legal_moves(bool white);

    // Method to evaluate the board position (stub for now)
    int evaluate() const;

    void make_move(const uint16_t& move);

private:
    // Helper method to get piece type at a specific square
    pair<Piece,uint8_t> get_piece_at(int square_index) const;

    void pawn_moves(vector<pair<uint16_t, uint8_t>>& moves, int& pos, bool white);

    void knight_moves(vector<pair<uint16_t, uint8_t>>& moves, int& pos, Bitboard& own_pieces);

    void bishop_moves(vector<pair<uint16_t, uint8_t>>& moves, int& pos, Bitboard& own_pieces, Bitboard& opponent_pieces);

    void rook_moves(vector<pair<uint16_t, uint8_t>>& moves, int& pos, Bitboard& own_pieces, Bitboard& opponent_pieces);

    void queen_moves(vector<pair<uint16_t, uint8_t>>& moves, int& pos, Bitboard& own_pieces, Bitboard& opponent_pieces);

    void king_moves(vector<pair<uint16_t, uint8_t>>& moves, int& pos, bool white, Bitboard& own_pieces);

    unordered_map<Piece, Bitboard*> piece_bitboards = {
        {WPAWN, &white_pawns},
        {WKNIGHT, &white_knights},
        {WBISHOP, &white_bishops},
        {WROOK, &white_rooks},
        {WQUEEN, &white_queens},
        {WKING, &white_kings},
        {BPAWN, &black_pawns},
        {BKNIGHT, &black_knights},
        {BBISHOP, &black_bishops},
        {BROOK, &black_rooks},
        {BQUEEN, &black_queens},
        {BKING, &black_kings}
    };

    vector<Bitboard> generate_knight_moves();
    vector<Bitboard> knight_move_masks;

    vector<Bitboard> generate_bishop_moves();
    vector<Bitboard> bishop_move_masks;

    vector<Bitboard> generate_rook_moves();
    vector<Bitboard> rook_move_masks;

    vector<Bitboard> queen_move_masks;

    vector<Bitboard> generate_king_moves();
    vector<Bitboard> king_move_masks;

    bool is_square_attacked(int square_index, bool white);

    bool compareBySecond(const std::pair<uint16_t, uint8_t>& a, const std::pair<uint16_t, uint8_t>& b) const;

    void Board::minimax() const;
};

#endif // BOARD_H
