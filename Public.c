//
//  Public.c
//  ChessEngine
//
//  Created by Tony on 21/05/2024.
//

#include "Public.h"
#include "Piece.h"
#include "Position.h"
#include "EngCommon.h"

const int engPawn = Pawn;
const int engKnight = Knight;
const int engBishop = Bishop;
const int engRook = Rook;
const int engQueen = Queen;
const int engKing = King;
const int engNoPiece = NoPieceType;

const int engWhite = White;
const int engBlack = Black;
const int engNoPlayer = NoPlayer;

const int engWhiteLong = WhiteLong;
const int engWhiteShort = WhiteShort;
const int engBlackLong = BlackLong;
const int engBlackShort = BlackShort;

typedef struct EngPositionDto {
    int playerToMove;
    int epSquare;
    int castlingFlags;
    int halfMoveClock;
    int moveNumber;
    EngPiece board[64];
} EngPositionDto;

EngPositionDto *engCreatePositionDto(void) {
    return getMem(sizeof(EngPositionDto));
}

void engFreePositionDto(EngPositionDto *dto) {
    freeMem(dto);
}

void engSetPlayerToMove(EngPositionDto *dto, int playerToMove) {
    dto->playerToMove = playerToMove;
}

int engGetPlayerToMove(const EngPositionDto *dto) {
    return dto->playerToMove;
}

void engSetEpSquare(EngPositionDto *dto, int epSquare) {
    dto->epSquare = epSquare;
}

int engGetEpSquare(const EngPositionDto *dto) {
    return dto->epSquare;
}

void engSetCastlingFlags(EngPositionDto *dto, int castlingFlags) {
    dto->castlingFlags = castlingFlags;
}

int engGetCastlingFlags(const EngPositionDto *dto) {
    return dto->castlingFlags;
}

void engSetHalfMoveCount(EngPositionDto *dto, int halfMoveCount) {
    dto->halfMoveClock = halfMoveCount;
}

int engGetHalfMoveCount(const EngPositionDto *dto) {
    return dto->halfMoveClock;
}

void engSetMoveNumber(EngPositionDto *dto, int moveNumber) {
    dto->moveNumber = moveNumber;
}

int engGetMoveNumber(const EngPositionDto *dto) {
    return dto->moveNumber;
}

void engSetPieceAt(EngPositionDto *dto, int file, int rank, EngPiece piece) {
    dto->board[file + 8 * rank] = piece;
}

EngPiece engGetPieceAt(const EngPositionDto *dto, int file, int rank) {
    return dto->board[file + 8 * rank];
}

