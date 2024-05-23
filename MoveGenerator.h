//
//  EngMoveGenerator.h
//  ChessBE
//
//  Created by Tony on 19/05/2024.
//

#ifndef EngMoveGenerator_h
#define EngMoveGenerator_h

#include "Move.h"

typedef struct MoveList {
    struct MoveList *next;
    int size, capacity;
    Move moves[1];
} MoveList;

#endif /* EngMoveGenerator_h */
