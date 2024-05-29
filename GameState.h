//
//  EngPosition.h
//  ChessBE
//
//  Created by Tony on 18/05/2024.
//

#ifndef EngPosition_h
#define EngPosition_h

#include "Piece.h"
#include "MoveList.h"
#include "Public.h"

typedef enum {
    WhiteLong = 1
    ,WhiteShort = 2
    ,BlackLong = 4
    ,BlackShort = 8
} CastlingFlags;

typedef struct {
    Piece board[64];
    CastlingFlags castlingFlags;
    int epSquare;
    int hash;
    Player playerToMove;
    int kingSquare[2];
} Position;

typedef struct EngGameState {
    struct EngGameState *prev;
    Position position;
    int halfMoveClock;
    int moveNumber;
    MoveList *activeMoves;
    MoveList *passiveMoves;
} GameState;

GameState *acquireGameState(void);

GameState *makeMove(const GameState *initialState, Move move);

void releaseGameState(GameState *gameState);

GameState *retractMove(GameState *gameState);

#endif /* EngPosition_h */
