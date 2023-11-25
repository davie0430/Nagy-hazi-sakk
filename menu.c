#include "menu.h"
#include "piece.h"
#include "boardprt.h"
#include <stdio.h>
#include <stdbool.h>


int menu1() {
    printf(ANSI_YELLOW"Kovács Dávid Ákos: SAKK\n\n");
    printf(" "ANSI_WHITE"1 "ANSI_RESET". Játék indítása\n");
    printf(" "ANSI_BLACK"2 "ANSI_RESET". Játék indítása speciális állástól\n");
    printf(" "ANSI_WHITE"3 "ANSI_RESET". Játszma betöltése\n");
    printf(" "ANSI_BLACK"4 "ANSI_RESET". Kilépés\n\n");
    printf(ANSI_RESET"Válassz " ANSI_WHITE"1 "ANSI_RESET" és "ANSI_BLACK"4 "ANSI_RESET" között (pl.: \"2\"): \n\n");
    int option, c;
    bool valid = false;
    do {
        if (scanf("%d", &option) == 1) {
            if (option > 0 && option < 5) {
                valid = true;
            } else {
                printf(ANSI_RED"HIBA: Hibás formátum.\n"ANSI_RESET);
            }
        } else {
            while (c = getchar(), c != '\n' && c != EOF);
            printf(ANSI_RED"HIBA: Hibás formátum.\n"ANSI_RESET);
        }
    } while (!valid);
    return option;
}

int promotion_menu(piece board[8][8],moving *m, fen_data *data) {
    printf("\nMelyik bábuvá akarsz fejlődni?\n");
    printf("1 Huszár\n2 Futó\n3 Bástya\n4 Vezér\n");
    int option, c;
    bool valid = false;
    do {
        if (scanf("%d", &option) == 1) {
            if (option > 0 && option < 5) {
                valid = true;
            } else {
                printf(ANSI_RED"HIBA: Hibás formátum.\n"ANSI_RESET);
            }
        } else {
            while (c = getchar(), c != '\n' && c != EOF);
            printf(ANSI_RED"HIBA: Hibás formátum.\n"ANSI_RESET);
        }
    } while (!valid);
    switch (option) {
        case 1:
            board[m->from_row][m->from_col].type = knight;
            board[m->from_row][m->from_col].color = !data->color;
            break;
        case 2:
            board[m->from_row][m->from_col].type = bishop;
            board[m->from_row][m->from_col].color = !data->color;
            break;
        case 3:
            board[m->from_row][m->from_col].type = rook;
            board[m->from_row][m->from_col].color = !data->color;
            break;
        case 4:
            board[m->from_row][m->from_col].type = queen;
            board[m->from_row][m->from_col].color = !data->color;
            break;
        default:
            break;
    }
    return option;
}

void ending_menu(int eredmeny,char *gyoztes) {
    if (eredmeny == 1 || eredmeny == 0) {
        printf("\nJáték vége! %s győzött.\n", gyoztes);
    } else {
        printf("Játék vége! Döntetlen.");
    }
    printf("1 Játék mentése\n2 Vissza a főmenübe\n3 Kilépés\n");
    int option, c;
    bool valid = false;
    do {
        if (scanf("%d", &option) == 1) {
            if (option > 0 && option < 4) {
                valid = true;
            } else {
                printf(ANSI_RED"HIBA: Hibás formátum.\n"ANSI_RESET);
            }
        } else {
            while (c = getchar(), c != '\n' && c != EOF);
            printf(ANSI_RED"HIBA: Hibás formátum.\n"ANSI_RESET);
        }
    } while (!valid);
    switch (option) {
        case 1:
            exit(0);
        case 2:
            clear_terminal();
            menu1();
            break;
        case 3:
            exit(0);
    }
}