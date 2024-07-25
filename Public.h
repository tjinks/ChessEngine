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
    WinByCheckmate,
    WinOnTime,
    DrawByStalemate,
    DrawByRepetition,
    DrawBy50MoveRule
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

struct GameState;

typedef struct EngGame {
    struct GameState *gameState;
} EngGame;

typedef struct {
    bool success;
    union {
        EngPosition *position;
        const char *errorMessage;
    };
} EngParseFenResult;

/*=================================================
 *  Position setup and access
 =================================================*/
EngPosition *engCreatePosition(void);

void engFreePosition(const EngPosition *position);

struct EngGame *engStartGame(const EngPosition *position);

struct EngGame *clone(const struct EngGame *game);

EngPosition *engGetCurrentPosition(const struct EngGame *game);

void engFreeGame(const struct EngGame *game);

bool engIsCheck(struct EngGame *game);

/*=================================================
 *  Move and move list management
 =================================================*/
EngMoveList *engGetMovesByFromAndTo(struct EngGame *game, EngSquare from, EngSquare to);

void engFreeMoveList(const EngMoveList *moveList);

EngSquareMask engGetTargets(struct EngGame *game, EngSquare from);

struct EngGame *engMakeMove(const EngMove *move);

bool engIsCastles(const EngMove *engMove);

/*=================================================
 *  Analysis
 =================================================*/
EngGameResult engGetResult(const struct EngGame *game);

/*=================================================
 *  Chess notation support (see Notation.c)
 =================================================*/
EngParseFenResult engParseFen(const char *fen);

/*=================================================
 *  Miscellaneous functions
 =================================================*/
void engFreeString(const char *s);

EngPiece engMakePiece(EngPlayer owner, EngPieceType type);

EngPieceType engGetPieceType(EngPiece piece);

EngPlayer engGetOwner(EngPiece piece);



#endif /* Public_h */

