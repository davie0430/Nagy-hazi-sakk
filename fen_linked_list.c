#include <stdio.h>
#include <string.h>
#include "piece.h"
#include "fen_code_to_chessboard_converter.h"
#include "boardprt.h"
#include "debugmalloc.h"

fen_node* initialize_fenlist() {
    return NULL;
}

void fenlist_append(fen_node** head_reference,const char *new_board) {
    fen_node* newfen_node = (fen_node*)malloc(sizeof(fen_node));
    if (newfen_node == NULL) {
        printf("Nem sikerült memóriát foglalni.\n");
        return;
    }
    strcpy(newfen_node->data,new_board);
    newfen_node->next = NULL;

    if (*head_reference == NULL) {
        newfen_node->prev = NULL;
        *head_reference = newfen_node;
        return;
    }
    fen_node* temp = *head_reference;
    while (temp->next != NULL) {
        temp = temp->next;
    }
    temp->next = newfen_node;
    newfen_node->prev = temp;
}

void fenlist_print(fen_node *head) {
    printf("\nLinked List:\n");
    while (head != NULL) {
        printf("%s\n", head->data);
        head = head->next;
    }
    printf("\n");
}

void fenlist_free(fen_node **head) {
    fen_node *current = *head;
    fen_node *temp;
    while (current != NULL) {
        temp = current;
        current = current->next;
        free(temp);
    }
    *head = NULL;
}

fen_node *move_to_end(fen_node *head) {
    fen_node* temp = head;
    while (temp != NULL && temp->next != NULL) {
        temp = temp->next;
    }
    return temp;
}

fen_node* simple_print(fen_node *current) {
    piece show_board[8][8] = {0};
    fen_data apple;
    fen_to_chessboard(current->data, show_board, &apple, false);
    chessboard_print(show_board);
    return current;
}

fen_node* move_to_previous(fen_node *current) {
    if (current->prev != NULL) {
        current = current->prev;
    }
    return current;
}

fen_node* move_to_next(fen_node *current) {
    if (current->next != NULL) {
        current = current->next;
    }
    return current;
}

int get_list_size(fen_node *head) {
    int count = 0;
    fen_node *current = head;
    while (current != NULL) {
        count++;
        current = current->next;
    }
    return count;
}