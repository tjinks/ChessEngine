//
//  Analysis.h
//  ChessEngine
//
//  Created by Tony on 11/07/2024.
//

#ifndef Analysis_h
#define Analysis_h

#include "GameState.h"

typedef enum {
    Checkmate,
    Stalemate,
    NotMate
} MateCheckResult;

MateCheckResult mateCheck(GameState *gameState);

bool isDrawByRepetition(GameState *gameState);

#endif /* Analysis_h */
