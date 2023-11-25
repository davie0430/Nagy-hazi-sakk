#include <stdbool.h>
#include "piece.h"
#include "pseudolegal.h"

bool is_king_in_check(piece board[8][8]) {
    bool check = false;
    for (int row = 0;row < 8 && !check; row++) {
        for (int col = 0; col < 8 && !check; col++) {
            if (board[row][col].legality == legal && board[row][col].type == king) {
                check = true;
                return check;
            }
        }
    }
    return check;
}

void castling_help(piece board[8][8],moving m,fen_data *data) { //ha ellepett a bastya vagy a kiraly, nem lehet sancolni
    if (board[m.from_row][m.from_col].type == rook) { //ha a bastya lep el arra az oldalra nem lehet sancolni
        if (board[m.from_row][m.from_col].color == white) {
            if (board[m.from_row][m.from_col].side == kingside) {
                data->w_k_c = false;
            }
        }
        if (board[m.from_row][m.from_col].color == black) {
            if (board[m.from_row][m.from_col].side == kingside) {
                data->b_k_c = false;
            }
        }
        if (board[m.from_row][m.from_col].color == white) {
            if (board[m.from_row][m.from_col].side == queenside) {
                data->w_q_c = false;
            }
        }
        if (board[m.from_row][m.from_col].color == black) {
            if (board[m.from_row][m.from_col].side == queenside) {
                data->b_q_c = false;
            }
        }
    }
    if (board[m.from_row][m.from_col].type == king) { //ha kiraly lep, az az oldal nem sancolhat tobbe
        if (board[m.from_row][m.from_col].color == white) {
            data->w_k_c = false;
            data->w_q_c = false;
        }
        if (board[m.from_row][m.from_col].color == black) {
            data->b_k_c = false;
            data->b_q_c = false;
        }
    }
}

bool castling_help_attacks(piece board[8][8],bool direction,moving m, fen_data data) {
    bool check = false;
    int got_row, got_col;
    got_row = m.from_row;
    got_col = m.from_col;
    int x = 0;
    if (direction) {
        //lepjen a kiraly jobbra egyet, majd csekkoljuk az osszes ellenseges babu tamadasat. ha nincs sakk, lepjen megegyet jobbra
        while (x < 3) {

            board[got_row][got_col + x] = board[got_row][got_col];

            for (int row = 0;row < 8 && !check; row++) {
                for (int col = 0; col < 8 && !check; col++) {
                    if (board[row][col].color == data.color && board[row][col].color != null) {
                        m.from_row = row; m.from_col = col;
                        //Innentol ovatosnak kell lenni, mas lesz a from_row, from_col, mint a bekert.
                        pseudo_legal_moves(&m,board);
                        if (is_king_in_check(board)) {
                            check = true;
                        }
                    }
                }
            }
            x++;
        }
    } if (!direction) {
        while (x < 4) {

            board[got_row][got_col - x] = board[got_row][got_col];

            for (int row = 0;row < 8 && !check; row++) {
                for (int col = 0; col < 8 && !check; col++) {
                    if (board[row][col].color == data.color && board[row][col].color != null) {
                        m.from_row = row; m.from_col = col;
                        //Innentol ovatosnak kell lenni, mas lesz a from_row, from_col, mint a bekert.
                        pseudo_legal_moves(&m,board);
                        if (is_king_in_check(board)) {
                            check = true;
                        }
                    }
                }
            }
            x++;
        }
    }
    if (check) {
        return true;
    } else {
        return false;
    }
}

bool castling(moving *m, fen_data *data, piece board[8][8]) {
    bool direction; //0 balra,1 jobbra
    //kiraly-e a sancolni kivano babu?
    if (board[m->from_row][m->from_col].type == king) {
        //jo helyrol indul-e a vilagos kiraly:
        if (m->from_row == 7 && m->from_col == 4) {
            if (m->to_row == 7 && m->to_col == 6) {
                if (data->w_k_c == true) {
                    //most kene megnezni hogy uresek-e az odavezeto mezok
                    if (board[7][5].type == empty && board[7][6].type == empty) {
                        //uresek a mezok, sancolni szeretnenk, bastya/kiraly nem mozdult eddig, viszont nemtudjuk mi van tamadva es mi nincs ut kozben. csekkoljuk:
                        direction = 1; //jobbra
                        if (!castling_help_attacks(board, direction, *m, *data)) {
                            return true;
                        }
                    }
                }
            } else if (m->to_row == 7 && m->to_col == 2) {
                if (data->w_q_c == true) {
                    //most kene megnezni hogy uresek-e az odavezeto mezok
                    if (board[7][3].type == empty && board[7][2].type == empty && board[7][1].type == empty) {
                        direction = 0; //balra
                        if (!castling_help_attacks(board,direction, *m, *data)) {
                            return true;
                        }
                    }
                }
            }
            //ha sotet szeretne sancolni:
        } else if (m->from_row == 0 && m->from_col == 4) {
            if (m->to_row == 0 && m->to_col == 6) {
                if (data->b_k_c == true) {
                    //most kene megnezni hogy uresek-e az odavezeto mezok
                    if (board[0][5].type == empty && board[0][6].type == empty) {
                        direction = 1; //jobbra
                        if (!castling_help_attacks(board, direction, *m, *data)) {
                            return true;
                        }
                    }
                }
            } else if (m->to_row == 0 && m->to_col == 2) {
                if (data->b_q_c == true) {
                    //most kene megnezni hogy uresek-e az odavezeto mezok
                    if (board[0][1].type == empty && board[0][2].type == empty && board[0][3].type == empty) {
                        direction = 0; //balra
                        if (!castling_help_attacks(board,direction, *m, *data)) {
                            return true;
                        }
                    }
                }
            }
        }
    }
    return false;
}

void castling_action(piece board[8][8],piece temp_board[8][8], moving m) {
    for (int i = 0;i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            board[i][j] = temp_board[i][j];
        }
    }
    board[m.to_row][m.to_col] = board[m.from_row][m.from_col];
    board[m.from_row][m.from_col].type = empty;
    board[m.from_row][m.from_col].color = null;
    if (m.to_col == 2) {
        board[m.to_row][m.to_col + 1] = board[m.to_row][0];
        board[m.to_row][0].type = empty;
        board[m.to_row][0].color = null;
    } else {
        board[m.to_row][m.to_col - 1] = board[m.to_row][7];
        board[m.to_row][7].type = empty;
        board[m.to_row][7].color = null;
    }
}
