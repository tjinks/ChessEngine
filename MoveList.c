//
//  MoveList.c
//  ChessEngine
//
//  Created by Tony on 23/05/2024.
//

#include <stdlib.h>
#include <string.h>
#include "MoveList.h"
#include "EngCommon.h"
#include "GameState.h"

static const size_t MovesOffset =  offsetof(MoveList, moves);
static const size_t MoveSize = sizeof(Move);
static const size_t InitialMoveListSize = 16;

static MoveList *mlFreeList = NULL;

static MoveList *createMoveList(void) {
    size_t required = MovesOffset + InitialMoveListSize * MoveSize;
    MoveList *result = getMem(required);
    result->capacity = InitialMoveListSize;
    result->size = 0;
    result->next = NULL;
    return result;
}

static MoveList *expand(MoveList *moveList) {
    int newCapacity = 2 * moveList->capacity;
    MoveList *result = getMem(MovesOffset + newCapacity * MoveSize);
    memcpy(result, moveList, MovesOffset + moveList->size * MoveSize);
    freeMem(moveList);
    result->capacity = newCapacity;
    return result;
}

MoveList *acquireMoveList(void) {
    if (mlFreeList) {
        MoveList *result = mlFreeList;
        mlFreeList = mlFreeList->next;
        result->size = 0;
        result->next = NULL;
        return result;
    } else {
        return createMoveList();
    }
}

MoveList *addMove(MoveList *moveList, Move move) {
    MoveList *result = moveList->capacity == moveList->size ? expand(moveList) : moveList;
    result->moves[result->size++] = move;
    return result;
}

static int sortFunc(const void *m1, const void *m2) {
    const Move *move1 = m1;
    const Move *move2 = m2;
    if (move1->score < move2->score) return 1;
    if (move1->score > move2->score) return -1;
    return 0;
}

void sortMoveList(MoveList *moveList) {
    qsort(moveList->moves, moveList->size, sizeof(Move), sortFunc);
}

bool containsTargets(const MoveList *moveList, SquareMask targets) {
    static const SquareMask One = 1;
    for (int i = 0; i < moveList->size; i++) {
        Move move = moveList->moves[i];
        int target = move.atoms[1].square;
        if ((One << target) & targets) {
            return true;
        }
    }
    
    return false;
}

void releaseMoveList(const MoveList *moveList) {
    MoveList *ml = (MoveList *)moveList;
    ml->next = mlFreeList;
    mlFreeList = ml;
}

MoveList *duplicateMoveList(const MoveList *moveList) {
    MoveList *result = acquireMoveList();
    for (int i = 0; i < moveList->size; i++) {
        addMove(result, moveList->moves[i]);
    }
    
    return result;
}

MoveList *filterMoveList(const MoveList *moveList, bool (*filterFunc)(Move, void *), void *filterData) {
    MoveList *result = acquireMoveList();
    for (int i = 0; i < moveList->size; i++) {
        if (filterFunc(moveList->moves[i], filterData)) {
            result = addMove(result, moveList->moves[i]);
        }
    }
            
    return result;
}


