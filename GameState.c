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
#include "MoveGenerator.h"

static const GameState *gsFreeList = NULL;

static const int HashFactor = 255 * 2;

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
                
                if (getPieceType(newContents) == King) {
                    position->kingSquare[position->playerToMove] = square;
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
    
    position->hash ^= 1;
    position->playerToMove ^= (White ^ Black);
    position->epSquare = move.epSquare;
    gameState->halfMoveClock = restartClock ? 100 : gameState->halfMoveClock - 1;
}

static void populateMoveLists(GameState *gameState) {
    const Position *position = &gameState->position;
    Player playerToMove = position->playerToMove;
    MoveGenerator activePlayerMoveGenerator, passivePlayerMoveGenerator;
    initMoveGenerator(&activePlayerMoveGenerator, position, playerToMove);
    initMoveGenerator(&passivePlayerMoveGenerator, position, getOpponent(playerToMove));
    
    addNonCastlingMoves(&activePlayerMoveGenerator);
    addNonCastlingMoves(&passivePlayerMoveGenerator);
    addCastlingMoves(&activePlayerMoveGenerator, passivePlayerMoveGenerator.moveList);
    addCastlingMoves(&activePlayerMoveGenerator, activePlayerMoveGenerator.moveList);
    
    gameState->activePlayerMoves = activePlayerMoveGenerator.moveList;
    gameState->passivePlayerMoves = passivePlayerMoveGenerator.moveList;
}

static bool isInCheck(GameState *gameState, bool activePlayer) {
    Player playerToMove = gameState->position.playerToMove;
    Player player = activePlayer ? playerToMove : getOpponent(playerToMove);
    int kingSquare = gameState->position.kingSquare[player];
    const MoveList *moveList;
    if (player == playerToMove) {
        moveList = getPassivePlayerMoves(gameState);
    } else {
        moveList = getActivePlayerMoves(gameState);
    }
    
    return containsTargets(moveList, ((SquareMask)1 << kingSquare));
}

bool isActivePlayerInCheck(GameState *gameState) {
    return isInCheck(gameState, true);
}

bool isPassivePlayerInCheck(GameState *gameState) {
    return isInCheck(gameState, false);
}

GameState *acquireGameState(void) {
    GameState *result;
    if (gsFreeList) {
        result = (GameState *)gsFreeList;
        gsFreeList = gsFreeList->prev;
    } else {
        result = getMem(sizeof(GameState));
    }

    result->activePlayerMoves = NULL;
    result->passivePlayerMoves = NULL;
    return result;
}

const GameState *retractMove(const GameState *gs) {
    const GameState *result = gs->prev;
    if (gs->activePlayerMoves) releaseMoveList(gs->activePlayerMoves);
    if (gs->passivePlayerMoves) releaseMoveList(gs->passivePlayerMoves);
    releaseGameState(gs);
    return result;
}

void releaseGameState(const GameState *gs) {
    ((GameState *)gs)->prev = gsFreeList;
    gsFreeList = gs;
}

GameState *makeMove(const GameState *initialState, Move move) {
    GameState *result = acquireGameState();
    result->position = initialState->position;
    result->halfMoveClock = initialState->halfMoveClock;
    result->moveNumber = initialState->moveNumber;
    updatePosition(result, move);
    if (result->position.playerToMove == White) {
        result->moveNumber++;
    }
    
    result->prev = initialState;
    return result;
}

void calculateHash(Position *position) {
    int hash = 0;
    for (int square = 0; square < 64; square++) {
        hash += HashFactor * (square + 1) * position->board[square];
    }
    
    if (position->playerToMove == Black) {
        hash += 1;
    }
    
    position->hash = hash;
}

const MoveList *getActivePlayerMoves(GameState *gameState) {
    if (gameState->activePlayerMoves == NULL) {
        populateMoveLists(gameState);
    }
    
    return gameState->activePlayerMoves;
}

const MoveList *getPassivePlayerMoves(GameState *gameState) {
    if (gameState->passivePlayerMoves == NULL) {
        populateMoveLists(gameState);
    }
    
    return gameState->passivePlayerMoves;
}



