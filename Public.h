//
//  Public.h
//  This file defines the public interface for the chess engine.
//
//  Created by Tony on 21/05/2024.
//

#ifndef Public_h
#define Public_h

#include <stdbool.h>

typedef enum  {
    NoResult,
    WinByCheckMate,
    WinByResignation,
    WinOnTime,
    DrawByStalemate,
    DrawByRepetition,
    DrawBy50MoveRule,
    DrawByAgreement
} EngGameResult;

typedef enum {
    White = 0,
    Black,
    NoPlayer
} EngPlayer;

typedef enum {
    Pawn = 0
    ,Knight
    ,Bishop
    ,Rook
    ,Queen
    ,King
    ,NoPieceType
} EngPieceType;

typedef enum {
    WhitePawn = Pawn
    ,WhiteKnight
    ,WhiteBishop
    ,WhiteRook
    ,WhiteQueen
    ,WhiteKing
    ,BlackPawn = WhitePawn + 16
    ,BlackKnight
    ,BlackBishop
    ,BlackRook
    ,BlackQueen
    ,BlackKing
    ,NoPiece = NoPlayer * 16 + NoPieceType
} EngPiece;

typedef int EngSquare;
typedef int EngMoveCounter;

typedef struct {
    EngPlayer playerToMove;
    EngSquare epSquare;
    bool whiteCanCastleShort, whiteCanCastleLong, blackCanCastleShort, blackCanCastleLong;
    EngMoveCounter halfMoveClock;
    EngMoveCounter moveNumber;
    EngPiece board[64];
} EngGameStateDto;

typedef struct {
    int positionsAnalysed;
    int ply;
} EngAnalysisProgress;

typedef bool (*ProgressCallback)(EngAnalysisProgress);

extern const EngSquare EngNoSquare;

typedef struct {
    bool whiteCanCastleShort;
    bool whiteCanCastleLong;
    bool blackCanCastleShort;
    bool blackCanCastleLong;
} EngCastlingFlags;

typedef enum {
    EngNone,
    EngGameState,
    EngMoveList,
    EngMove
} EngObjType;

typedef struct {
    EngObjType type;
    void *obj;
} EngObj;

typedef struct {
    EngSquare from, to;
    EngPiece piece;
} EngPieceMove;

void engObjFree(EngObj obj);

/*=================================================
 *  GameState accessors
 =================================================*/
EngObj engCreateGameState(void);

EngPiece getPieceAt(EngObj gameState, EngSquare square);
void setPieceAt(EngObj gameState, EngSquare square, EngPiece piece);

EngPlayer engGetPlayerToMove(EngObj gamestate);
void engSetPlayerToMove(EngObj gameState, EngPlayer player);

EngCastlingFlags engGetCastlingFlags(EngObj gameState);
void engSetCastlingFlags(EngObj gameState, EngCastlingFlags castlingFlags);

EngSquare engGetEpSquare(EngObj gameState);
void engSetEpSquare(EngObj gameState, EngSquare square);

EngMoveCounter engGetHalfMoveClock(EngObj gameState);
void engSetHalfMoveClock(EngObj gameState, EngMoveCounter halfMoveClock);

EngMoveCounter engGetMoveNumber(EngObj gameState);
void engSetMoveNumber(EngObj gameState, EngMoveCounter moveNumber);

EngGameResult engGetResult(EngObj gameState);

bool engIsInCheck(EngObj gameState);

/*=================================================
 *  Move and move list management
 =================================================*/
EngObj engGetMoveByFromAndTo(EngObj gameState, EngSquare from, EngSquare to);

EngObj engGetMovesByFrom(EngObj gameState, EngSquare from);

int engGetMoveListSize(EngObj moveList);

EngObj engGetMove(EngObj moveList, int index);

EngObj engMakeMove(EngObj gameState, EngObj engMove);

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



#endif /* Public_h */

