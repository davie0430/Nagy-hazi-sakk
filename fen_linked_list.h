#ifndef CHESSMOD_FEN_LINKED_LIST_H
#define CHESSMOD_FEN_LINKED_LIST_H

#include "piece.h"

fen_node* initialize_fenlist();

void fenlist_append(fen_node** head_reference,const char *new_board);

void fenlist_print(fen_node *head);

void fenlist_free(fen_node **head);

fen_node *move_to_end(fen_node *head);

fen_node *simple_print(fen_node *current);

fen_node *move_to_previous(fen_node *current);

fen_node *move_to_next(fen_node *current);

int get_list_size(fen_node *head);

#endif //CHESSMOD_FEN_LINKED_LIST_H
