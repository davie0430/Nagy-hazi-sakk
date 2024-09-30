#include "fen_code_to_chessboard_converter.h"
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <stddef.h>
void castling_rights(char token_char, fen_data *data) {
    if (token_char == 'K') {
        data->w_k_c = true;
    }
    if (token_char == 'k') {
        data->b_k_c = true;
    }
    if (token_char == 'Q') {
        data->w_q_c = true;
    }
    if (token_char == 'q') {
        data->b_q_c = true;
    }
}

void fen_to_chessboard(char fen[100],piece board[8][8],fen_data *data,bool first) {
    char temp_fen[100];
    strcpy(temp_fen, fen);
    if (first) {
        data->w_k_c = false;
        data->b_k_c = false,
        data->w_q_c = false;
        data->b_q_c = false;
        data->first_enp_row = 0;
        data->first_enp_col = 0;
        for (int row = 0; row < 8; row++) { //kinullazas
            for (int col = 0; col < 8; col++) {
                board[row][col].color = null;
                board[row][col].type = empty;

                board[row][col].legality = illegal; //default allapot lepesek elott
            }
        }
        first = false;
    }
    bool phase2 = false;
    char* token = (char*)(strtok(fen, "/")); //szetvalasztas
    int row = 0, col = 0, idx = 0, space_count = 0; bool collecting = true;
    while (token != NULL) { //amig a string vegere nem er

        while (token[idx] != '\0') { //stringen belul a /-nel szetvalasztottakat nezi kulon kulon \0-ig
            if (token[idx] == ' ') {
                space_count++;
                phase2 = true; //mar beolvastuk az osszes babut a tablan, kovi a szin es a sanc
            }
            if (phase2) {
                if ((token[idx]) == '-') {
                    collecting = false;
                }
                if (space_count > 2) {
                    if (token[idx] >= 'a' && token[idx] <= 'h' && collecting) {
                        data->first_enp_col = token[idx] - 'a';

                    }
                    if (isdigit(token[idx]) && collecting) {
                        data->first_enp_row = 7 - token[idx] + '1';
                        collecting = false;
                    }
                    for (int rows = 0; rows < 8; rows++) {
                        for (int cols = 0; cols < 8; cols++) {
                            board[rows][cols].en_passant = false;
                            board[rows][cols].en_passant_warning = false;
                        }
                    }
                    if (collecting) {
                        board[data->first_enp_row][data->first_enp_col].en_passant = true;
                    }
                }

                castling_rights(token[idx],data);
                if (token[idx] == 'w' || token[idx] == 'b') {
                    switch(token[idx]) {
                        case 'w':
                            data->color = false;
                            break;
                        case 'b':
                            data->color = true; //pont az ellentet kell ideirni, mert a loopban elso dolog az lesz hogy megforditja jora
                            break;
                    }
                }

            }
            if (!phase2) {
                if (token[idx] >= '1' && token[idx] <= '8') { //ha talal szamot mashogy kell kezelni (lasd: FEN wikipedia)
                    int col_as_num = token[idx] - '0';
                    for (int i = 0; i < col_as_num; i++) {
                        board[row][col].type = empty;
                        col++;
                    }
                } else {
                    //ha nem szam
                    piece_color color = (token[idx] >= 'A' && token[idx] <= 'Z') ? white : black; //ha nagybetuk vilagos, ha nem, sotet szin
                    piece_type type = 0;
                    switch (tolower(token[idx])) {
                        case 'p': type = pawn; break;
                        case 'n': type = knight; break;
                        case 'b': type = bishop; break;
                        case 'r': type = rook; break;
                        case 'q': type = queen; break;
                        case 'k': type = king; break;
                        default: break;
                    }
                    board[row][col].color = color; //vegul betaplaljuk a tombbe a dolgokat
                    board[row][col].type = type;
                    col++;
                }
            }
            idx++;
        }
        token = strtok(NULL, "/"); //johet a kovetkezo token
        row++; //ezert row is uj
        col = 0;
        idx = 0;
    }
    strcpy(fen,temp_fen);
}