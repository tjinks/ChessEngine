//
//  Analysis.c
//  ChessEngine
//
//  Created by Tony on 11/07/2024.
//

#include "Analysis.h"

MateCheckResult mateCheck(GameState *gameState) {
    const MoveList *moveList = getActivePlayerMoves(gameState);
    for (int i = 0; i < moveList->size; i++) {
        GameState *stateAfterMove = makeMove(gameState, moveList->moves[i]);
        bool inCheck = isPassivePlayerInCheck(stateAfterMove);
        releaseGameState(stateAfterMove);
        if (!inCheck) {
            return NotMate;
        }
    }
    
    if (isActivePlayerInCheck(gameState)) {
        return Checkmate;
    } else {
        return Stalemate;
    }
}



