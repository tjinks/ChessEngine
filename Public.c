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
#include "AnalysisData.h"

const EngSquare EngNoSquare = NoSquare;

EngPiece *engGetBoardPtr(EngGameStateDto *dto) {
    return &dto->board[0];
}

EngGameState *engCreateGameState(const EngGameStateDto *dto) {
    EngGameState *result = getMem(sizeof(EngGameState));
    GameState *gameState = acquireGameState();
    result->handle = gameState;
    Position *position = &gameState->position;
    memcpy(position->board, dto->board, sizeof(Piece) * 64);

    position->castlingFlags = 0;
    if (dto->blackCanCastleLong) position->castlingFlags |= BlackLong;
    if (dto->blackCanCastleShort) position->castlingFlags |= BlackShort;
    if (dto->whiteCanCastleLong) position->castlingFlags |= WhiteLong;
    if (dto->whiteCanCastleShort) position->castlingFlags |= WhiteShort;
    
    position->epSquare = dto->epSquare;
    position->playerToMove = dto->playerToMove;
    
    gameState->halfMoveClock = dto->halfMoveClock;
    gameState->moveNumber = dto->moveNumber;
    
    return result;
}

void engFreeGameState(EngGameState *engGameState) {
    releaseGameState(engGameState->handle);
    freeMem(engGameState);
}

EngGameStateDto engGetGameStateDto(const EngGameState *engGameState) {
    EngGameStateDto result;
    GameState *gameState = engGameState->handle;
    const Position *position = &gameState->position;
    CastlingFlags castlingFlags = position->castlingFlags;
    result.blackCanCastleLong = ((castlingFlags & BlackLong) != 0);
    result.blackCanCastleShort = ((castlingFlags & BlackShort) != 0);
    result.whiteCanCastleLong = ((castlingFlags & WhiteLong) != 0);
    result.whiteCanCastleShort = ((castlingFlags & WhiteShort) != 0);
    memcpy(result.board, gameState->position.board, sizeof(Piece) * 64);
    result.epSquare = position->epSquare;
    result.playerToMove = position->playerToMove;
    result.halfMoveClock = gameState->halfMoveClock;
    result.moveNumber = gameState->moveNumber;
    return result;
}

EngMoveQueryResult *engFindMovesByFromAndTo(const EngGameState *engGameState, EngSquare from, EngSquare to) {
    AnalysisData analysisData = createAnalysisData(engGameState->handle);
    EngMoveQueryResult *result = NULL;
    MoveList *moveList = analysisData.activePlayerMoves;
    const Position *position = analysisData.position;
    for (int i = 0; i < moveList->size; i++) {
        Move move = moveList->moves[i];
        int f = move.atoms[0].square;
        int t = move.atoms[1].square;
        if (f == from && t == to) {
            EngMoveQueryResult *queryResult = getMem(sizeof(EngMoveQueryResult));
            queryResult->nextResult = result;
            result = queryResult;
        }
    }
    return NULL;
}

EngMoveQueryResult *engFindMovesByPieceAndTo(const EngGameState *engGameState, EngPieceType pieceType, EngSquare to) {
    return NULL;
}

void engFreeMoveQueryResults(EngMoveQueryResult *firstResult) {
    if (firstResult) {
        engFreeMoveQueryResults(firstResult->nextResult);
        freeMem(firstResult);
    }
}



