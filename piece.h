
#ifndef PIECE_H
#define PIECE_H

#include <stdbool.h>

//ha windows platform
#ifdef _WIN32
#include <windows.h>
#endif

//szinkodok
#define ANSI_WHITE "\x1b[30;107m"
#define ANSI_BLACK "\x1b[30;100m"
#define ANSI_YELLOW "\e[0;33m"
#define ANSI_RED "\e[0;31m"
#define ANSI_RESET "\33[m"

typedef  enum {queenside, kingside} board_side;

typedef  enum {white_side, black_side} board_territory;

typedef enum {white, black, null} piece_color;

typedef enum {empty, pawn, knight, bishop, rook, queen, king} piece_type;

typedef enum {illegal, legal} is_legal;

typedef struct {
    piece_color color;
    piece_type type;
    is_legal legality; //lepesek elott ezt csekkoljuk, igy nem kell kulon dinamikus tomb a legalis lepesek szamolasara. lepes utan vissza mindegyik illegalra.
    board_side side;
    board_territory territory;
    bool en_passant;
    bool en_passant_warning;
} piece;

//fen altal beolvasott dolgok itt tarolodnak el:
typedef struct fen {
    bool color;
    bool w_k_c; //vilagos kiraly oldali sanc
    bool w_q_c; //vilagos vezer oldali sanc
    bool b_k_c; //sotet kiraly oldali sanc
    bool b_q_c; //sotet vezer oldali sanc
    int first_enp_row;
    int first_enp_col;
} fen_data;

typedef struct moving {
    int from_row;
    int from_col;
    int to_row;
    int to_col;
} moving;

typedef struct temp_moving {
    int temp_from_row;
    int temp_from_col;
    int temp_to_row;
    int temp_to_col;
} temp_moving;


#endif //PIECE_H
