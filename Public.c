//
//  Public.c
//  ChessEngine
//
//  Created by Tony on 21/05/2024.
//

#include <string.h>
#include <stdio.h>
#include "Public.h"
#include "EngCommon.h"
#include "BoardGeometry.h"
#include "GameState.h"
#include "AnalysisData.h"

const EngSquare EngNoSquare = NoSquare;

typedef struct {
    Move move;
    GameState *gameState;
} MoveWrapper;

typedef struct {
    MoveList *moveList;
    GameState *gameState;
} MoveListWrapper;

static void fatalError(const char *msg) {
    fprintf(stderr, "%s\n", msg);
    exit(1);
}

static EngObj handleInvalidType(EngObj obj, const char *funcName) {
    char errorMsg[100];
    static const EngObj result = {EngNone, NULL};
    sprintf(errorMsg, "Invalid object type %d passed to %s", (int)obj.type, funcName);
    return result;
}

static void *verifyType(EngObj obj, EngObjType expected, const char *funcName) {
    if (obj.type == expected) {
        return obj.obj;
    }
    
    handleInvalidType(obj, funcName);
    return NULL;
}

#define VERIFY_TYPE(obj, expected) (verifyType((obj), (expected), __func__))

void engObjFree(EngObj obj) {
    switch (obj.type) {
        case EngGameState:
            releaseGameState(obj.obj);
            break;
        case EngMoveList:
            releaseMoveList(obj.obj);
            break;
        case EngMove:
            freeMem(obj.obj);
            break;
        default:
            handleInvalidType(obj, __func__);
    }
}

/*=================================================
 *  GameState accessors
 =================================================*/
EngObj engCreateGameState(void) {
    EngObj result = {EngGameState, acquireGameState()};
    return result;
}

EngPiece getPieceAt(EngObj gameState, EngSquare square) {
    GameState *gs = VERIFY_TYPE(gameState, EngGameState);
    return gs->position.board[square];
}

void setPieceAt(EngObj gameState, EngSquare square, EngPiece piece) {
    GameState *gs = VERIFY_TYPE(gameState, EngGameState);
    gs->position.board[square] = piece;
}

EngPlayer engGetPlayerToMove(EngObj gameState) {
    GameState *gs = VERIFY_TYPE(gameState, EngGameState);
    return gs->position.playerToMove;
}

void engSetPlayerToMove(EngObj gameState, EngPlayer player) {
    GameState *gs = VERIFY_TYPE(gameState, EngGameState);
    gs->position.playerToMove = player;
}

EngCastlingFlags engGetCastlingFlags(EngObj gameState) {
    GameState *gs = VERIFY_TYPE(gameState, EngGameState);
    CastlingFlags castlingFlags = gs->position.castlingFlags;
    EngCastlingFlags result;
    result.blackCanCastleLong = (castlingFlags & BlackLong) != 0;
    result.blackCanCastleShort = (castlingFlags & BlackShort) != 0;
    result.whiteCanCastleLong = (castlingFlags & WhiteLong) != 0;
    result.whiteCanCastleShort = (castlingFlags & WhiteShort) != 0;
    return result;
}

void engSetCastlingFlags(EngObj gameState, EngCastlingFlags castlingFlags) {
    GameState *gs = VERIFY_TYPE(gameState, EngGameState);
    CastlingFlags cf = 0;
    if (castlingFlags.blackCanCastleLong) cf |= BlackLong;
    if (castlingFlags.blackCanCastleShort) cf |= BlackShort;
    if (castlingFlags.whiteCanCastleLong) cf |= WhiteLong;
    if (castlingFlags.whiteCanCastleShort) cf |= WhiteShort;
    gs->position.castlingFlags = cf;
}

EngSquare engGetEpSquare(EngObj gameState) {
    GameState *gs = VERIFY_TYPE(gameState, EngGameState);
    return gs->position.epSquare;
}

void engSetEpSquare(EngObj gameState, EngSquare square) {
    GameState *gs = VERIFY_TYPE(gameState, EngGameState);
    gs->position.epSquare = square;
}

EngMoveCounter engGetHalfMoveClock(EngObj gameState) {
    GameState *gs = VERIFY_TYPE(gameState, EngGameState);
    return gs->halfMoveClock;
}

void engSetHalfMoveClock(EngObj gameState, EngMoveCounter halfMoveClock) {
    GameState *gs = VERIFY_TYPE(gameState, EngGameState);
    gs->halfMoveClock = halfMoveClock;
}

EngMoveCounter engGetMoveNumber(EngObj gameState) {
    GameState *gs = VERIFY_TYPE(gameState, EngGameState);
    return gs->moveNumber;
}

void engSetMoveNumber(EngObj gameState, EngMoveCounter moveNumber) {
    GameState *gs = VERIFY_TYPE(gameState, EngGameState);
    gs->moveNumber = moveNumber;
}

EngGameResult engGetResult(EngObj gameState) {
    return NoResult;
}

bool engIsInCheck(EngObj gameState) {
    GameState *gs = VERIFY_TYPE(gameState, EngGameState);
    AnalysisData analysisData = createAnalysisData(gs);
    bool result = isActivePlayerInCheck(&analysisData);
    freeAnalysisData(&analysisData);
    return result;
}

/*=================================================
 *  Move and move list management
 =================================================*/
EngObj engGetMoveByFromAndTo(EngObj gameState, EngSquare from, EngSquare to) {
    AnalysisData analysisData = createAnalysisData(VERIFY_TYPE(gameState, EngGameState));
    MoveList *moveList = duplicateMoveList(analysisData.activePlayerMoves);
    
    MoveListWrapper *moveListWrapper = getMem(sizeof(MoveListWrapper));
    moveListWrapper->gameState = duplicateGameState(analysisData.gameState);
    moveListWrapper->moveList = duplicateMoveList(analysisData.activePlayerMoves);
    freeAnalysisData(&analysisData);
    EngObj result = {EngMoveList, moveListWrapper};
    return result;
}

EngObj engGetMovesByFrom(EngObj gameState, EngSquare from);

int engGetMoveListSize(EngObj moveList);

EngObj engGetMove(EngObj moveList, int index);

EngObj engMakeMove(EngObj gameState, EngObj move);

EngPieceMove engGetPrimaryMove(EngObj engMove);

EngPieceMove engGetSecondaryMove(EngObj engMove);

bool engIsCastles(EngObj engMove);

bool engIsPromotion(EngObj engMove);

EngSquare engGetEpSquare(EngObj engMove);

/*=================================================
 *  Miscellaneous functions
 =================================================*/
bool isWinForActivePlayer(EngGameResult result);

bool isWinForPassivePlayer(EngGameResult result);

bool isDraw(EngGameResult result);


