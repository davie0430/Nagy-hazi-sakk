#ifndef CHESSMOD_SAVE_LOAD_H
#define CHESSMOD_SAVE_LOAD_H

#include "piece.h"

void savings(fen_node *fen_list);

void nezet(fen_node *fen_list);

int file_load(fen_node **head);

#endif //CHESSMOD_SAVE_LOAD_H
