//
//  Public.c
//  ChessEngine
//
//  Created by Tony on 21/05/2024.
//

#include <string.h>
#include "Public.h"
#include "EngCommon.h"
#include "BoardGeometry.h"
#include "GameState.h"

const EngSquare EngNoSquare = NoSquare;

static const EngPiece InitialPosition[] = {
    WhiteRook, WhiteKnight, WhiteBishop, WhiteQueen, WhiteKing, WhiteBishop, WhiteKnight, WhiteRook,
    WhitePawn, WhitePawn, WhitePawn, WhitePawn, WhitePawn, WhitePawn, WhitePawn, WhitePawn,
    NoPiece, NoPiece, NoPiece, NoPiece, NoPiece, NoPiece, NoPiece, NoPiece,
    NoPiece, NoPiece, NoPiece, NoPiece, NoPiece, NoPiece, NoPiece, NoPiece,
    NoPiece, NoPiece, NoPiece, NoPiece, NoPiece, NoPiece, NoPiece, NoPiece,
    NoPiece, NoPiece, NoPiece, NoPiece, NoPiece, NoPiece, NoPiece, NoPiece,
    BlackPawn, BlackPawn, BlackPawn, BlackPawn, BlackPawn, BlackPawn, BlackPawn, BlackPawn,
    BlackRook, BlackKnight, BlackBishop, BlackQueen, BlackKing, BlackBishop, BlackKnight, BlackRook
};

EngGameStateDto *engCreateGameStateDto(void) {
    EngGameStateDto *result = getMem(sizeof(EngGameStateDto));
    *(EngPiece **)(&result->board) = getMem(sizeof(EngPiece) * 64);
    memcpy(result->board, InitialPosition, sizeof(EngPiece) * 64);
    result->blackCanCastleLong = true;
    result->blackCanCastleShort = true;
    result->whiteCanCastleLong = true;
    result->whiteCanCastleShort = true;
    result->epSquare = NoSquare;
    result->halfMoveClock = 100;
    result->moveNumber = 1;
    result->playerToMove = White;
    return result;
}

void engFreeGameStateDto(EngGameStateDto *dto) {
    freeMem(dto->board);
    freeMem(dto);
}

struct EngGameState *engDtoToGameState(const EngGameStateDto *dto) {
    GameState *result = acquireGameState();
    Position *position = &result->position;
    memcpy(position->board, dto->board, sizeof(Piece) * 64);

    position->castlingFlags = 0;
    if (dto->blackCanCastleLong) position->castlingFlags |= BlackLong;
    if (dto->blackCanCastleShort) position->castlingFlags |= BlackShort;
    if (dto->whiteCanCastleLong) position->castlingFlags |= WhiteLong;
    if (dto->whiteCanCastleShort) position->castlingFlags |= WhiteShort;
    
    position->epSquare = dto->epSquare;
    position->playerToMove = dto->playerToMove;
    
    result->halfMoveClock = dto->halfMoveClock;
    result->moveNumber = dto->moveNumber;
    
    return result;
}

void engFreeGameState(struct EngGameState *gameState) {
    releaseGameState(gameState);
}



