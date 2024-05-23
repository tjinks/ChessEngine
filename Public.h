//
//  Public.h
//  This file defines the public interface for the chess engine.
//
//  Created by Tony on 21/05/2024.
//

#ifndef Public_h
#define Public_h

extern const int engPawn;
extern const int engKnight;
extern const int engBishop;
extern const int engRook;
extern const int engQueen;
extern const int engKing;
extern const int engNoPiece;

extern const int engWhite;
extern const int engBlack;
extern const int engNoPlayer;

extern const int engWhiteCanCastleLong;
extern const int engWhiteCanCastleShort;
extern const int engBlackCanCastleLong;
extern const int engBlackCanCastleShort;

typedef struct {
    int type;
    int owner;
} EngPiece;

typedef struct EngPositionDto EngPositionDto;

EngPositionDto *engCreatePositionDto(void);

void engFreePositionDto(EngPositionDto *dto);

void engSetPieceAt(EngPositionDto *dto, int file, int rank, EngPiece piece);

EngPiece engGetPieceAt(const EngPositionDto *dto, int file, int rank);

void engSetPlayerToMove(EngPositionDto *dto, int playerToMove);

int engGetPlayerToMove(const EngPositionDto *dto);

void engSetEpSquare(EngPositionDto *dto, int epSquare);

int engGetEpSquare(const EngPositionDto *dto);

void engSetCastlingFlags(EngPositionDto *dto, int castlingFlags);

int engGetCastlingFlags(const EngPositionDto *dto);

void engSetHalfMoveCount(EngPositionDto *dto, int halfMoveCount);

int engGetHalfMoveCount(const EngPositionDto *dto);

void engSetMoveNumber(EngPositionDto *dto, int moveNumber);

int engGetMoveNumber(const EngPositionDto *dto);

#endif /* Public_h */
