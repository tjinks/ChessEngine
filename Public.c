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
#include "Piece.h"
#include "Analysis.h"

const EngSquare EngNoSquare = NoSquare;

typedef struct {
    EngPosition position;
    Piece board[64];
} EngPositionWrapper;

typedef struct {
    EngMove move;
    int hash;
    EngGame *game;
} EngMoveWrapper;

static const Piece initialPosition[] = {
    WhiteRook, WhiteKnight, WhiteBishop, WhiteQueen, WhiteKing, WhiteBishop, WhiteKnight, WhiteRook,
    WhitePawn, WhitePawn, WhitePawn, WhitePawn, WhitePawn, WhitePawn, WhitePawn, WhitePawn,
    NoPiece, NoPiece, NoPiece, NoPiece, NoPiece, NoPiece, NoPiece, NoPiece,
    NoPiece, NoPiece, NoPiece, NoPiece, NoPiece, NoPiece, NoPiece, NoPiece,
    NoPiece, NoPiece, NoPiece, NoPiece, NoPiece, NoPiece, NoPiece, NoPiece,
    NoPiece, NoPiece, NoPiece, NoPiece, NoPiece, NoPiece, NoPiece, NoPiece,
    BlackPawn, BlackPawn, BlackPawn, BlackPawn, BlackPawn, BlackPawn, BlackPawn, BlackPawn,
    BlackRook, BlackKnight, BlackBishop, BlackQueen, BlackKing, BlackBishop, BlackKnight, BlackRook
};

/*=================================================
 *  Position setup and access
 =================================================*/
EngPosition *engCreatePosition(void) {
    EngPositionWrapper *result = getMem(sizeof(EngPositionWrapper));
    memcpy(result->board, initialPosition, 64 * sizeof(Piece));
    EngPosition *position = &result->position;
    position->blackCanCastleLong = true;
    position->blackCanCastleShort = true;
    position->whiteCanCastleLong = true;
    position->whiteCanCastleShort = true;
    position->epSquare = NoSquare;
    position->playerToMove = White;
    position->halfMoveClock = 100;
    position->moveNumber = 1;
    position->board = result->board;
    return position;
}

void engFreePosition(const EngPosition *position) {
    freeMem(position);
}

struct EngGame *engStartGame(const EngPosition *engPosition) {
    GameState *gameState = acquireGameState();
    Position *position = &gameState->position;
    memcpy(position->board, engPosition->board, 64 * sizeof(Piece));
    position->epSquare = engPosition->epSquare;
    position->playerToMove = engPosition->playerToMove;
    
    position->castlingFlags = 0;
    if (engPosition->blackCanCastleLong) position->castlingFlags |= BlackLong;
    if (engPosition->blackCanCastleShort) position->castlingFlags |= BlackShort;
    if (engPosition->whiteCanCastleLong) position->castlingFlags |= WhiteLong;
    if (engPosition->whiteCanCastleShort) position->castlingFlags |= WhiteShort;
    gameState->halfMoveClock = engPosition->halfMoveClock;
    gameState->moveNumber = engPosition->moveNumber;
    position->kingSquare[White] = position->kingSquare[Black] = NoSquare;
    
    for (int i = 0; i < 64; i++) {
        if (position->board[i] == WhiteKing) position->kingSquare[White] = i;
        if (position->board[i] == BlackKing) position->kingSquare[Black] = i;
    }
    
    calculateHash(position);
    
    EngGame *result = getMem(sizeof(EngGame));
    result->gameState = gameState;
    return result;
}

EngPosition *engGetCurrentPosition(const struct EngGame *game) {
    const GameState *gameState = game->gameState;
    EngPositionWrapper *result = getMem(sizeof(EngPositionWrapper));
    result->position.board = result->board;
    const Position *position = &gameState->position;
    memcpy(result->board, position->board, 64 * sizeof(Piece));
    CastlingFlags castlingFlags = position->castlingFlags;
    result->position.blackCanCastleLong = (castlingFlags & BlackLong) != 0;
    result->position.whiteCanCastleLong = (castlingFlags & WhiteLong) != 0;
    result->position.blackCanCastleShort = (castlingFlags & BlackShort) != 0;
    result->position.whiteCanCastleShort = (castlingFlags & WhiteShort) != 0;
    result->position.epSquare = position->epSquare;
    result->position.playerToMove = position->playerToMove;
    result->position.halfMoveClock = gameState->halfMoveClock;
    result->position.moveNumber = gameState->moveNumber;
    return &result->position;
}

struct EngGame *clone(const struct EngGame *game) {
    GameState *gsCopy = acquireGameState();
    *gsCopy = *game->gameState;
    gsCopy->prev = NULL;
    EngGame *result = getMem(sizeof(EngGame));
    result->gameState = gsCopy;
    return result;
}


void engFreeGame(const struct EngGame *game) {
    releaseGameState(game->gameState);
    freeMem(game);
}

bool engIsCheck(struct EngGame *game) {
    bool result = false;
    const Position *position = &game->gameState->position;
    const MoveList *passivePlayerMoves = getPassivePlayerMoves(game->gameState);
    int target = position->kingSquare[position->playerToMove];
    for (int i = 0; i < passivePlayerMoves->size && !result; i++) {
        Move move = passivePlayerMoves->moves[i];
        if (move.atoms[1].square == target) {
            result = true;
        }
    }
    
    return result;
}

/*=================================================
 *  Move and move list management
 =================================================*/
static bool legalMoveFilter(Move move, void *filterData) {
    GameState *gameState = filterData;
    GameState *newState = makeMove(gameState, move);
    bool result = !isPassivePlayerInCheck(newState);
    retractMove(newState);
    return result;
}

static bool fromFilter(Move move, void *filterData) {
    int from = *((int *)filterData);
    return move.atoms[0].square == from;
}

struct FromAndTo {
    int from, to;
};

static bool fromAndToFilter(Move move, void *filterData) {
    const struct FromAndTo *fromAndTo = filterData;
    return move.atoms[0].square == fromAndTo->from && move.atoms[1].square == fromAndTo->to;
}

static EngMoveWrapper *createEngMoveWrapper(EngGame *game, Move move, int index) {
    EngMoveWrapper *result = getMem(sizeof(EngMoveWrapper));
    EngMove *engMove = &result->move;
    const Position *position = &game->gameState->position;
    engMove->primary.from = move.atoms[0].square;
    engMove->primary.to = move.atoms[1].square;
    engMove->primary.piece = position->board[engMove->primary.from];
    
    if (move.atomCount == 4) {
        engMove->secondary.from = move.atoms[2].square;
        engMove->secondary.to = move.atoms[3].square;
        engMove->secondary.piece = position->board[engMove->secondary.from];
    } else {
        engMove->secondary.from = NoSquare;
        engMove->secondary.to = NoSquare;
        engMove->secondary.piece = NoPiece;
    }
    
    if (move.atomCount == 3) {
        engMove->epCaptureSquare = move.atoms[2].square;
    } else {
        engMove->epCaptureSquare = NoSquare;
    }
    
    engMove->promoteTo = move.promoteTo;
    
    result->hash = hashMove(move);
    result->game = game;
    return result;
}

static EngMoveList *moveListToEngMoveList(EngGame *game, const MoveList *moveList) {
    EngMoveList *result = getMem(sizeof(EngMoveList));
    result->firstMove = NULL;
    for (int i = 0; i < moveList->size; i++) {
        EngMoveWrapper *wrapper = createEngMoveWrapper(game, moveList->moves[i], i);
        EngMove *engMove = &wrapper->move;
        engMove->nextMove = result->firstMove;
        result->firstMove = engMove;
    }
    
    return result;
}

EngMoveList *engGetMovesByFromAndTo(struct EngGame *game, EngSquare from, EngSquare to) {
    GameState *gameState = game->gameState;
    const MoveList *activePlayerMoves = getActivePlayerMoves(gameState);
    MoveList *legalMoves = filterMoveList(activePlayerMoves, legalMoveFilter, game->gameState);
    struct FromAndTo fromAndTo = {from, to};
    MoveList *requiredMoves = filterMoveList(legalMoves, fromAndToFilter, &fromAndTo);
    EngMoveList *result = moveListToEngMoveList(game, requiredMoves);
    releaseMoveList(legalMoves);
    releaseMoveList(requiredMoves);
    return result;
}

void engFreeMoveList(const EngMoveList *moveList) {
    EngMove *currentMove = moveList->firstMove;
    while (currentMove) {
        EngMove *nextMove = currentMove->nextMove;
        freeMem(currentMove);
        currentMove = nextMove;
    }
    
    freeMem(moveList);
}

EngSquareMask engGetTargets(struct EngGame *game, EngSquare from) {
    const MoveList *activePlayerMoves = getActivePlayerMoves(game->gameState);
    MoveList *filteredByFrom = filterMoveList(activePlayerMoves, fromFilter, &from);
    MoveList *requiredMoves = filterMoveList(filteredByFrom, legalMoveFilter, game->gameState);
    EngSquareMask result = 0;
    for (int i = 0; i < requiredMoves->size; i++) {
        Move move = requiredMoves->moves[i];
        int to = move.atoms[1].square;
        result |= ((SquareMask)1 << to);
    }
    
    releaseMoveList(filteredByFrom);
    releaseMoveList(requiredMoves);
    return result;
}

EngGame *engMakeMove(const EngMove *engMove) {
    const EngMoveWrapper *wrapper = (const EngMoveWrapper *)engMove;
    GameState *gameState = wrapper->game->gameState;
    const MoveList *activePlayerMoves = getActivePlayerMoves(gameState);
    for (int i = 0; i < activePlayerMoves->size; i++) {
        if (wrapper->hash == hashMove(activePlayerMoves->moves[i])) {
            wrapper->game->gameState = makeMove(gameState, activePlayerMoves->moves[i]);
            return wrapper->game;
        }
    }
    
    fatalError("Move not found in current position's active move list");
    return NULL;
}

struct EngGame *engRetractMove(struct EngGame *game) {
    game->gameState = retractMove(game->gameState);
    return game;
}

bool engIsCastles(const EngMove *engMove) {
    return engMove->secondary.piece != NoPiece;
}

/*=================================================
 *  Analysis
 =================================================*/
EngGameResult engGetResult(const struct EngGame *game) {
    GameState *gameState = game->gameState;
    if (gameState->halfMoveClock == 0) {
        return DrawBy50MoveRule;
    }

    switch (mateCheck(gameState)) {
        case NotMate:
            return NoResult;
        case Checkmate:
            return WinByCheckmate;
        case Stalemate:
            return DrawByStalemate;
    }
}

/*=================================================
 *  Miscellaneous functions
 =================================================*/
void engFreeString(const char *s) {
    freeMem(s);
}

Piece engMakePiece(Player owner, PieceType type) {
    return makePiece(owner, type);
}

EngPieceType engGetPieceType(EngPiece piece) {
    return getPieceType(piece);
}

EngPlayer engGetOwner(EngPiece piece) {
    return getOwner(piece);
}



