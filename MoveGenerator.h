//
//  EngMoveGenerator.h
//  ChessBE
//
//  Created by Tony on 19/05/2024.
//

#ifndef EngMoveGenerator_h
#define EngMoveGenerator_h

#include "Move.h"
#include "GameState.h"
#include "MoveList.h"
#include "BoardGeometry.h"

typedef struct {
    const Position *position;
    MoveList *moveList;
    Player player;
    Direction pawnMoveDirection;
    Direction pawnCaptureDirections[2];
    int secondRank;
    int seventhRank;
} MoveGenerator;

void initMoveGenerator(MoveGenerator *generator, const Position *position, Player player);

void addNonCastlingMoves(MoveGenerator *);

void addCastlingMoves(MoveGenerator *, const MoveList *opponentMoves);


#endif /* EngMoveGenerator_h */
