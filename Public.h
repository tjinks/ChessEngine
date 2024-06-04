//
//  Public.h
//  This file defines the public interface for the chess engine.
//
//  Created by Tony on 21/05/2024.
//

#ifndef Public_h
#define Public_h

#include <stdbool.h>

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

typedef struct {
    struct GameState *handle;
} EngGameState;



extern const EngSquare EngNoSquare;

typedef struct {
    EngPiece piece;
    EngSquare source, target;
} EngMovePart;

typedef struct {
    int partCount;
    EngMovePart parts[2];
    EngSquare epCapture;
    bool isCheck;
    bool isCheckmate;
    bool isCapture;
} EngMove;

typedef struct EngMoveQueryResult {
    struct EngMoveQueryResult *nextResult;
    EngMove *moves;
} EngMoveQueryResult;

typedef struct {
    bool whiteCanCastleShort;
    bool whiteCanCastleLong;
    bool blackCanCastleShort;
    bool blackCanCastleLong;
} EngCastlingFlags;

EngPiece *engGetBoardPtr(EngGameStateDto *);

EngGameState *engCreateGameState(const EngGameStateDto *);

void engFreeGameState(EngGameState *gameState);

EngGameStateDto engGetGameStateDto(const EngGameState *gameState);

EngMoveQueryResult *engFindMovesByFromAndTo(const EngGameState *gameState, EngSquare from, EngSquare to);

EngMoveQueryResult *engFindMovesByPieceAndTo(const EngGameState *gameState, EngPieceType pieceType, EngSquare to);

void engFreeMoveQueryResults(EngMoveQueryResult *firstResult);

void engMakeMove(EngGameState *gameState, EngMove move);

void retractLastMove(EngGameState *gameState);

#endif /* Public_h */

