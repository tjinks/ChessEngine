//
//  Analyser.h
//  ChessEngine
//
//  Created by Tony on 31/05/2024.
//

#ifndef Analyser_h
#define Analyser_h

#include "GameState.h"

typedef struct {
    const GameState *gameState;
    const Position *position;
    Player playerToMove;
    MoveList *activePlayerMoves;
    MoveList *passivePlayerMoves;
} AnalysisData;

AnalysisData createAnalysisData(const GameState *gameState);

void freeAnalysisData(AnalysisData *);

bool isActivePlayerInCheck(const AnalysisData *analysisData);

bool isPassivePlayerInCheck(const AnalysisData *analysisData);

#endif /* Analyser_h */
