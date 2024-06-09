//
//  Analyser.c
//  ChessEngine
//
//  Created by Tony on 31/05/2024.
//

#include "AnalysisData.h"
#include "MoveGenerator.h"

static bool isInCheck(const AnalysisData *analysisData, bool activePlayer) {
    Player player = activePlayer ? analysisData->playerToMove : getOpponent(analysisData->playerToMove);
    int kingSquare = analysisData->position->kingSquare[player];
    const MoveList *moveList;
    if (player == analysisData->playerToMove) {
        moveList = analysisData->passivePlayerMoves;
    } else {
        moveList = analysisData->activePlayerMoves;
    }
    
    return containsTargets(moveList, ((SquareMask)1 << kingSquare));
}

AnalysisData createAnalysisData(const GameState *gameState) {
    AnalysisData result;
    result.gameState = gameState;
    result.position = &gameState->position;
    result.playerToMove = gameState->position.playerToMove;
    
    MoveGenerator activePlayerMoveGenerator, passivePlayerMoveGenerator;
    initMoveGenerator(&activePlayerMoveGenerator, result.position, result.playerToMove);
    initMoveGenerator(&passivePlayerMoveGenerator, result.position, getOpponent(result.playerToMove));
    
    addNonCastlingMoves(&activePlayerMoveGenerator);
    addNonCastlingMoves(&passivePlayerMoveGenerator);
    addCastlingMoves(&activePlayerMoveGenerator, passivePlayerMoveGenerator.moveList);
    addCastlingMoves(&activePlayerMoveGenerator, activePlayerMoveGenerator.moveList);
    result.activePlayerMoves = activePlayerMoveGenerator.moveList;
    result.passivePlayerMoves = passivePlayerMoveGenerator.moveList;
    return result;
}

void freeAnalysisData(const AnalysisData *data) {
    releaseMoveList(data->activePlayerMoves);
    releaseMoveList(data->passivePlayerMoves);
}

bool isActivePlayerInCheck(const AnalysisData *analysisData) {
    return isInCheck(analysisData, true);
}

bool isPassivePlayerInCheck(const AnalysisData *analysisData) {
    return isInCheck(analysisData, false);
}



