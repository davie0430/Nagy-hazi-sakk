#include "piece.h"

void chessboard_to_fen(piece board[8][8],char fen_from_cb[100]) {
    int index = 0;
    int empty_count = 0;
    for (int row = 0;row < 8;row++) {
        for (int col = 0;col < 8;col++) {
            if (board[row][col].type == empty) {
                empty_count++;
            } else {
                if (empty_count > 0) {
                    fen_from_cb[index] = (char)('0' + empty_count);
                    index++;
                    empty_count = 0;
                }
                switch (board[row][col].type) {
                    case pawn: fen_from_cb[index] = (board[row][col].color == white) ? 'P' : 'p';
                        break;
                    case knight: fen_from_cb[index] = (board[row][col].color == white) ? 'N' : 'n';
                        break;
                    case bishop: fen_from_cb[index] = (board[row][col].color == white) ? 'B' : 'b';
                        break;
                    case rook: fen_from_cb[index] = (board[row][col].color == white) ? 'R' : 'r';
                        break;
                    case queen: fen_from_cb[index] = (board[row][col].color == white) ? 'Q' : 'q';
                        break;
                    case king: fen_from_cb[index] = (board[row][col].color == white) ? 'K' : 'k';
                        break;
                        default: break;
                }
                index++;
            }
        }
        if (empty_count > 0) {
            fen_from_cb[index] = (char)('0' + empty_count);
            index++;
            empty_count = 0;
        }
        if (row != 7) {
            fen_from_cb[index] = '/';
            index++;
        }
    }
}

