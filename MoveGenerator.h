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

MoveList *generateMoves(const Position *position, Player player);

void addNonCastlingMoves(const Position *position, MoveList *moveList, Player player);

void addCastlingMoves(const Position *position, MoveList *moveList, Player player, const MoveList *opponentMoves);


#endif /* EngMoveGenerator_h */
