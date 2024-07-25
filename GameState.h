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

typedef struct GameState {
    struct GameState *prev;
    Position position;
    int halfMoveClock;
    int moveNumber;
    bool isRepetitionBarrier;
    MoveList *activePlayerMoves;
    MoveList *passivePlayerMoves;
} GameState;

GameState *acquireGameState(void);

const MoveList *getActivePlayerMoves(GameState *gameState);

const MoveList *getPassivePlayerMoves(GameState *gameState);

GameState *makeMove(GameState *initialState, Move move);

void releaseGameState(GameState *gameState);

GameState *retractMove(GameState *gameState);

void calculateHash(Position *position);

bool isActivePlayerInCheck(GameState *);

bool isPassivePlayerInCheck(GameState *);

bool isSamePosition(GameState *gs1, GameState *gs2);

bool isThreefoldRepetition(GameState *gameState);

#endif /* EngPosition_h */
