#ifndef CASTLING_H
#define CASTLING_H

#include "piece.h"

bool is_king_in_check(piece board[8][8]);

void castling_help(piece board[8][8],moving m,fen_data *data);

bool castling_help_attacks(piece board[8][8],bool direction,moving m, fen_data data);

bool castling(moving *m, fen_data *data, piece board[8][8]);

void castling_action(piece board[8][8],piece temp_board[8][8], moving m);

#endif //CASTLING_H
