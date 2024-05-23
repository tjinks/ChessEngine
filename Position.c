//
//  EngPosition.c
//  ChessBE
//
//  Created by Tony on 18/05/2024.
//

#include <stdio.h>
#include "Position.h"
#include "Piece.h"
#include "Move.h"
#include "EngCommon.h"

static GameState *gsFreeList = NULL;

static GameState *getGameState(void) {
    if (gsFreeList) {
        GameState *result = gsFreeList;
        gsFreeList = gsFreeList->prev;
        return result;
    } else {
        return getMem(sizeof(GameState));
    }
}

static void releaseGameState(GameState *gs) {
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

struct GameState *makeMove(const GameState *initialState, Move move) {
    GameState *result = getGameState();
    *result = *initialState;
    updatePosition(&result->position, move);
    
    return NULL;
}
