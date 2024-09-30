#include "boardprt.h"
#include "piece.h"
#include <stdio.h>
#include <stdlib.h>
void print_piece(int color, piece_type type) {

    switch (type) {
        case empty:
            printf("  ");
            break;
        case pawn:
            printf(color == white ? "\u2659 " : "\u265F ");
            break;
        case knight:
            printf(color == white ? "\u2658 " : "\u265e ");
            break;
        case bishop:
            printf(color == white ? "\u2657 " : "\u265d ");
            break;
        case rook:
            printf(color == white ? "\u2656 " : "\u265c ");
            break;
        case queen:
            printf(color == white ? "\u2655 " : "\u265b ");
            break;
        case king:
            printf(color == white ? "\u2654 " : "\u265a ");
            break;
    }
    printf("%s",ANSI_RESET);
}

void clear_terminal() {
#ifdef _WIN32
    system("cls");  // For Windows
#else
    system("clear");  // For Linux/macOS
#endif
}

void chessboard_print(piece chessboard[8][8]) {
    clear_terminal();
    int notation = 8;
    for (int i = 8;i > 0;i--) {
        printf("%c ",notation + 'a' - i);
    }
    printf("\n");
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            chessboard[row][col].legality = illegal;
            if (col > 3) { //sanc miatt kell
                chessboard[row][col].side = queenside;
            } else {
                chessboard[row][col].side = kingside;
            }
            if (row > 3) { //en passant miatt kell
                chessboard[row][col].territory = white_side;
            } else {
                chessboard[row][col].territory = black_side;
            }
            if ((row + col) % 2 == 0) {
                printf("%s",ANSI_WHITE);
            } else {
                printf("%s",ANSI_BLACK);
            }

            print_piece(chessboard[row][col].color, chessboard[row][col].type);

            if (col == 7) {
                printf("%s%d ",ANSI_RESET,notation);
                notation--;
            }

        }
        printf("\n");
    }
}
