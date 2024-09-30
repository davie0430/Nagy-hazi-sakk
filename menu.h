#ifndef MENU_H
#define MENU_H

#include "piece.h"
#include <stdbool.h>

int menu1();

int nezet_menu();

int promotion_menu(piece board[8][8],moving *m, fen_data *data);

bool ending_menu(int eredmeny,char *gyoztes,fen_node *fen_list);

#endif //MENU_H
