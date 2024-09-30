#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "piece.h"
#include "pseudolegal.h"
#include "boardprt.h"
#include "fen_code_to_chessboard_converter.h"
#include "menu.h"
#include "castling.h"
#include "fen_linked_list.h"
#include "chessboardtofen.h"
#include "save_load.h"

int input_validity(piece board[8][8],const char *input,moving *m, fen_node *fen_list) {
    if (strcmp("help",input) == 0) {
        printf("\nFormátumra példa: \"e2e4\", ahol \"e2\" jelöli a mezőt ahonnan lépsz,\n\"e4\" pedig azt a mezőt ahová lépsz. Ha sáncolni szeretnél, az egy\nkirály lépés ugyan így. pl.: \"e1g1\". Kilépéshez bármikor írd: \"exit\"\nVisszajátszáshoz: \"nezet\"\n");
        return 0;
    }
    if (strcmp("nezet",input) == 0) {
        nezet(fen_list);
        chessboard_print(board);
    }

    if (strcmp("exit",input) == 0) {
        if (ending_menu(-1,"döntetlen",fen_list)) {
            return 2;
        }
    }
    if (strlen(input) == 4) {
        m->from_col = input[0] - 'a';
        m->from_row = 7 - input[1] + '1';
        m->to_col = input[2] - 'a';
        m->to_row = 7 - input[3] + '1';
        if (m->from_row > 7 || m->from_row < 0 || m->from_col > 7 || m->from_col < 0 || m->to_row > 7 || m->to_row < 0 || m->to_col > 7 || m->to_col < 0 ) {
            printf("\nHIBA: Hibás formátum.\n");
            return 0;
        } else {
            return 1;
        }
    } else {
        printf("\nHIBA: Hibás formátum.\n");
        return 0;
    }
}

bool is_right_color(piece board[8][8],moving *m,bool color) {
    if (color) {
        if (board[m->from_row][m->from_col].color == white) {
            return true;
        } else {
            return false;
        }
    } else {
        if (board[m->from_row][m->from_col].color == black) {
            return true;
        } else {
            return false;
        }
    }
}

void default_position(moving *m, temp_moving *t, piece board[8][8], piece temp_board[8][8]) {
    //VISSZAALLITJUK AZ EREDETI ALLAST:
    m->from_row = t->temp_from_row;
    m->from_col = t->temp_from_col;
    m->to_row = t->temp_to_row;
    m->to_col = t->temp_to_col;

    //VISSZAALLITJUK A TABLA LEGALITASAT:
    for (int i = 0;i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            board[i][j] = temp_board[i][j];
        }
    }
}

int moving_aciton(piece board[8][8], piece temp_board[8][8], moving *m, temp_moving *t, bool *check, bool *them_checked,fen_data *data,int *fifty_move) {
    //Megezzuk az osszes pseudo legalis lepeset a babunak
    pseudo_legal_moves(m,board);

    if (is_there_even_legal_move(board)) {
        //LEMASOLJUK A MOSTANI FROM,TO ROW-T:
        t->temp_from_row = m->from_row;
        t->temp_from_col = m->from_col;
        t->temp_to_row = m->to_row;
        t->temp_to_col = m->to_col;

        //IDEIGLENESEN ODALEPUNK:
        //IF ENPASSANT LEUTES
        if (board[m->from_row][m->from_col].type == pawn && board[m->to_row][m->to_col].en_passant == true) {
            if (data->color == black) {
                board[m->to_row + 1][m->to_col].type = empty;
                board[m->to_row + 1][m->to_col].color = null;
            } else {
                board[m->to_row - 1][m->to_col].type = empty;
                board[m->to_row - 1][m->to_col].color = null;
            }
        }

        board[m->to_row][m->to_col] = board[m->from_row][m->from_col];
        board[m->from_row][m->from_col].type = empty;
        board[m->from_row][m->from_col].color = null;


        //Most megnezzuk hogy ha elleptunk onnan, sakkban lennenk-e a kovetkezo lepesben. Ha igen, akkor az felfedett sakk, ami illegalis.
        *check = false;
        *them_checked = false;
        for (int row = 0;row < 8 && !*check; row++) {
            for (int col = 0; col < 8 && !*check; col++) {
                if (board[row][col].color == data->color) {
                    m->from_row = row; m->from_col = col;
                    //Innentol ovatosnak kell lenni, mas lesz a from_row, from_col, mint a bekert.
                    pseudo_legal_moves(&*m,board);
                    if (is_king_in_check(board)) {
                        *check = true;
                    }
                } else if (board[row][col].color != data->color && board[row][col].color != null) {
                    m->from_row = row; m->from_col = col;
                    pseudo_legal_moves(&*m,board);
                    if (!*them_checked && is_king_in_check(board)) {
                        *them_checked = true;
                    }
                }
            }
        }
        if (!*check) {
            //VISSZAALLITJUK AZ EREDETI ALLAST:
            default_position(m,t,board,temp_board);
            pseudo_legal_moves(m,board);

            if (board[m->to_row][m->to_col].legality == legal) {
                //en passantnal el kell tunteni a fentmarado ellenekzo szinu babut:
                if (board[m->from_row][m->from_col].type == pawn && board[m->to_row][m->to_col].en_passant == true) {
                    if (data->color == black) {
                        board[m->to_row + 1][m->to_col].type = empty;
                        board[m->to_row + 1][m->to_col].color = null;
                    } else {
                        board[m->to_row - 1][m->to_col].type = empty;
                        board[m->to_row - 1][m->to_col].color = null;
                    }
                }
                castling_help(board,*m,data); //ha kiraly, vagy bastya lepett, akkor nem (vagy limitaltan) lehet sancolni onnantol
                //promóció:
                if (board[m->from_row][m->from_col].type == pawn) {
                    if ((m->from_row == 1 && board[m->from_row][m->from_col].color == white) || (m->from_row == 6 && board[m->from_row][m->from_col].color == black)) {
                        promotion_menu(board,m,data);
                    }
                }
                if (board[m->to_row][m->from_col].color != null) {
                    *fifty_move = 0;
                }
                board[m->to_row][m->to_col] = board[m->from_row][m->from_col];
                board[m->from_row][m->from_col].type = empty;
                board[m->from_row][m->from_col].color = null;

                *check = false;

                for (int row = 0;row < 8;row++) { //ha van enpassant, egy korig meg otthagyjuk, hogy leuthesse a masik.
                    for (int col = 0;col < 8;col++) {
                        if (board[row][col].en_passant_warning) {
                            board[row][col].en_passant = false;
                        }
                        if (board[row][col].en_passant == true) {
                            board[row][col].en_passant_warning = true;
                        }

                    }
                }
                return 0;

            } else {
                printf("\nHIBA: Illegális lépés.\n");
            }
        } else {
            //VISSZAALLITJUK AZ EREDETI ALLAST:
            default_position(m,t,board,temp_board);
            printf("\nHIBA: Illegális lépés, (Sakkban lenne a királyod)\n");
        }
    } else { printf("\nHIBA: Ezzel a bábuval nincs legális lépésed.\n"); }
    return -1;
}

bool checkmate(piece board[8][8],moving *m,fen_data *data) {
    for (int i = 0;i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (board[i][j].type == king && board[i][j].color != data->color) {
                printf("%d %d",i,j);
                m->from_row = i; m->from_col = j;
                pseudo_legal_moves(m,board);
                if (!is_there_even_legal_move(board)) {
                    return true;
                }
            }
        }
    }
    return false;
}

bool game(piece board[8][8], fen_data *data,fen_node *fen_list) {
    char fen_from_cb[100] = {0};
    char *player;
    char input[7];
    bool playing = true;
    bool check;
    bool them_checked = false;
    int fifty_move_rule_count = 0;
    moving m;
    temp_moving t;
    piece temp_board[8][8];
    while (playing) {
        chessboard_print(board);
        chessboard_to_fen(board,fen_from_cb);
        fenlist_append(&fen_list,fen_from_cb);
        if (data->color) {
            player = "Sötét";
            data->color = false;
        }
        else if (!data->color) {
            player = "Világos";
            data->color = true;
        }

        while (1) {
            for (int i = 0;i < 8; i++) {
                for (int j = 0; j < 8; j++) {
                    temp_board[i][j] = board[i][j];
                }
            }
            if (them_checked == true) {
                //sakk van, nézzük meg, hogy matt-e
                if (checkmate(board,&m,data)) {
                    playing = false;
                    printf("\nSAKK MATT!\n");
                    if (data->color) {
                        int eredmeny = 1;
                        char gyoztes[] = "sötét";
                        if (ending_menu(eredmeny,gyoztes,fen_list)) {
                            return false;
                        }
                    } else {
                        int eredmeny = 0;
                        char gyoztes[] = "világos";
                        if (ending_menu(eredmeny,gyoztes,fen_list)) {
                            return false;
                        }
                    }
                } else {
                    printf("\nSAKK!\n");
                }
            }
            printf("\n%s lépése (Segítség: \"help\"): ", player);
            if (scanf("%6s", input) == 1) {
                int c;
                while ((c = getchar()) != '\n' && c != EOF) {}
                if (strlen(input) > 5) {
                    printf("\nHibás formátum.\n");

                }
                int inp = input_validity(board,input,&m,fen_list);
                if (inp == 1) {
                    if (is_right_color(board,&m,data->color)) {
                        //Helyes az input, most megnézzük, hogy sancolni akar-e
                        if (!them_checked && castling(&m, data, board)) { //ha nem vagy sakkban csak akkor lehet sancolni
                            castling_action(board,temp_board,m);
                            break;
                        } else {
                            if (moving_aciton(board,temp_board,&m,&t,&check,&them_checked,data,&fifty_move_rule_count) == 0) {
                                if (board[m.to_row][m.to_col].type == pawn) {
                                    fifty_move_rule_count = 0;
                                } else {
                                    fifty_move_rule_count++;
                                }
                                if (fifty_move_rule_count == 50) { //ha elertunk 50 lepest leutes, vagy gyaloglepes nelkul:
                                    char gyoztes[] = "döntetlen";
                                    playing = false;
                                    if (ending_menu(3,gyoztes,fen_list)) {
                                        return false;
                                    }
                                }
                                break;
                            }
                        }
                    } else { printf("\nHIBA: Illegális lépés, ez nem a te bábud.\n"); }
                } if (inp == 2) {
                    return true;
                }
            } else { printf("\nHIBA: Hibás formátum.\n"); }
        }
    }
    return true;
}

void start() {
    int playing = true;
    while (playing) {
        fen_node *fen_list = initialize_fenlist(); //lancolt lista az allasnak
        int choice = menu1();
        if (choice == 1) {
            fenlist_free(&fen_list); //ha esteleg mar volt elozo jatszma, szabaditsuk fel a memoriat
            char fen[100] = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
            piece chessboard[8][8];
            fen_data data; //ez az eppen kovetkezo szin ellentetes szine
            fen_to_chessboard(fen,chessboard,&data,true);
            if (!game(chessboard,&data,fen_list)) {
                playing = false;
            }

        } else if (choice == 2) {
            fenlist_free(&fen_list);
            printf("Kérlek adj meg egy valós FEN kódot: ");
            char fen[100] = {0};
            scanf(" %[^\n]s",fen);
            piece chessboard[8][8];
            fen_data data; //ez az eppen kovetkezo szin ellentetes szine
            fen_to_chessboard(fen,chessboard,&data,true);
            if (!game(chessboard,&data,fen_list)) {
                playing = false;
            }
        } else if (choice == 3) {
            fen_node *head = initialize_fenlist(); //lancolt lista az allasnak
            if (file_load(&head) == 0) {
                nezet(head);
                fenlist_free(&head);
            } else {
                clear_terminal();
            }
        } else if (choice == 4) {
            fenlist_free(&fen_list);
            exit(0);
        }
    }
}

int main(void) {
#ifdef _WIN32
    SetConsoleOutputCP(65001);
#endif
    start();
    return 0;
}