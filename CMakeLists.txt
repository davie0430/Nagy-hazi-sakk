cmake_minimum_required(VERSION 3.26)
project(chessmod C)
add_compile_options(-Wall)
add_compile_options(-Werror)
add_compile_options(-Wno-unused-results)

set(CMAKE_C_STANDARD 11)

add_executable(chessmod main.c
        piece.h
        pseudolegal.c
        pseudolegal.h
        boardprt.h
        boardprt.c
        fen_code_to_chessboard_converter.h
        fen_code_to_chessboard_converter.c
        menu.c
        menu.h
        castling.c
        castling.h
        chessboardtofen.c
        chessboardtofen.h
        fen_linked_list.c
        fen_linked_list.h
        save_load.c
        save_load.h
)
