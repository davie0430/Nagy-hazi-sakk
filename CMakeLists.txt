cmake_minimum_required(VERSION 3.26)
project(chessmod C)

set(CMAKE_C_STANDARD 11)

add_executable(chessmod main.c
        piece.h
        pseudolegal.c
        pseudolegal.h
        boardprt.h
        boardprt.c
        fentocb.h
        fentocb.c
        menu.c
        menu.h
        castling.c
        castling.h
)

set(GCC_ENABLE_WARNINGS "-O1 -Wall -Werror -Wno-unused-result")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${GCC_ENABLE_WARNINGS}")

