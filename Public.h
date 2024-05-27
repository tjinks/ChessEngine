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

typedef struct {
    EngPlayer playerToMove;
    int epSquare;
    bool whiteCanCastleShort, whiteCanCastleLong, blackCanCastleShort, blackCanCastleLong;
    int halfMoveClock;
    int moveNumber;
    EngPiece *const board;
} EngGameStateDto;

typedef int EngSquare;

extern const EngSquare EngNoSquare;

struct EngGameState;

typedef struct EngMoveQueryResults {
    struct EngMoveQueryResponse *next;
    EngSquare from, to;
    EngSquare from1, from2;
    EngSquare epSquare;
    EngPiece promoteTo;
} EngMoveQueryResults;

EngGameStateDto *engCreateGameStateDto(void);

void engFreeGameStateDto(EngGameStateDto *);

struct EngGameState *engDtoToGameState(const EngGameStateDto *);

void engFreeGameState(struct EngGameState *);

EngGameStateDto *engGameStateToDto(const struct EngGameState *gameState);

EngMoveQueryResults *engFindMovesByFromAndTo(const struct EngGameState *gameState, EngSquare from, EngSquare to);

EngMoveQueryResults *engFindMovesByPieceAndTo(const struct EngGameState *gameState, EngPieceType pieceType, EngSquare to);

void engFreeMoveQueryResults(EngMoveQueryResults *);



#endif /* Public_h */

