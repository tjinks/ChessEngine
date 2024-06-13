//
//  Public.h
//  This file defines the public interface for the chess engine.
//
//  Created by Tony on 21/05/2024.
//

#ifndef Public_h
#define Public_h

#include <stdbool.h>
#include <stdint.h>

typedef uint_least64_t EngSquareMask;

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
    EngSquare from, to;
    EngPiece piece;
} EngPieceMove;

typedef struct EngMove {
    struct EngMove *nextMove;
    EngPieceMove primary;
    EngPieceMove secondary;
    EngSquare epCaptureSquare;
    EngPiece promoteTo;
} EngMove;

typedef struct {
    EngMove *firstMove;
} EngMoveList;

typedef struct {
    EngPiece *board;
    EngPlayer playerToMove;
    bool whiteCanCastleLong, whiteCanCastleShort, blackCanCastleLong, blackCanCastleShort;
    EngSquare epSquare;
    EngMoveCounter halfMoveClock;
    EngMoveCounter moveNumber;
} EngPosition;

struct EngGame;

/*=================================================
 *  Position setup and access
 =================================================*/
EngPosition *engCreatePosition(void);

void engFreePosition(const EngPosition *position);

struct EngGame *engStartGame(const EngPosition *position);

EngPosition *engGetCurrentPosition(const struct EngGame *game);

void engFreeGame(const struct EngGame *game);

bool engIsCheck(const struct EngGame *game);

/*=================================================
 *  Move and move list management
 =================================================*/
EngMoveList *engGetMovesByFromAndTo(struct EngGame *game, EngSquare from, EngSquare to);

void engFreeMoveList(const EngMoveList *moveList);

EngSquareMask engGetTargets(struct EngGame *game, EngSquare from);

struct EngGame *engMakeMove(const EngMove *move);

bool engIsCastles(const EngMove *engMove);

/*=================================================
 *  Chess notation support (see Notation.c)
 =================================================*/
EngPosition *engParseFen(const char *fen, void (*errorCallback)(const char *message));

/*=================================================
 *  Miscellaneous functions
 =================================================*/
bool isWinForActivePlayer(EngGameResult result);

bool isWinForPassivePlayer(EngGameResult result);

bool isDraw(EngGameResult result);



#endif /* Public_h */

