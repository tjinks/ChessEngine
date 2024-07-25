//
//  MoveList.h
//  ChessEngine
//
//  Created by Tony on 23/05/2024.
//

#ifndef MoveList_h
#define MoveList_h

#include "Move.h"
#include "EngCommon.h"

typedef struct MoveList {
    struct MoveList *next;
    int size, capacity;
    Move moves[1];
} MoveList;

MoveList *acquireMoveList(void);

void releaseMoveList(const MoveList *moveList);

MoveList *addMove(MoveList *moveList, Move move);

void sortMoveList(MoveList *moveList);

bool containsTargets(const MoveList *moveList, SquareMask targets);

MoveList *duplicateMoveList(const MoveList *moveList);

MoveList *filterMoveList(const MoveList *moveList, bool (*filterFunc)(Move, void *), void *filterData);


#endif /* MoveList_h */
