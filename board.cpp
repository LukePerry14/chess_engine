#include <utility>
#include <vector>
#include <bitset>
#include <algorithm>
#include "board.h"

typedef uint64_t Bitboard;
using namespace std;

Board::Board() {
    // Initialize bitboards for starting position in binary format
    black_pawns   = 0b0000000011111111000000000000000000000000000000000000000000000000;
    black_knights = 0b0100001000000000000000000000000000000000000000000000000000000000;
    black_bishops = 0b0010010000000000000000000000000000000000000000000000000000000000;
    black_rooks   = 0b1000000100000000000000000000000000000000000000000000000000000000;
    black_queens  = 0b0000100000000000000000000000000000000000000000000000000000000000;
    black_kings   = 0b0001000000000000000000000000000000000000000000000000000000000000;
    white_pawns   = 0b0000000000000000000000000000000000000000000000001111111100000000;
    white_knights = 0b0000000000000000000000000000000000000000000000000000000001000010;
    white_bishops = 0b0000000000000000000000000000000000000000000000000000000000100100;
    white_rooks   = 0b0000000000000000000000000000000000000000000000000000000010000001;
    white_queens  = 0b0000000000000000000000000000000000000000000000000000000000001000;
    white_kings   = 0b0000000000000000000000000000000000000000000000000000000000010000;

    whiteTurn = true;

    knight_move_masks = generate_knight_moves();
    bishop_move_masks = generate_bishop_moves();
    rook_move_masks = generate_rook_moves();
    for (int t = 0; t < 64; t++) {
        queen_move_masks.push_back(rook_move_masks[t] | bishop_move_masks[t]);
    }
    king_move_masks = generate_king_moves();
}

// Method to generate legal moves
vector<pair<uint16_t, uint8_t>> Board::generate_legal_moves(bool white) {
    vector<pair<uint16_t, uint8_t>> moves;
    Bitboard own_pieces = white ? (white_pawns | white_knights | white_bishops | white_rooks | white_queens | white_kings)
                                : (black_pawns | black_knights | black_bishops | black_rooks | black_queens | black_kings);

    Bitboard opponent_pieces = white ? (black_pawns | black_knights | black_bishops | black_rooks | black_queens | black_kings)
                                     : (white_pawns | white_knights | white_bishops | white_rooks | white_queens | white_kings);


    if (white) {
        Bitboard square_mask = 1ULL;
        for (int pos = 0; pos < 64; pos++) {
            if (pos > 7) {
                if (white_pawns & square_mask) {
                    pawn_moves(moves, pos, true);
                }
            }
            if (white_knights & square_mask) {
                knight_moves(moves, pos, own_pieces);
            } else if (white_bishops & square_mask) {
                bishop_moves(moves, pos, own_pieces, opponent_pieces);
            } else if (white_rooks & square_mask) {
                rook_moves(moves, pos, own_pieces, opponent_pieces);
            } else if (white_queens & square_mask) {
                queen_moves(moves, pos, own_pieces, opponent_pieces);
            } else if (white_kings & square_mask) {
                king_moves(moves, pos, true, own_pieces);
            }
            square_mask = square_mask << 1;
        }
        for(auto move : moves) {
            uint16_t m = move.first;
            cout << bitset<16>(m) << endl;
        }

    } else {
        Bitboard square_mask = 1ULL;
        for (int pos = 0; pos < 64; pos++) {
            if (pos < 56) {
                if (black_pawns & square_mask) {
                    pawn_moves(moves, pos, false);
                }
            }
            if (black_knights & square_mask) {
                knight_moves(moves, pos, own_pieces);
            } else if (black_bishops & square_mask) {
                bishop_moves(moves, pos, own_pieces, opponent_pieces);
            } else if (black_rooks & square_mask) {
                rook_moves(moves, pos, own_pieces, opponent_pieces);
            } else if (black_queens & square_mask) {
                queen_moves(moves, pos, own_pieces, opponent_pieces);
            } else if (black_kings & square_mask) {
                king_moves(moves, pos, false, own_pieces);
            }
            square_mask = square_mask << 1;
        }
    }
    return moves;
}

void Board::pawn_moves(vector<pair<uint16_t, uint8_t>>& moves, int& pos, bool white) {
    if (white) {
        // Forward moves
        if (pos >= 8 && pos <= 15) {
            if (get_piece_at(pos+8).first == EMPTY) {
                moves.push_back(make_pair((pos | ((pos + 8) << 8)), 0));

                if (get_piece_at(pos+16).first == EMPTY) {
                    moves.push_back(make_pair((pos | ((pos + 16) << 8)), 0));
                }
            }
        } else {
            if (get_piece_at(pos+8).first == EMPTY) {
                moves.push_back(make_pair((pos | ((pos + 8) << 8)), 0));
            }
        }

        // Leftward taking Moves
        auto piece_and_val = get_piece_at((pos+7));
        Piece piece = piece_and_val.first;
        uint8_t val = piece_and_val.second;
        switch (piece) {
            case EMPTY:
                break;
            case BPAWN:
                moves.push_back(make_pair((pos | ((pos+7) << 8)), val));
                break;
            case BBISHOP:
                moves.push_back(make_pair((pos | ((pos+7) << 8)), val));
                break;
            case BKNIGHT:
                moves.push_back(make_pair((pos | ((pos+7) << 8)), val));
                break;
            case BROOK:
                moves.push_back(make_pair((pos | ((pos+7) << 8)), val));
                break;
            case BQUEEN:
                moves.push_back(make_pair((pos | ((pos+7) << 8)), val));
                break;
            default:
                break;
        }

        // Rightward Taking moves
        piece_and_val = get_piece_at((pos+9));
        piece = piece_and_val.first;
        val = piece_and_val.second;
        switch (piece) {
            case EMPTY:
                break;
            case BPAWN:
                moves.push_back(make_pair((pos | ((pos+9) << 8)), val));
                break;
            case BBISHOP:
                moves.push_back(make_pair((pos | ((pos+9) << 8)), val));
                break;
            case BKNIGHT:
                moves.push_back(make_pair((pos | ((pos+9) << 8)), val));
                break;
            case BROOK:
                moves.push_back(make_pair((pos | ((pos+9) << 8)), val));
                break;
            case BQUEEN:
                moves.push_back(make_pair((pos | ((pos+9) << 8)), val));
                break;
            default:
                break;
        }
    } else {
        // Forward moves
        if (pos >= 48 && pos <= 55) {
            if (get_piece_at(pos-8).first == EMPTY) {
                moves.push_back(make_pair((pos | ((pos - 8) << 8)), 0));

                if (get_piece_at(pos-16).first == EMPTY) {
                    moves.push_back(make_pair((pos | ((pos - 16) << 8)), 0));
                }
            }
        } else {
            if (get_piece_at(pos-8).first == EMPTY) {
                moves.push_back(make_pair((pos | ((pos - 8) << 8)), 0));
            }
        }

        // Leftward taking Moves
        auto piece_and_val = get_piece_at((pos-9));
        Piece piece = piece_and_val.first;
        uint8_t val = piece_and_val.second;
        switch (piece) {
            case EMPTY:
                break;
            case BPAWN:
                moves.push_back(make_pair((pos | ((pos-9) << 8)), val));
                break;
            case BBISHOP:
                moves.push_back(make_pair((pos | ((pos-9) << 8)), val));
                break;
            case BKNIGHT:
                moves.push_back(make_pair((pos | ((pos-9) << 8)), val));
                break;
            case BROOK:
                moves.push_back(make_pair((pos | ((pos-9) << 8)), val));
                break;
            case BQUEEN:
                moves.push_back(make_pair((pos | ((pos-9) << 8)), val));
                break;
            default:
                break;
        }

        // Rightward Taking moves
        piece_and_val = get_piece_at((pos-7));
        piece = piece_and_val.first;
        val = piece_and_val.second;
        switch (piece) {
            case EMPTY:
                break;
            case BPAWN:
                moves.push_back(make_pair((pos | ((pos-7) << 8)), val));
                break;
            case BBISHOP:
                moves.push_back(make_pair((pos | ((pos-7) << 8)), val));
                break;
            case BKNIGHT:
                moves.push_back(make_pair((pos | ((pos-7) << 8)), val));
                break;
            case BROOK:
                moves.push_back(make_pair((pos | ((pos-7) << 8)), val));
                break;
            case BQUEEN:
                moves.push_back(make_pair((pos | ((pos-7) << 8)), val));
                break;
            default:
                break;
        }
    }
}

void Board::knight_moves(vector<pair<uint16_t, uint8_t>>& moves, int& pos, Bitboard& own_pieces) {
    Bitboard possible_moves = knight_move_masks[pos];
    
    for (int target_pos = 0; target_pos < 64; ++target_pos) {
        if (possible_moves & (1ULL << target_pos)) {
            if (!(own_pieces & (1ULL << target_pos))) {
                auto piece_and_val = get_piece_at(target_pos);
                uint8_t val = piece_and_val.second;
                uint16_t move =  (target_pos << 8) | pos;
                moves.push_back(make_pair(move, val));
            }
        }
    }
}

void Board::bishop_moves(vector<pair<uint16_t, uint8_t>>& moves, int& pos, Bitboard& own_pieces, Bitboard& opponent_pieces) {
    int x = pos % 8;
    int y = pos / 8;

    // Directions: top-right, bottom-right, bottom-left, top-left
    const int directions[4][2] = { {1, 1}, {1, -1}, {-1, -1}, {-1, 1} };

    for (int d = 0; d < 4; ++d) {
        int dx = directions[d][0];
        int dy = directions[d][1];

        for (int distance = 1; distance < 8; ++distance) {
            int nx = x + dx * distance;
            int ny = y + dy * distance;

            if (nx < 0 || nx >= 8 || ny < 0 || ny >= 8) break;

            int target_pos = ny * 8 + nx;

            if (own_pieces & (1ULL << target_pos)) {
                break; // stop if a piece is blocked by own piece
            }

            auto piece_and_val = get_piece_at(target_pos);
            uint8_t val = piece_and_val.second;
            uint16_t move = pos | (target_pos << 8);
            moves.push_back(make_pair(move, val));

            if (opponent_pieces & (1ULL << target_pos)) {
                break; // stop if a piece is captured
            }
        }
    }
}

void Board::rook_moves(vector<pair<uint16_t, uint8_t>>& moves, int& pos, Bitboard& own_pieces, Bitboard& opponent_pieces) {
    int x = pos % 8;
    int y = pos / 8;

    // Directions: right, left, up, down
    const int directions[4][2] = { {1, 0}, {-1, 0}, {0, 1}, {0, -1} };

    for (int d = 0; d < 4; ++d) {
        int dx = directions[d][0];
        int dy = directions[d][1];

        for (int distance = 1; distance < 8; ++distance) {
            int nx = x + dx * distance;
            int ny = y + dy * distance;

            if (nx < 0 || nx >= 8 || ny < 0 || ny >= 8) break;

            int target_pos = ny * 8 + nx;

            if (own_pieces & (1ULL << target_pos)) {
                break; // stop if a piece is blocked by own piece
            }

            auto piece_and_val = get_piece_at(target_pos);
            uint8_t val = piece_and_val.second;
            uint16_t move = pos | (target_pos << 8);
            moves.push_back(make_pair(move, val));

            if (opponent_pieces & (1ULL << target_pos)) {
                break; // stop if a piece is captured
            }
        }
    }
}

void Board::queen_moves(vector<pair<uint16_t, uint8_t>>& moves, int& pos, Bitboard& own_pieces, Bitboard& opponent_pieces) {
    bishop_moves(moves, pos, own_pieces, opponent_pieces);
    rook_moves(moves, pos, own_pieces, opponent_pieces);
}

void Board::king_moves(vector<pair<uint16_t, uint8_t>>& moves, int& pos, bool white, Bitboard& own_pieces) {
    Bitboard possible_moves = king_move_masks[pos];

    for (int target_pos = 0; target_pos < 64; ++target_pos) {
        if (possible_moves & (1ULL << target_pos)) {
            if (!(own_pieces & (1ULL << target_pos))) {
                if (!(is_square_attacked(target_pos, white))){
                    auto piece_and_val = get_piece_at(target_pos);
                    uint8_t val = piece_and_val.second;
                    uint16_t move = pos | (target_pos << 8);
                    moves.push_back(make_pair(move, val));
                }
            }
        }
    }

    // Check and add castling moves if valid
    if (white && pos == 4) { // White king on e1
        // Castling kingside
        if (white_rooks & (1ULL << 7)) {
            if (!(own_pieces & (1ULL << 5)) && !(own_pieces & (1ULL << 6))) {
                bool is_valid_castle = true;
                for (int i = 4; i <= 6; ++i) {
                    if (is_square_attacked(i, false)) {
                        is_valid_castle = false;
                        break;
                    }
                }
                if (is_valid_castle) {
                    moves.push_back(make_pair((4 << 8) | 6, 0)); // Castling kingside
                }
            }
        }
        // Castling queenside
        if ((white_rooks & (1ULL << 0))) {
            if (!(own_pieces & (1ULL << 1)) && !(own_pieces & (1ULL << 2)) && !(own_pieces & (1ULL << 3))) {
                bool is_valid_castle = true;
                for (int i = 2; i <= 4; ++i) {
                    if (is_square_attacked(i, false)) {
                        is_valid_castle = false;
                        break;
                    }
                }
                if (is_valid_castle) {
                    moves.push_back(make_pair((4 << 8) | 2, 0)); // Castling queenside
                }
            }
        }
    } else if (!white && pos == 60) { // Black king on e8
        // Castling kingside
        if ((black_rooks & (1ULL << 63))) {
            if (!(own_pieces & (1ULL << 61)) && !(own_pieces & (1ULL << 62))) {
                bool is_valid_castle = true;
                for (int i = 60; i <= 62; ++i) {
                    if (is_square_attacked(i, true)) {
                        is_valid_castle = false;
                        break;
                    }
                }
                if (is_valid_castle) {
                    moves.push_back(make_pair((60 << 8) | 62, 0)); // Castling kingside
                }
            }
        }
        // Castling queenside
        if ((black_rooks & (1ULL << 56))) {
            if (!(own_pieces & (1ULL << 59)) && !(own_pieces & (1ULL << 58)) && !(own_pieces & (1ULL << 57))) {
                bool is_valid_castle = true;
                for (int i = 58; i <= 60; ++i) {
                    if (is_square_attacked(i, true)) {
                        is_valid_castle = false;
                        break;
                    }
                }
                if (is_valid_castle) {
                    moves.push_back(make_pair((60 << 8) | 58, 0)); // Castling queenside
                }
            }
        }
    }
}

bool Board::is_square_attacked(int square_index, bool white) {
    // Check if the square is attacked by opponent's pawns
    if (white) {
        if (square_index >= 8) {
            if (square_index % 8 != 0 && (1ULL << (square_index - 9) & black_pawns)) return true; // capture from left
            if (square_index % 8 != 7 && (1ULL << (square_index - 7) & black_pawns)) return true; // capture from right
        }
    } else {
        if (square_index <= 55) {
            if (square_index % 8 != 0 && (1ULL << (square_index + 7) & white_pawns)) return true; // capture from left
            if (square_index % 8 != 7 && (1ULL << (square_index + 9) & white_pawns)) return true; // capture from right
        }
    }

    // Check for knight attacks
    Bitboard knight_attacks = knight_move_masks[square_index];
    Bitboard knights = white ? black_knights : white_knights;
    if (knight_attacks & knights) return true;

    // Check for bishop/queen diagonal attacks
    Bitboard bishops_and_queens = white ? (black_bishops | black_queens) : (white_bishops | white_queens);
    Bitboard occupied = white ? (white_pawns | white_knights | white_bishops | white_rooks | white_queens | white_kings)
                              : (black_pawns | black_knights | black_bishops | black_rooks | black_queens | black_kings);

    for (int d = 0; d < 4; ++d) {
        int dx = (d == 0 || d == 1) ? 1 : -1;
        int dy = (d == 0 || d == 3) ? 1 : -1;
        for (int distance = 1; distance < 8; ++distance) {
            int nx = (square_index % 8) + dx * distance;
            int ny = (square_index / 8) + dy * distance;
            if (nx < 0 || nx >= 8 || ny < 0 || ny >= 8) break;

            int target_pos = ny * 8 + nx;
            if (occupied & (1ULL << target_pos)) {
                if (bishops_and_queens & (1ULL << target_pos)) return true;
                break; // blocked by another piece
            }
        }
    }

    // Check for rook/queen straight line attacks
    Bitboard rooks_and_queens = white ? (black_rooks | black_queens) : (white_rooks | white_queens);

    for (int d = 0; d < 4; ++d) {
        int dx = (d == 0) ? 1 : (d == 1) ? -1 : 0;
        int dy = (d == 2) ? 1 : (d == 3) ? -1 : 0;
        for (int distance = 1; distance < 8; ++distance) {
            int nx = (square_index % 8) + dx * distance;
            int ny = (square_index / 8) + dy * distance;
            if (nx < 0 || nx >= 8 || ny < 0 || ny >= 8) break;

            int target_pos = ny * 8 + nx;
            if (occupied & (1ULL << target_pos)) {
                if (rooks_and_queens & (1ULL << target_pos)) return true;
                break; // blocked by another piece
            }
        }
    }

    // Check for king attacks
    Bitboard king_attacks = king_move_masks[square_index];
    Bitboard kings = white ? black_kings : white_kings;
    if (king_attacks & kings) return true;

    return false;
}

vector<Bitboard> Board::generate_knight_moves() {
    vector<Bitboard> knight_moves(64, 0);
    
    for (int i = 0; i < 64; ++i) {
        Bitboard moves = 0;
        int x = i % 8;
        int y = i / 8;
        
        for (int j = 0; j < 8; ++j) {
            int nx = x + knight_offsets[j][0];
            int ny = y + knight_offsets[j][1];
            
            if (0 <= nx && nx < 8 && 0 <= ny && ny < 8) {
                moves |= 1ULL << (ny * 8 + nx);
            }
        }
        
        knight_moves[i] = moves;
    }
    
    return knight_moves;
}

vector<Bitboard> Board::generate_bishop_moves() {
    vector<Bitboard> bishop_moves(64, 0);

    for (int i = 0; i < 64; ++i) {
        Bitboard moves = 0;
        int x = i % 8;
        int y = i / 8;

        for (int dx = -1; dx <= 1; dx += 2) {
            for (int dy = -1; dy <= 1; dy += 2) {
                for (int dist = 1; dist < 8; ++dist) {
                    int nx = x + dist * dx;
                    int ny = y + dist * dy;
                    if (nx < 0 || nx >= 8 || ny < 0 || ny >= 8) {
                        break;
                    }
                    moves |= 1ULL << (ny * 8 + nx);
                }
            }
        }

        bishop_moves[i] = moves;
    }

    return bishop_moves;
}

vector<Bitboard> Board::generate_rook_moves() {
    vector<Bitboard> rook_moves(64, 0);

    for (int i = 0; i < 64; ++i) {
        Bitboard moves = 0;
        int x = i % 8;
        int y = i / 8;

        // Horizontal moves
        for (int nx = x + 1; nx < 8; ++nx) {
            moves |= 1ULL << (y * 8 + nx);
        }
        for (int nx = x - 1; nx >= 0; --nx) {
            moves |= 1ULL << (y * 8 + nx);
        }

        // Vertical moves
        for (int ny = y + 1; ny < 8; ++ny) {
            moves |= 1ULL << (ny * 8 + x);
        }
        for (int ny = y - 1; ny >= 0; --ny) {
            moves |= 1ULL << (ny * 8 + x);
        }

        rook_moves[i] = moves;
    }

    return rook_moves;
}

vector<Bitboard> Board::generate_king_moves() {
    vector<Bitboard> king_moves(64, 0);

    for (int i = 0; i < 64; ++i) {
        Bitboard moves = 0;
        int x = i % 8;
        int y = i / 8;

        // Horizontal and Vertical moves
        for (int dx = -1; dx <= 1; ++dx) {
            for (int dy = -1; dy <= 1; ++dy) {
                if (dx != 0 || dy != 0) {
                    int nx = x + dx;
                    int ny = y + dy;
                    if (nx >= 0 && nx < 8 && ny >= 0 && ny < 8) {
                        moves |= 1ULL << (ny * 8 + nx);
                    }
                }
            }
        }

        king_moves[i] = moves;
    }

    return king_moves;
}

void Board::make_move(const uint16_t& move) {
    Bitboard startingPos = 1ULL << (move & 0xFF);
    Bitboard endingPos = 1ULL << ((move >> 8) & 0xFF);

    Piece startPiece = get_piece_at(move & 0xFF).first;
    Piece endPiece = get_piece_at((move >> 8) & 0xFF).first;

    if (endPiece == EMPTY) {
        Bitboard& capturing = *piece_bitboards[startPiece];
        capturing ^= startingPos;
        capturing ^= endingPos;
    } else {
        Bitboard& captured = *piece_bitboards[endPiece];
        captured ^= endingPos;

        Bitboard& capturing = *piece_bitboards[startPiece];
        capturing ^= startingPos;
        capturing ^= endingPos;
    }
}

pair<Piece,uint8_t> Board::get_piece_at(int square_index) const {
    Bitboard square_mask = 1ULL << square_index;

    if (white_pawns & square_mask) {
        return make_pair(WPAWN, 1);
    } else if (black_pawns & square_mask) {
        return make_pair(BPAWN, 1);
    } else if (white_knights & square_mask) {
        return make_pair(WKNIGHT, 3);
    } else if (black_knights & square_mask) {
        return make_pair(BKNIGHT, 3);
    } else if (white_bishops & square_mask) {
        return make_pair(WBISHOP, 3);
    } else if (black_bishops & square_mask) {
        return make_pair(BBISHOP, 3);
    } else if (white_rooks & square_mask) {
        return make_pair(WROOK, 5);
    } else if (black_rooks & square_mask) {
        return make_pair(BROOK, 5);
    } else if (white_queens & square_mask) {
        return make_pair(WQUEEN, 9);
    } else if (black_queens & square_mask) {
        return make_pair(BQUEEN, 9);
    } else if (white_kings & square_mask) {
        return make_pair(WKING, 15);
    } else if (black_kings & square_mask) {
        return make_pair(BKING, 15);
    } else {
        return make_pair(EMPTY, 0);
    }
}

void Board::print_board() const {
    const char* piece_repr[13] = {".", "P", "N", "B", "R", "Q", "K", "p", "n", "b", "r", "q", "k"};
    for (int rank = 7; rank >= 0; --rank) {
        for (int file = 0; file < 8; ++file) {
            int square_index = rank * 8 + file;
            Piece piece = get_piece_at(square_index).first;
            cout << piece_repr[piece] << " ";
        }
        cout << endl;
    }
}

// function used to sort moves to accelerate alpha beta pruning
bool compareBySecond(const std::pair<uint16_t, uint8_t>& a, const std::pair<uint16_t, uint8_t>& b) const {
    return a.second < b.second; 
}

int Board::evaluate() const {
    return 0;
}

void Board::minimax(bool white, int alpha, int beta) const {
    // minimax function
}
