//
//  BoardGeometry.h
//  ChessEngine
//
//  Created by Tony on 23/05/2024.
//

#ifndef BoardGeometry_h
#define BoardGeometry_h

#include <stdlib.h>

enum {
    a1, b1, c1, d1, e1, f1, g1, h1,
    a2, b2, c2, d2, e2, f2, g2, h2,
    a3, b3, c3, d3, e3, f3, g3, h3,
    a4, b4, c4, d4, e4, f4, g4, h4,
    a5, b5, c5, d5, e5, f5, g5, h5,
    a6, b6, c6, d6, e6, f6, g6, h6,
    a7, b7, c7, d7, e7, f7, g7, h7,
    a8, b8, c8, d8, e8, f8, g8, h8,
    NoSquare
};

typedef struct {
    int count;
    int squares[8];
} SquareList;

typedef int (*Direction)(int);

inline int rank(int square) {
    return square / 8;
}

inline int file(int square) {
    return square % 8;;
}

inline int distance(int s1, int s2) {
    int dr = abs(rank(s1) - rank(s2));
    int df = abs(file(s1) - file(s2));
    return dr > df ? dr : df;
}

inline int north(int square) {
    return square >= a8 ? NoSquare : square + 8;
}

inline int south(int square) {
    return square <= h1 ? NoSquare : square - 8;
}

inline int east(int square) {
    return file(square) == 7 ? NoSquare : square + 1;
}

inline int west(int square) {
    return file(square) == 0 ? NoSquare : square - 1;
}

inline int ne(int square) {
    return square >= a8 || file(square) == 7 ? NoSquare : square + 9;
}

inline int se(int square) {
    return square <= h1 || file(square) == 7 ? NoSquare : square - 7;
}

inline int nw(int square) {
    return square >= a8 || file(square) == 0 ? NoSquare : square + 7;
}

inline int sw(int square) {
    return square <= h1 || file(square) == 0 ? NoSquare : square - 9;
}

SquareList knightMovesFrom(int square);

SquareList kingMovesFrom(int square);

#endif /* BoardGeometry_h */
