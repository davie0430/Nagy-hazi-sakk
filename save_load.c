#include <stdio.h>
#include <stdbool.h>
#include "piece.h"
#include "fen_linked_list.h"
#include "boardprt.h"
#include "menu.h"

void nezet(fen_node *fen_list) { //játszmák megtekintése (vagy a jelenlegi, vagy a betöltött)
    int size = get_list_size(fen_list);
    int count = size;
    fen_node* travel_node = move_to_end(fen_list);
    fenlist_print(travel_node);
    int action;
    bool nezet_mode = true;
    while (nezet_mode) {
        simple_print(travel_node);
        action = nezet_menu();
        if (action == 1) {
            if (count < size) {
                travel_node = move_to_next(travel_node);
                count++;
            }
        }
        if (action == 2) {
            if (count != 0) {
                travel_node = move_to_previous(travel_node);
                count--;
            }
        }
        if (action == 3) {
            nezet_mode = false;
            clear_terminal();
        }
    }
}

void savings(fen_node *head) { //mentjük a játszmát
    FILE *file_pointer;
    file_pointer = fopen("saves.txt", "w");

    if (file_pointer != NULL) {
        while (head != NULL) {
            fprintf(file_pointer, "%s ", head->data);
            head = head->next;
        }
        fprintf(file_pointer, "\n");
        fclose(file_pointer);
    } else {
        printf("Hiba történt a fájl megnyitása során.\n");
    }
}

int file_load(fen_node **fen_list) { //betöltjük a mentést ha már volt előző
    FILE *file_pointer;
    file_pointer = fopen("saves.txt", "r");
    if (file_pointer != NULL) {
        char string[100];
        while (fscanf(file_pointer, "%s", string) == 1) {
            fenlist_append(&*fen_list,string);
        }
        fenlist_print(*fen_list);
        fclose(file_pointer);
        return 0;
    } else {
        return -1;
    }
}
