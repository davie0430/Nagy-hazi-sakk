#include "pseudolegal.h"
#include "piece.h"


bool psl_pawn_blockade(int from_row, int from_col, int x, piece board[8][8]) {
    int new_row = from_row - x; //mivel a tablan ha +1et mesz, az valjoban -1et jelent, meg kell fordiatni az x-t. tehat ha direciton -1, akkor +1-e valtozik itt.

    if (board[new_row][from_col].type == empty) {
        board[new_row][from_col].legality = legal;
        return true;
    } else {
        return false;
    }
}

void psl_pawn(piece board[8][8], int from_row, int from_col, int to_row, int to_col) { //to_row csak en passant miatt
    int direction = (board[from_row][from_col].color == white) ? 1 : -1; //ha feher, a direction 1, else a direction -1

    if ((from_row == 6 && board[from_row][from_col].color == white) || (from_row == 1 && board[from_row][from_col].color == black)) {
        if (psl_pawn_blockade(from_row, from_col, direction, board)) {
            if (psl_pawn_blockade(from_row, from_col, 2 * direction, board)) {
                if (to_row == from_row - 2*direction) {
                    board[from_row - direction][from_col].en_passant = true;
                }
            }
        }
    }
    if (from_row != 0 && from_row != 7) {
        psl_pawn_blockade(from_row, from_col, direction, board);
        //tabla szelen ne legyen tulindexeles
        //atlos dolgok:
        if (from_col < 7 && from_col > 0) {
            if (board[from_row - direction][from_col + 1].type != empty) {
                board[from_row - direction][from_col + 1].legality = legal;
            }
            if (board[from_row - direction][from_col - 1].type != empty) {
                board[from_row - direction][from_col - 1].legality = legal;
            }
            //ha van en passant
            if (board[from_row - direction][from_col + 1].en_passant == true) {
                if (board[to_row - direction][to_col].color != board[from_row][from_col].color) {
                    board[from_row - direction][from_col + 1].legality = legal;
                }
            }
            if (board[from_row - direction][from_col - 1].en_passant == true) {
                if (board[to_row - direction][to_col].color != board[from_row][from_col].color) {
                    board[from_row - direction][from_col - 1].legality = legal;
                }
            }
        } else {
            //ha a tabla szelen van a gyalog:
            if (from_col == 0) {
                if (board[from_row - direction][from_col + 1].type != empty) {
                    board[from_row - direction][from_col + 1].legality = legal;
                }
            } else {
                if (board[from_row - direction][from_col - 1].type != empty) {
                    board[from_row - direction][from_col - 1].legality = legal;
                }
            }
        }
    }
}

void psl_knight(piece board[8][8], int from_row, int from_col) { //itt nem szedem kulon szín szerint mivel itt eleg csak iffel megnezni hogy melyik szint utheti le
    int knight_moves[8][2] = {
            {-2, -1}, {-2, 1},
            {2, -1}, {2, 1},
            {1, 2}, {-1, 2},
            {1, -2}, {-1, -2}};
    int new_row;
    int new_col;

    for (int i = 0; i < 8; i++) {
        new_row = from_row + knight_moves[i][0];
        new_col = from_col + knight_moves[i][1];
        if (new_row >= 0 && new_row < 8 && new_col >= 0 && new_col < 8) {
            board[new_row][new_col].legality = board[new_row][new_col].color != board[from_row][from_col].color || board[new_row][new_col].type == empty;
        }

    }
}
//innen horizontalis es veritkalis, illetve atlos mozgas kulon vettem, es utanna ezeket hivom ha kell.
void psl_diagonal(piece board[8][8], int from_row, int from_col) {
    int directions[4][2] = {{1, 1}, {1, -1}, {-1, 1}, {-1, -1}};

    for (int i = 0; i < 4; i++) {
        int row = from_row + directions[i][0];
        int col = from_col + directions[i][1];

        while (row >= 0 && row < 8 && col >= 0 && col < 8) {
            if (board[row][col].type == empty) {
                board[row][col].legality = legal;
            } else if (board[from_row][from_col].color != board[row][col].color) {
                // ha a celmezoben egy ellenseges babu all, azt leutheted
                board[row][col].legality = legal;
                break; // leutottel egy ellenseges babut, ne keressen tovabb szabad mezoket
            } else {
                break; // nem lephetsz ra a sajat babudra
            }

            row += directions[i][0];
            col += directions[i][1];
        }
    }
}

void psl_horizontal_vertical(piece board[8][8], int from_row, int from_col) {
    int directions[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};

    for (int i = 0; i < 4; i++) {
        int row = from_row + directions[i][0];
        int col = from_col + directions[i][1];

        while (row >= 0 && row < 8 && col >= 0 && col < 8) {
            if (board[row][col].type == empty) {
                board[row][col].legality = legal;
            } else if (board[from_row][from_col].color != board[row][col].color) {
                board[row][col].legality = legal;
                break;
            } else {
                break;
            }
            row += directions[i][0];
            col += directions[i][1];
        }
    }
}

void psl_bishop(piece board[8][8], int from_row, int from_col) {
    psl_diagonal(board, from_row, from_col);
}

void psl_rook(piece board[8][8], int from_row, int from_col) {
    psl_horizontal_vertical(board, from_row, from_col);
}

void psl_queen(piece board[8][8], int from_row, int from_col) {
    psl_diagonal(board, from_row, from_col);
    psl_horizontal_vertical(board, from_row, from_col);
}

void psl_king(piece board[8][8], int from_row, int from_col) {
    int directions[8][2] = {
            {1, 0}, {-1, 0}, {0, 1}, {0, -1},
            {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};

    for (int i = 0; i < 8; i++) {
        int row = from_row + directions[i][0];
        int col = from_col + directions[i][1];

        if (row >= 0 && row < 8 && col >= 0 && col < 8) {
            if (board[row][col].type == empty || board[row][col].color != board[from_row][from_col].color) {

                board[row][col].legality = legal;
            }
        }
    }
}

void legality_reset(piece board[8][8]) {
    for (int row = 0;row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            board[row][col].legality = illegal;
        }
    }
}

void pseudo_legal_moves(moving *m,piece board[8][8]) { //megnezem az adott babunak a legalis lepeseit, DE nem csekkolva hogy sakkban lenne-e valaki, van-e felfedett sakk, etc.
    //from[0] == row (sor), from[1] == col (oszlop)
    legality_reset(board);
    int from_row = m->from_row, from_col = m->from_col;
    int to_row = m->to_row; int to_col = m->to_col;
    switch (board[from_row][from_col].type) {
        case pawn:
            psl_pawn(board,from_row,from_col, to_row, to_col);
            break;
        case knight:
            psl_knight(board,from_row,from_col);
            break;
        case bishop:
            psl_bishop(board,from_row,from_col);
            break;
        case rook:
            psl_rook(board,from_row,from_col);
            break;
        case queen:
            psl_queen(board,from_row,from_col);
            break;
        case king:
            psl_king(board,from_row,from_col);
            break;
        default:
            break;

    }
}

bool is_there_even_legal_move(piece board[8][8]) {
    //for loop hogy megnezzem melyek a legalis lepesek:
    bool is_there_legal = false;

    for (int row = 0; row < 8 && !is_there_legal; row++) {
        for (int col = 0; col < 8 && !is_there_legal; col++) {
            if (board[row][col].legality == legal) {
                is_there_legal = true;
            }
        }
    }
    return is_there_legal;
}
