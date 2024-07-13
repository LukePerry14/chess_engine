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
    int maxDepth;

public:
    Board();

    void print_board() const;

    pair<vector<pair<uint16_t, uint8_t>>, float> generate_legal_moves_and_evaluate(bool white, unordered_map<Piece, Bitboard> pieces);



    unordered_map<Piece, Bitboard> make_move(const uint16_t& move, unordered_map<Piece, Bitboard>& pieces);

private:

    pair<Piece,uint8_t> get_piece_at(int square_index, const unordered_map<Piece, Bitboard>& pieces) const;

    void pawn_moves(vector<pair<uint16_t, uint8_t>>& moves, int& pos, bool white, int& enemy_king_pos, unordered_map<Piece, Bitboard>& pieces);

    void knight_moves(vector<pair<uint16_t, uint8_t>>& moves, int& pos, Bitboard& own_pieces, int& enemy_king_pos, unordered_map<Piece, Bitboard>& pieces);

    void bishop_moves(vector<pair<uint16_t, uint8_t>>& moves, int& pos, Bitboard& own_pieces, Bitboard& opponent_pieces, int& enemy_king_pos, unordered_map<Piece, Bitboard>& pieces);

    void rook_moves(vector<pair<uint16_t, uint8_t>>& moves, int& pos, Bitboard& own_pieces, Bitboard& opponent_pieces, int& enemy_king_pos, unordered_map<Piece, Bitboard>& pieces);

    void queen_moves(vector<pair<uint16_t, uint8_t>>& moves, int& pos, Bitboard& own_pieces, Bitboard& opponent_pieces, int& enemy_king_pos, unordered_map<Piece, Bitboard>& pieces);

    void king_moves(vector<pair<uint16_t, uint8_t>>& moves, int& pos, bool white, Bitboard& own_pieces, unordered_map<Piece, Bitboard>& pieces);

    unordered_map<Piece, Bitboard> piece_bitboards = {
        {WPAWN, white_pawns},
        {WKNIGHT, white_knights},
        {WBISHOP, white_bishops},
        {WROOK, white_rooks},
        {WQUEEN, white_queens},
        {WKING, white_kings},
        {BPAWN, black_pawns},
        {BKNIGHT, black_knights},
        {BBISHOP, black_bishops},
        {BROOK, black_rooks},
        {BQUEEN, black_queens},
        {BKING, black_kings}
    };

    int find_king_position(Bitboard king_board);

    vector<Bitboard> generate_knight_moves();
    vector<Bitboard> knight_move_masks;

    vector<Bitboard> generate_bishop_moves();
    vector<Bitboard> bishop_move_masks;

    vector<Bitboard> generate_rook_moves();
    vector<Bitboard> rook_move_masks;

    vector<Bitboard> queen_move_masks;

    vector<Bitboard> generate_king_moves();
    vector<Bitboard> king_move_masks;

    bool is_square_attacked(int square_index, bool white, unordered_map<Piece, Bitboard>& pieces);

    static bool compareBySecond(const pair<uint16_t, uint8_t>& a, const pair<uint16_t, uint8_t>& b);

    pair<float, uint16_t> minimax(bool white, float& alpha, float& beta, int depth, unordered_map<Piece, Bitboard> pieces);

    array<uint8_t, 64> white_pawn_value_mask = {
        0, 0, 0, 0, 0, 0, 0, 0,
        4, 4, 4, 4, 4, 4, 4, 4,
        2, 2, 2, 2, 2, 2, 2, 2,
        2, 2, 3, 3, 3, 3, 2, 2,
        2, 3, 3, 3, 3, 3, 3, 2,
        1, 2, 2, 2, 2, 2, 2, 1,
        1, 1, 1, 1, 1, 1, 1, 1,
        0, 0, 0, 0, 0, 0, 0, 0
    };

    array<uint8_t, 64> black_pawn_value_mask = {
        0, 0, 0, 0, 0, 0, 0, 0,
        1, 1, 1, 1, 1, 1, 1, 1,
        1, 2, 2, 2, 2, 2, 2, 1,
        2, 3, 3, 3, 3, 3, 3, 2,
        2, 2, 3, 3, 3, 3, 2, 2,
        2, 2, 2, 2, 2, 2, 2, 2,
        4, 4, 4, 4, 4, 4, 4, 4,
        0, 0, 0, 0, 0, 0, 0, 0
    };

    array<uint8_t, 64> knight_value_mask = {
        2, 3, 4, 4, 4, 4, 3, 2,
        3, 4, 6, 6, 6, 6, 4, 3,
        4, 6, 8, 8, 8, 8, 6, 4,
        4, 6, 8,10,10, 8, 6, 4,
        4, 6, 8,10,10, 8, 6, 4,
        4, 6, 8, 8, 8, 8, 6, 4,
        3, 4, 6, 6, 6, 6, 4, 3,
        2, 3, 4, 4, 4, 4, 3, 2
    };

    array<uint8_t, 64> bishop_value_mask = {
        2, 3, 3, 3, 3, 3, 3, 2,
        3, 4, 4, 4, 4, 4, 4, 3,
        3, 4, 6, 6, 6, 6, 4, 3,
        3, 4, 6, 8, 8, 6, 4, 3,
        3, 4, 6, 8, 8, 6, 4, 3,
        3, 4, 6, 6, 6, 6, 4, 3,
        3, 4, 4, 4, 4, 4, 4, 3,
        2, 3, 3, 3, 3, 3, 3, 2
    };

    array<uint8_t, 64>  rook_value_mask = {
        5, 5, 5, 5, 5, 5, 5, 5,
        6, 6, 6, 6, 6, 6, 6, 6,
        7, 7, 7, 7, 7, 7, 7, 7,
        8, 8, 8, 8, 8, 8, 8, 8,
        8, 8, 8, 8, 8, 8, 8, 8,
        7, 7, 7, 7, 7, 7, 7, 7,
        6, 6, 6, 6, 6, 6, 6, 6,
        5, 5, 5, 5, 5, 5, 5, 5
    };

    array<uint8_t, 64> queen_value_mask = {
        2, 3, 4, 5, 5, 4, 3, 2,
        3, 4, 5, 6, 6, 5, 4, 3,
        4, 5, 7, 8, 8, 7, 5, 4,
        5, 6, 8,10,10, 8, 6, 5,
        5, 6, 8,10,10, 8, 6, 5,
        4, 5, 7, 8, 8, 7, 5, 4,
        3, 4, 5, 6, 6, 5, 4, 3,
        2, 3, 4, 5, 5, 4, 3, 2
    };

    array<uint8_t, 64> king_value_mask = {
        2, 2, 2, 2, 2, 2, 2, 2,
        2, 3, 3, 3, 3, 3, 3, 2,
        2, 3, 4, 4, 4, 4, 3, 2,
        2, 3, 4, 5, 5, 4, 3, 2,
        2, 3, 4, 5, 5, 4, 3, 2,
        2, 3, 4, 4, 4, 4, 3, 2,
        2, 3, 3, 3, 3, 3, 3, 2,
        2, 2, 2, 2, 2, 2, 2, 2
    };
};

#endif // BOARD_H
