#ifndef FENTOCB_H
#define FENTOCB_H

#include "piece.h"

void castling_rights(char token_char, fen_data *data);

void fen_to_chessboard(char fen[100],piece board[8][8],fen_data *data);

#endif //FENTOCB_H