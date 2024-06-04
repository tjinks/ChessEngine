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

static const int HashFactor = 255;

static void updatePosition(GameState *gameState, Move move) {
    Position *position = &gameState->position;
    int hash = position->hash;
    bool restartClock = false;
    for (int i = 0; i < move.atomCount; i++) {
        int square = move.atoms[i].square;
        Piece newContents = move.atoms[i].newContents;
        Piece originalContents = position->board[square];
        switch (i) {
            case 0:
                if (getPieceType(originalContents) == Pawn) {
                    restartClock = true;
                }
                
                break;
            case 1:
                if (originalContents != NoPiece) {
                    restartClock = true;
                }
                
                break;
            default:
                break;
        }
        
        position->board[square] = newContents;
        hash += HashFactor * (square + 1) * (newContents - originalContents);
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
    gameState->halfMoveClock = restartClock ? 100 : gameState->halfMoveClock - 1;
}

GameState *acquireGameState(void) {
    GameState *result;
    if (gsFreeList) {
        result = gsFreeList;
        gsFreeList = gsFreeList->prev;
    } else {
        result = getMem(sizeof(GameState));
    }

    return result;
}

GameState *retractMove(GameState *gs) {
    GameState *result = gs->prev;
    releaseGameState(gs);
    return result;
}

void releaseGameState(GameState *gs) {
    gs->prev = gsFreeList;
    gsFreeList = gs;
}

GameState *makeMove(GameState *initialState, Move move) {
    GameState *result = acquireGameState();
    *result = *initialState;
    updatePosition(result, move);
    if (result->position.playerToMove == White) {
        result->moveNumber++;
    }
    
    result->prev = initialState;
    return result;
}

void generateHash(Position *position) {
    int hash = 0;
    for (int square = 0; square < 64; square++) {
        hash += HashFactor * (square + 1) * position->board[square];
    }
    
    if (position->playerToMove == Black) {
        hash = -hash;
    }
    
    position->hash = hash;
}
