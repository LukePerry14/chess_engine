#include <utility>
#include <vector>
#include <bitset>
#include <algorithm>
#include <climits>
#include <cmath>
#include <limits>
#include "board.h"

typedef uint64_t Bitboard;
using namespace std;

Board::Board() {
    // Initialize bitboards for starting position
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
    maxDepth = 5;

    knight_move_masks = generate_knight_moves();
    bishop_move_masks = generate_bishop_moves();
    rook_move_masks = generate_rook_moves();
    for (int t = 0; t < 64; t++) {
        queen_move_masks.push_back(rook_move_masks[t] | bishop_move_masks[t]);
    }
    king_move_masks = generate_king_moves();

    float alpha = numeric_limits<float>::lowest();
    float beta = numeric_limits<float>::max();
    auto ret = minimax(true, alpha, beta, maxDepth, piece_bitboards);
    cout << ret.first << endl;
    cout << bitset<16>(ret.second) << endl;
}

pair<vector<pair<uint16_t, uint8_t>>, float> Board::generate_legal_moves_and_evaluate(bool white, unordered_map<Piece, Bitboard> pieces) {
    vector<pair<uint16_t, uint8_t>> white_moves;
    vector<pair<uint16_t, uint8_t>> black_moves;
    Bitboard white_pawns = pieces[WPAWN];
    Bitboard white_knights = pieces[WKNIGHT];
    Bitboard white_bishops = pieces[WBISHOP];
    Bitboard white_rooks = pieces[WROOK];
    Bitboard white_queens = pieces[WQUEEN];
    Bitboard white_kings = pieces[WKING];

    Bitboard black_pawns = pieces[BPAWN];
    Bitboard black_knights = pieces[BKNIGHT];
    Bitboard black_bishops = pieces[BBISHOP];
    Bitboard black_rooks = pieces[BROOK];
    Bitboard black_queens = pieces[BQUEEN];
    Bitboard black_kings = pieces[BKING];

    int white_king_pos = find_king_position(white_kings);
    int black_king_pos = find_king_position(black_kings);

    Bitboard own_pieces = white ? (white_pawns | white_knights | white_bishops | white_rooks | white_queens | white_kings)
                                : (black_pawns | black_knights | black_bishops | black_rooks | black_queens | black_kings);

    Bitboard opponent_pieces = white ? (black_pawns | black_knights | black_bishops | black_rooks | black_queens | black_kings)
                                     : (white_pawns | white_knights | white_bishops | white_rooks | white_queens | white_kings);

    float score = 0;

    Bitboard square_mask = 1ULL;
    for (int pos = 0; pos < 64; pos++) {
        if (pos > 0) {
            square_mask = square_mask << 1;
        }

        if (pos > 7) {
            if (white_pawns & square_mask) {
                score +=1;
                pawn_moves(white_moves, pos, true, black_king_pos, pieces);
                score += (white_pawn_value_mask[pos] + 
                    ((((pos + 7) % 8) == ((pos % 8) - 1) && (pos + 7 == black_king_pos)) || 
                    (((pos + 9) % 8) == ((pos % 8) + 1) && (pos + 9 == black_king_pos))) ? 4 : 0); // Add bonus value if check

                continue;
            }
        }
        if (white_knights & square_mask) {
            score += 3;
            int before_moves = white_moves.size();
            knight_moves(white_moves, pos, own_pieces, black_king_pos, pieces);
            int after_moves = white_moves.size();
            score += (0.1 * (after_moves - before_moves) * knight_value_mask[pos]);

            size_t start_index = white_moves.size() - (after_moves - before_moves);
            for (size_t i = start_index; i < white_moves.size(); ++i) {
                if (white_moves[i].first == black_king_pos) {
                    score += 4;
                }
            }
            continue;
        } else if (white_bishops & square_mask) {
            score += 3;
            int before_moves = white_moves.size();
            bishop_moves(white_moves, pos, own_pieces, opponent_pieces, black_king_pos, pieces);
            int after_moves = white_moves.size();
            score += (0.1 * (after_moves - before_moves) * bishop_value_mask[pos]);

            size_t start_index = white_moves.size() - (after_moves - before_moves);
            for (size_t i = start_index; i < white_moves.size(); ++i) {
                if (white_moves[i].first == black_king_pos) {
                    score += 4;
                }
            }
            continue;
        } else if (white_rooks & square_mask) {
            score += 5;
            int before_moves = white_moves.size();
            rook_moves(white_moves, pos, own_pieces, opponent_pieces, black_king_pos, pieces);
            int after_moves = white_moves.size();
            score += (0.1 * (after_moves - before_moves) * rook_value_mask[pos]);

            size_t start_index = white_moves.size() - (after_moves - before_moves);
            for (size_t i = start_index; i < white_moves.size(); ++i) {
                if (white_moves[i].first == black_king_pos) {
                    score += 4;
                }
            }
            continue;
        } else if (white_queens & square_mask) {
            score += 9;
            int before_moves = white_moves.size();
            queen_moves(white_moves, pos, own_pieces, opponent_pieces, black_king_pos, pieces);
            int after_moves = white_moves.size();
            score += (0.1 * (after_moves - before_moves) * queen_value_mask[pos]);

            size_t start_index = white_moves.size() - (after_moves - before_moves);
            for (size_t i = start_index; i < white_moves.size(); ++i) {
                if (white_moves[i].first == black_king_pos) {
                    score += 4;
                }
            }
            continue;
        } else if (white_kings & square_mask) {
            int before_moves = white_moves.size();
            king_moves(white_moves, pos, true, own_pieces, pieces);
            int after_moves = white_moves.size();
            if (after_moves - before_moves == 0) {
                score = numeric_limits<float>::lowest();
                break;
            }
            continue;
        }
        if (pos < 56) {
            if (black_pawns & square_mask) {
                pawn_moves(black_moves, pos, false, white_king_pos, pieces);
                score -= (white_pawn_value_mask[pos] + 
                    ((((pos + 7) % 8) == ((pos % 8) - 1) && (pos + 7 == white_king_pos)) || 
                    (((pos + 9) % 8) == ((pos % 8) + 1) && (pos + 9 == white_king_pos))) ? 4 : 0); // Add bonus value if check
                continue;
            }
        }
        if (black_knights & square_mask) {
            score -= 3;
            int before_moves = black_moves.size();
            knight_moves(black_moves, pos, own_pieces, white_king_pos, pieces);
            int after_moves = black_moves.size();
            score -= (0.1 * (after_moves - before_moves) * knight_value_mask[pos]);

            size_t start_index = black_moves.size() - (after_moves - before_moves);
            for (size_t i = start_index; i < black_moves.size(); ++i) {
                if (black_moves[i].first == white_king_pos) {
                    score -= 4;
                }
            }
            continue;
        } else if (black_bishops & square_mask) {
            score -= 3;
            int before_moves = black_moves.size();
            bishop_moves(black_moves, pos, own_pieces, opponent_pieces, white_king_pos, pieces);
            int after_moves = black_moves.size();
            score -= (0.1 * (after_moves - before_moves) * bishop_value_mask[pos]);

            size_t start_index = black_moves.size() - (after_moves - before_moves);
            for (size_t i = start_index; i < black_moves.size(); ++i) {
                if (black_moves[i].first == white_king_pos) {
                    score -= 4;
                }
            }
            continue;
        } else if (black_rooks & square_mask) {
            score -= 5;
            int before_moves = black_moves.size();
            rook_moves(black_moves, pos, own_pieces, opponent_pieces, white_king_pos, pieces);
            int after_moves = black_moves.size();
            score -= (0.1 * (after_moves - before_moves) * rook_value_mask[pos]);

            size_t start_index = black_moves.size() - (after_moves - before_moves);
            for (size_t i = start_index; i < black_moves.size(); ++i) {
                if (black_moves[i].first == white_king_pos) {
                    score -= 4;
                }
            }
            continue;
        } else if (black_queens & square_mask) {
            score -= 9;
            int before_moves = black_moves.size();
            queen_moves(black_moves, pos, own_pieces, opponent_pieces, white_king_pos, pieces);
            int after_moves = black_moves.size();
            score -= (0.1 * (after_moves - before_moves) * queen_value_mask[pos]);

            size_t start_index = black_moves.size() - (after_moves - before_moves);
            for (size_t i = start_index; i < black_moves.size(); ++i) {
                if (black_moves[i].first == white_king_pos) {
                    score -= 4;
                }
            }
            continue;
        } else if (black_kings & square_mask) {
            int before_moves = black_moves.size();
            king_moves(black_moves, pos, false, own_pieces, pieces);
            int after_moves = black_moves.size();
            
            if (after_moves - before_moves == 0) {
                score = numeric_limits<float>::lowest();
                break;
            }
            continue;
        }
    }
    vector<pair<uint16_t, uint8_t>>& moves = white? white_moves : black_moves;
    return make_pair(moves, score);
}

int Board::find_king_position(Bitboard king_board) {
    for (int pos = 0; pos < 64; ++pos) {
        if (king_board & (1ULL << pos)) {
            return pos;
        }
    }
    // Return -1 if the king is not found
    return -1;
}

void Board::pawn_moves(vector<pair<uint16_t, uint8_t>>& moves, int& pos, bool white, int& enemy_king_pos, unordered_map<Piece, Bitboard>& pieces) {
    uint8_t bonus = 0;
    pair<Piece,uint8_t> piece_and_val;
    Piece piece;
    uint8_t val;
    if (white) {
        // Forward moves
        if (pos >= 8 && pos <= 15) {
            if (get_piece_at(pos+8, pieces).first == EMPTY) {
                if ((((pos + 15) % 8) == ((pos % 8) - 1) && (pos + 15 == enemy_king_pos)) || (((pos + 17) % 8) == ((pos % 8) + 1) && (pos + 17 == enemy_king_pos))){
                    bonus += 2;
                }
                moves.push_back(make_pair((pos | ((pos + 8) << 8)), 0 + bonus));

                if (get_piece_at(pos+16, pieces).first == EMPTY) {
                    if ((((pos + 23) % 8) == ((pos % 8) - 1) && (pos + 23 == enemy_king_pos)) || (((pos + 25) % 8) == ((pos % 8) + 1) && (pos + 25 == enemy_king_pos))){
                        bonus += 2;
                    }
                    moves.push_back(make_pair((pos | ((pos + 16) << 8)), 0 + bonus));
                }
            }
        } else if (pos <=55) {
            if (get_piece_at(pos+8, pieces).first == EMPTY) {
                moves.push_back(make_pair((pos | ((pos + 8) << 8)), 0)); // add logic to upgrade pawn
            }
        }

        // Leftward taking Moves
        if ((pos + 7) % 8 == (pos & 8) - 1) {
            piece_and_val = get_piece_at((pos+7), pieces);
            piece = piece_and_val.first;
            val = piece_and_val.second;
            if ((((pos + 14) % 8) == ((pos % 8) - 1) && (pos + 14 == enemy_king_pos)) || (((pos + 16) % 8) == ((pos % 8) + 1) && (pos + 16 == enemy_king_pos))) {
                bonus += 2;
            }
            switch (piece) {
                case EMPTY:
                    break;
                case BPAWN:
                    moves.push_back(make_pair((pos | ((pos+7) << 8)), val + bonus));
                    break;
                case BBISHOP:
                    moves.push_back(make_pair((pos | ((pos+7) << 8)), val + bonus));
                    break;
                case BKNIGHT:
                    moves.push_back(make_pair((pos | ((pos+7) << 8)), val + bonus));
                    break;
                case BROOK:
                    moves.push_back(make_pair((pos | ((pos+7) << 8)), val + bonus));
                    break;
                case BQUEEN:
                    moves.push_back(make_pair((pos | ((pos+7) << 8)), val + bonus));
                    break;
                default:
                    break;
            }
        }

        // Rightward Taking moves
        if ((pos + 9) % 8 == (pos & 8) + 1) {
            piece_and_val = get_piece_at((pos+9), pieces);
            piece = piece_and_val.first;
            val = piece_and_val.second;
            if ((((pos + 16) % 8) == ((pos % 8) - 1) && (pos + 16 == enemy_king_pos)) || (((pos + 18) % 8) == ((pos % 8) + 1) && (pos + 18 == enemy_king_pos))) {
                bonus += 2;
            }
            switch (piece) {
                case EMPTY:
                    break;
                case BPAWN:
                    moves.push_back(make_pair((pos | ((pos+9) << 8)), val + bonus));
                    break;
                case BBISHOP:
                    moves.push_back(make_pair((pos | ((pos+9) << 8)), val + bonus));
                    break;
                case BKNIGHT:
                    moves.push_back(make_pair((pos | ((pos+9) << 8)), val + bonus));
                    break;
                case BROOK:
                    moves.push_back(make_pair((pos | ((pos+9) << 8)), val + bonus));
                    break;
                case BQUEEN:
                    moves.push_back(make_pair((pos | ((pos+9) << 8)), val + bonus));
                    break;
                default:
                    break;
            }
        }
    } else {
        // Forward moves
        if (pos >= 48 && pos <= 55) {
            if (get_piece_at(pos-8, pieces).first == EMPTY) {
                if ((((pos - 17) % 8) == ((pos % 8) - 1) && (pos - 17 == enemy_king_pos)) || (((pos - 15) % 8) == ((pos % 8) + 1) && (pos - 15 == enemy_king_pos))){
                    bonus += 2;
                }
                moves.push_back(make_pair((pos | ((pos - 8) << 8)), 0));

                if (get_piece_at(pos-16, pieces).first == EMPTY) {
                    if ((((pos - 27) % 8) == ((pos % 8) - 1) && (pos - 27 == enemy_king_pos)) || (((pos - 25) % 8) == ((pos % 8) + 1) && (pos - 25 == enemy_king_pos))){
                        bonus += 2;
                    }
                    moves.push_back(make_pair((pos | ((pos - 16) << 8)), 0));
                }
            }
        } else if(pos >= 8) {
            if (get_piece_at(pos-8, pieces).first == EMPTY) {
                moves.push_back(make_pair((pos | ((pos - 8) << 8)), 0)); // add logic to upgrade pawn
            }
        }

        // Leftward taking Moves
        if ((pos - 9) % 8 == (pos & 8) - 1) {
            piece_and_val = get_piece_at((pos-9), pieces);
            piece = piece_and_val.first;
            val = piece_and_val.second;
            if ((((pos - 18) % 8) == ((pos % 8) - 1) && (pos - 18 == enemy_king_pos)) || (((pos - 16) % 8) == ((pos % 8) + 1) && (pos - 16 == enemy_king_pos))) {
                bonus += 2;
            }
            switch (piece) {
                case EMPTY:
                    break;
                case BPAWN:
                    moves.push_back(make_pair((pos | ((pos-9) << 8)), val + bonus));
                    break;
                case BBISHOP:
                    moves.push_back(make_pair((pos | ((pos-9) << 8)), val + bonus));
                    break;
                case BKNIGHT:
                    moves.push_back(make_pair((pos | ((pos-9) << 8)), val + bonus));
                    break;
                case BROOK:
                    moves.push_back(make_pair((pos | ((pos-9) << 8)), val + bonus));
                    break;
                case BQUEEN:
                    moves.push_back(make_pair((pos | ((pos-9) << 8)), val + bonus));
                    break;
                default:
                    break;
            }
        }
        // Rightward Taking moves
        if ((pos - 7) % 8 == (pos & 8) + 1) {
            piece_and_val = get_piece_at((pos-7), pieces);
            piece = piece_and_val.first;
            val = piece_and_val.second;
            if ((((pos - 16) % 8) == ((pos % 8) - 1) && (pos - 16 == enemy_king_pos)) || (((pos - 14) % 8) == ((pos % 8) + 1) && (pos - 14 == enemy_king_pos))) {
                bonus += 2;
            }
            switch (piece) {
                case EMPTY:
                    break;
                case BPAWN:
                    moves.push_back(make_pair((pos | ((pos-7) << 8)), val + bonus));
                    break;
                case BBISHOP:
                    moves.push_back(make_pair((pos | ((pos-7) << 8)), val + bonus));
                    break;
                case BKNIGHT:
                    moves.push_back(make_pair((pos | ((pos-7) << 8)), val + bonus));
                    break;
                case BROOK:
                    moves.push_back(make_pair((pos | ((pos-7) << 8)), val + bonus));
                    break;
                case BQUEEN:
                    moves.push_back(make_pair((pos | ((pos-7) << 8)), val + bonus));
                    break;
                default:
                    break;
            }
        }
    }
}

void Board::knight_moves(vector<pair<uint16_t, uint8_t>>& moves, int& pos, Bitboard& own_pieces, int& enemy_king_pos, unordered_map<Piece, Bitboard>& pieces) {
    Bitboard possible_moves = knight_move_masks[pos];
    int bonus = 0;
    
    for (int target_pos = 0; target_pos < 64; ++target_pos) {
        if (possible_moves & (1ULL << target_pos)) {
            if (!(own_pieces & (1ULL << target_pos))) {
                auto piece_and_val = get_piece_at(target_pos, pieces);
                uint8_t val = piece_and_val.second;
                uint16_t move =  (target_pos << 8) | pos;
                if (((target_pos + 15 < 63) && (((target_pos+15) % 8 == (target_pos%8) - 1) && (target_pos + 15 == enemy_king_pos))) || ((target_pos+17 < 63) &&(((target_pos+17) % 8 == (target_pos%8) + 1) && (target_pos + 17 == enemy_king_pos)))) {
                    bonus = 4; // check in upward direction
                } else if (((target_pos + 10 < 63) && (((target_pos+10) % 8 == (target_pos%8) + 2) && (target_pos + 10 == enemy_king_pos))) || ((target_pos + 6 < 63) &&(((target_pos+6) % 8 == (target_pos%8) - 2) && (target_pos + 6 == enemy_king_pos)))) {
                    bonus = 4; // check in top half of semi-sphere
                } else if (((target_pos - 10 > 0) && (((target_pos-10) % 8 == (target_pos%8) - 2) && (target_pos - 10 == enemy_king_pos))) || ((target_pos - 6 > 0) &&(((target_pos-6) % 8 == (target_pos%8) + 2) && (target_pos - 6 == enemy_king_pos)))) {
                    bonus = 4; // check in downard direction
                } else if (((target_pos - 17 > 0) && (((target_pos-17) % 8 == (target_pos%8) -1) && (target_pos - 17 == enemy_king_pos))) || ((target_pos - 15 > 0) &&(((target_pos-15) % 8 == (target_pos%8) + 1) && (target_pos - 15 == enemy_king_pos)))) {
                    bonus = 4; // // check in bottom half of semi-sphere
                }
                moves.push_back(make_pair(move, val + bonus));
            }
        }
    }
}

void Board::bishop_moves(vector<pair<uint16_t, uint8_t>>& moves, int& pos, Bitboard& own_pieces, Bitboard& opponent_pieces, int& enemy_king_pos, unordered_map<Piece, Bitboard>& pieces) {
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
                break;
            }

            auto piece_and_val = get_piece_at(target_pos, pieces);
            uint8_t val = piece_and_val.second;
            uint16_t move = pos | (target_pos << 8);
            for (int m = 0; m < 4; ++m) {
                int mdx = directions[m][0];
                int mdy = directions[m][1];

                for (int mdistance = 1; mdistance < 8; ++mdistance) {
                    int mnx = x + mdx * mdistance;
                    int mny = y + mdy * mdistance;

                    if (mnx < 0 || mnx >= 8 || mny < 0 || mny >= 8) break;

                    int mtarget_pos = mny * 8 + mnx;

                    if (target_pos == enemy_king_pos) {
                        val += 2;
                    }
                    if ((own_pieces & (1ULL << mtarget_pos)) || (opponent_pieces & (1ULL << mtarget_pos))) {
                        break;
                    }

                }
            }
            moves.push_back(make_pair(move, val));

            if (opponent_pieces & (1ULL << target_pos)) {
                break; // stop if a piece is captured
            }
        }
    }
}

void Board::rook_moves(vector<pair<uint16_t, uint8_t>>& moves, int& pos, Bitboard& own_pieces, Bitboard& opponent_pieces, int& enemy_king_pos, unordered_map<Piece, Bitboard>& pieces) {
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

            auto piece_and_val = get_piece_at(target_pos, pieces);
            uint8_t val = piece_and_val.second;
            uint16_t move = pos | (target_pos << 8);
            for (int m = 0; m < 4; ++m) {
                int mdx = directions[m][0];
                int mdy = directions[m][1];

                for (int mdistance = 1; mdistance < 8; ++mdistance) {
                    int mnx = x + mdx * mdistance;
                    int mny = y + mdy * mdistance;

                    if (mnx < 0 || mnx >= 8 || mny < 0 || mny >= 8) break;

                    int mtarget_pos = mny * 8 + mnx;

                    if (mtarget_pos == enemy_king_pos) {
                        val +=2;
                    }

                    if ((own_pieces & (1ULL << target_pos)) || (opponent_pieces & (1ULL << target_pos))){
                        break;
                    }
                }
            }
            moves.push_back(make_pair(move, val));

            if (opponent_pieces & (1ULL << target_pos)) {
                break; // stop if a piece is captured
            }
        }
    }
}

void Board::queen_moves(vector<pair<uint16_t, uint8_t>>& moves, int& pos, Bitboard& own_pieces, Bitboard& opponent_pieces, int& enemy_king_pos, unordered_map<Piece, Bitboard>& pieces) {
    bishop_moves(moves, pos, own_pieces, opponent_pieces, enemy_king_pos, pieces);
    rook_moves(moves, pos, own_pieces, opponent_pieces, enemy_king_pos, pieces);
}

void Board::king_moves(vector<pair<uint16_t, uint8_t>>& moves, int& pos, bool white, Bitboard& own_pieces, unordered_map<Piece, Bitboard>& pieces) {
    Bitboard possible_moves = king_move_masks[pos];

    for (int target_pos = 0; target_pos < 64; ++target_pos) {
        if (possible_moves & (1ULL << target_pos)) {
            if (!(own_pieces & (1ULL << target_pos))) {
                if (!(is_square_attacked(target_pos, white, pieces))){
                    auto piece_and_val = get_piece_at(target_pos, pieces);
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
                    if (is_square_attacked(i, false, pieces)) {
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
                    if (is_square_attacked(i, false, pieces)) {
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
                    if (is_square_attacked(i, true, pieces)) {
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
                    if (is_square_attacked(i, true, pieces)) {
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

bool Board::is_square_attacked(int square_index, bool white, unordered_map<Piece, Bitboard>& pieces) {
    Bitboard white_pawns = pieces[WPAWN];
    Bitboard white_knights = pieces[WKNIGHT];
    Bitboard white_bishops = pieces[WBISHOP];
    Bitboard white_rooks = pieces[WROOK];
    Bitboard white_queens = pieces[WQUEEN];
    Bitboard white_kings = pieces[WKING];

    Bitboard black_pawns = pieces[BPAWN];
    Bitboard black_knights = pieces[BKNIGHT];
    Bitboard black_bishops = pieces[BBISHOP];
    Bitboard black_rooks = pieces[BROOK];
    Bitboard black_queens = pieces[BQUEEN];
    Bitboard black_kings = pieces[BKING];
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

unordered_map<Piece, Bitboard> Board::make_move(const uint16_t& move, unordered_map<Piece, Bitboard>& pieces) {
    Bitboard startingPos = 1ULL << (move & 0xFF);
    Bitboard endingPos = 1ULL << ((move >> 8) & 0xFF);

    Piece startPiece = get_piece_at(move & 0xFF, pieces).first;
    Piece endPiece = get_piece_at((move >> 8) & 0xFF, pieces).first;

    if (endPiece == EMPTY) {
        Bitboard& capturing = pieces[startPiece];
        capturing ^= startingPos;
        capturing ^= endingPos;
    } else {
        Bitboard& captured = pieces[endPiece];
        captured ^= endingPos;

        Bitboard& capturing = pieces[startPiece];
        capturing ^= startingPos;
        capturing ^= endingPos;
    }

    return pieces;
}

pair<Piece, uint8_t> Board::get_piece_at(int square_index, const unordered_map<Piece, Bitboard>& pieces) const {
    const Bitboard white_pawns = pieces.at(WPAWN);
    const Bitboard white_knights = pieces.at(WKNIGHT);
    const Bitboard white_bishops = pieces.at(WBISHOP);
    const Bitboard white_rooks = pieces.at(WROOK);
    const Bitboard white_queens = pieces.at(WQUEEN);
    const Bitboard white_kings = pieces.at(WKING);

    const Bitboard black_pawns = pieces.at(BPAWN);
    const Bitboard black_knights = pieces.at(BKNIGHT);
    const Bitboard black_bishops = pieces.at(BBISHOP);
    const Bitboard black_rooks = pieces.at(BROOK);
    const Bitboard black_queens = pieces.at(BQUEEN);
    const Bitboard black_kings = pieces.at(BKING);

    const Bitboard square_mask = 1ULL << square_index;

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
            Piece piece = get_piece_at(square_index, piece_bitboards).first;
            cout << piece_repr[piece] << " ";
        }
        cout << endl;
    }
}

// function used to sort moves to accelerate alpha beta pruning
bool Board::compareBySecond(const std::pair<uint16_t, uint8_t>& a, const std::pair<uint16_t, uint8_t>& b) {
    return a.second < b.second; 
}


pair<float, uint16_t> Board::minimax(bool white, float& alpha, float& beta, int depth, unordered_map<Piece, Bitboard> pieces) {

    if (depth == 0){
        auto eval_and_moves = generate_legal_moves_and_evaluate(white, pieces);
        float eval = eval_and_moves.second;
        uint16_t sub = 0;
        return make_pair(eval, sub);
    }

    if (white) { // Maximising value
        float maxEval = numeric_limits<float>::lowest();
        uint16_t takenMove;
        auto eval_and_moves = generate_legal_moves_and_evaluate(true, pieces);
        vector<pair<uint16_t, uint8_t>> moves = eval_and_moves.first;
        sort(moves.begin(), moves.end(), Board::compareBySecond);

        for (auto move : moves) {
            unordered_map<Piece, Bitboard> npieces = make_move(move.first, pieces);
            auto eval_and_move = minimax(false, alpha, beta, depth-1, npieces);
            float eval = eval_and_move.first;
            maxEval = max(maxEval, eval);
            if (maxEval == eval) {
                takenMove = move.first;
            }
            alpha = max(alpha, eval);
            if (beta <= alpha) {
                break;
            }
        }
        return make_pair(maxEval, takenMove);

    } else { // Minimising value
        float minEval = numeric_limits<float>::max();
        uint16_t takenMove;
        auto eval_and_moves = generate_legal_moves_and_evaluate(false, pieces);
        vector<pair<uint16_t, uint8_t>> moves = eval_and_moves.first;
        sort(moves.begin(), moves.end(), Board::compareBySecond);

        for (auto move : moves) {
            unordered_map<Piece, Bitboard> npieces = make_move(move.first, pieces);
            auto eval_and_move = minimax(true, alpha, beta, depth-1, npieces);
            float eval = eval_and_move.first;
            minEval = min(minEval, eval);
            if (minEval == eval) {
                takenMove = move.first;
            }
            beta = min(alpha, eval);
            if (beta <= alpha) {
                break;
            }
        }
        return make_pair(minEval, takenMove);
    }

}
