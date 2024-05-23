//
//  BoardGeometry.c
//  ChessEngine
//
//  Created by Tony on 23/05/2024.
//

#include "BoardGeometry.h"

extern int north(int);
extern int south(int);
extern int east(int);
extern int west(int);
extern int ne(int);
extern int se(int);
extern int nw(int);
extern int sw(int);
extern int file(int);
extern int rank(int);
extern int distance(int, int);

static inline void tryAdd(SquareList *list, int square) {
    if (square != NoSquare) list->squares[list->count++] = square;
}

SquareList knightMovesFrom(int square) {
    SquareList result = {0};
    int target;
    if ((target = north(square)) != NoSquare) tryAdd(&result, ne(target));
    if ((target = east(square)) != NoSquare) tryAdd(&result, ne(target));
    if ((target = east(square)) != NoSquare) tryAdd(&result, se(target));
    if ((target = south(square)) != NoSquare) tryAdd(&result, se(target));
    if ((target = south(square)) != NoSquare) tryAdd(&result, sw(target));
    if ((target = west(square)) != NoSquare) tryAdd(&result, sw(target));
    if ((target = west(square)) != NoSquare) tryAdd(&result, nw(target));
    if ((target = north(square)) != NoSquare) tryAdd(&result, nw(target));
    return result;
}

SquareList kingMovesFrom(int square) {
    SquareList result = {0};
    tryAdd(&result, north(square));
    tryAdd(&result, south(square));
    tryAdd(&result, east(square));
    tryAdd(&result, west(square));
    tryAdd(&result, ne(square));
    tryAdd(&result, se(square));
    tryAdd(&result, nw(square));
    tryAdd(&result, sw(square));
    return result;
}
