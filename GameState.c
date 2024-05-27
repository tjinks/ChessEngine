//
//  EngPosition.c
//  ChessBE
//
//  Created by Tony on 18/05/2024.
//

#include <stdio.h>
#include "GameState.h"
#include "Piece.h"
#include "Move.h"
#include "EngCommon.h"
#include "BoardGeometry.h"

static GameState *gsFreeList = NULL;

GameState *acquireGameState(void) {
    GameState *result;
    if (gsFreeList) {
        result = gsFreeList;
        gsFreeList = gsFreeList->prev;
    } else {
        result = getMem(sizeof(GameState));
    }

    result->activeMoves = acquireMoveList();
    result->passiveMoves = acquireMoveList();
    return result;
}

GameState *retractMove(GameState *gs) {
    GameState *result = gs->prev;
    releaseGameState(gs);
    return result;
}

void releaseGameState(GameState *gs) {
    releaseMoveList(gs->activeMoves);
    releaseMoveList(gs->passiveMoves);
    gs->prev = gsFreeList;
    gsFreeList = gs;
}

static void updatePosition(Position *position, Move move) {
    int hash = position->hash;
    for (int i = 0; i < move.atomCount; i++) {
        int square = move.atoms[i].square;
        Piece newContents = move.atoms[i].newContents;
        Piece originalContents = position->board[square];
        position->board[square] = newContents;
        hash += 255 * (square + 1) * (newContents - originalContents);
        if (position->castlingFlags) {
            switch (square) {
                case a1:
                    position->castlingFlags &= ~WhiteLong;
                    break;
                case e1:
                    position->castlingFlags &= ~(WhiteLong | WhiteShort);
                    break;
                case h1:
                    position->castlingFlags &= ~WhiteShort;
                    break;
                case a8:
                    position->castlingFlags &= ~BlackLong;
                    break;
                case e8:
                    position->castlingFlags &= ~(BlackLong | BlackShort);
                    break;
                case h8:
                    position->castlingFlags &= ~BlackShort;
                    break;
            }
        }
    }
    
    position->hash = -hash;
    position->epSquare = move.epSquare;
}

GameState *makeMove(const GameState *initialState, Move move) {
    GameState *result = acquireGameState();
    *result = *initialState;
    updatePosition(&result->position, move);
    
    return NULL;
}
