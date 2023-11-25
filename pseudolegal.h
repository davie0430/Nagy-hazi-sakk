#ifndef PSEUDOLEGAL_H
#define PSEUDOLEGAL_H

#include "piece.h"
#include <stdbool.h>

bool psl_pawn_blockade(int from_row, int from_col, int x, piece board[8][8]);

void legality_reset(piece board[8][8]);

void pseudo_legal_moves(moving *m,piece board[8][8]);

bool is_there_even_legal_move(piece board[8][8]);

#endif //PSEUDOLEGAL_H
