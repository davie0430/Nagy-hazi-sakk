#ifndef BOARDPRT_H
#define BOARDPRT_H

#include "piece.h"

void print_piece(int color, piece_type type);

void clear_terminal();

void chessboard_print(piece chessboard[8][8]);

#endif //BOARDPRT_H
