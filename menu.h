#ifndef MENU_H
#define MENU_H

#include "piece.h"

int menu1();

int promotion_menu(piece board[8][8],moving *m, fen_data *data);

void ending_menu(int eredmeny,char *gyoztes);

#endif //MENU_H
