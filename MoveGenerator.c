//
//  EngMoveGenerator.c
//  ChessBE
//
//  Created by Tony on 19/05/2024.
//

#include <stdlib.h>
#include <string.h>
#include "MoveGenerator.h"
#include "EngCommon.h"
#include "Position.h"
#include "Piece.h"

static const size_t MovesOffset =  offsetof(MoveList, moves);
static const size_t MoveSize = sizeof(Move);
static const size_t InitialMoveListSize = 16;

static const int North = 8;
static const int South = -8;
static const int East = 1;
static const int West = -1;
static const int Ne = 9;
static const int Nw = 7;
static const int Se = -7;
static const int Sw = -9;

typedef struct {
    const Position *position;
    MoveList *moveList;
    Player player;
    int pawnMoveDirection;
    int pawnCaptureDirectionWest;
    int pawnCaptureDirectionEast;
    int secondRank;
    int seventhRank;
} MoveGenerator;

static MoveList *mlFreeList = NULL;

static inline int rank(int square) {
    return square / 8;
}

static inline int file(int square) {
    return square % 8;;
}

static MoveList *createMoveList(void) {
    size_t required = MovesOffset + InitialMoveListSize * MoveSize;
    MoveList *result = getMem(required);
    result->capacity = InitialMoveListSize;
    result->size = 0;
    result->next = NULL;
    return result;
}

static MoveList *getMoveList(void) {
    if (mlFreeList) {
        MoveList *result = mlFreeList;
        mlFreeList = mlFreeList->next;
        result->size = 0;
        return result;
    } else {
        return createMoveList();
    }
}

static MoveList *expand(MoveList *moveList) {
    int newCapacity = 2 * moveList->capacity;
    MoveList *result = getMem(MovesOffset + newCapacity * MoveSize);
    memcpy(result, moveList, MovesOffset + moveList->size * MoveSize);
    freeMem(moveList);
    result->capacity = newCapacity;
    return result;
}

inline static MoveList *addMove(MoveList *moveList, Move move) {
    MoveList *result;
    if (moveList->capacity == moveList->size) {
        result = expand(moveList);
    } else {
        result = moveList;
    }
    
    moveList->moves[moveList->size] = move;
    moveList->size++;
    return moveList;
}

static void freeMoveList(MoveList *moveList) {
    moveList->next = mlFreeList;
    mlFreeList = moveList;
}

static Move createSimpleMove(const Position *position, int from, int to) {
    Move result;
    result.atomCount = 2;
    result.atoms[1].square = to;
    result.atoms[1].newContents = position->board[from];
    result.atoms[0].square = from;
    result.atoms[0].newContents = NoPiece;
    result.promoteTo = -1;
    result.epSquare = -1;
    return result;
}

static void addSimpleMove(MoveGenerator *generator, int from, int to) {
    Move move;
    move.atomCount = 2;
    move.atoms[1].square = to;
    move.atoms[1].newContents = generator->position->board[from];
    move.atoms[0].square = from;
    move.atoms[0].newContents = NoPiece;
    generator->moveList = addMove(generator->moveList, move);
}

static void addPromotionMoves(MoveGenerator *generator, int from, int to) {
    static const Piece whitePromotionOptions[] = {WhiteQueen, WhiteRook, WhiteKnight, WhiteBishop, NoPiece};
    static const Piece blackPromotionOptions[] = {BlackQueen, BlackRook, BlackKnight, BlackBishop, NoPiece};
    const Piece *promotionOptions = generator->player == White ? whitePromotionOptions : blackPromotionOptions;
    for (int i = 0; promotionOptions[i] != NoPiece; i++) {
        Move move = createSimpleMove(generator->position, from, to);
        move.promoteTo = promotionOptions[i];
        generator->moveList = addMove(generator->moveList, move);
    }
}

static void addDoublePawnMove(MoveGenerator *generator, int from, int to) {
    Move move = createSimpleMove(generator->position, from, to);
    move.epSquare = from + generator->pawnMoveDirection;
    generator->moveList = addMove(generator->moveList, move);
}

static void tryAddSimpleMove(MoveGenerator *generator, int from, int to) {
    int targetPiece = generator->position->board[to];
    if (getOwner(targetPiece) != generator->player) {
        addSimpleMove(generator, from, to);
    }
}

static void addSlidingMoves(MoveGenerator *generator, int from, int direction) {
    int current = from;
    for (;;) {
        switch (direction) {
            case North:
                if (rank(current) == 7) return;
            case South:
                if (rank(current) == 0) return;
            case East:
                if (file(current) == 7) return;
            case West:
                if (file(current) == 0) return;
            case Ne:
                if (rank(current) == 7 || file(current) == 7) return;
            case Nw:
                if (rank(current) == 7 || file(current) == 0) return;
            case Se:
                if (rank(current) == 0 || file(current) == 7) return;
            case Sw:
                if (rank(current) == 0 || file(current) == 0) return;
        }
        
        current += direction;
        Piece piece = generator->position->board[current];
        Player owner = getOwner(piece);
        if (owner == generator->player) {
            return;
        }
        
        addSimpleMove(generator, from, current);
        if (owner != NoPlayer) {
            return;
        }
    }
}

static void addRookMoves(MoveGenerator *generator, int from) {
    addSlidingMoves(generator, from, North);
    addSlidingMoves(generator, from, South);
    addSlidingMoves(generator, from, East);
    addSlidingMoves(generator, from, West);
}

static void addBishopMoves(MoveGenerator *generator, int from) {
    addSlidingMoves(generator, from, Ne);
    addSlidingMoves(generator, from, Nw);
    addSlidingMoves(generator, from, Se);
    addSlidingMoves(generator, from, Sw);
}

static void addQueenMoves(MoveGenerator *generator, int from) {
    addBishopMoves(generator, from);
    addRookMoves(generator, from);
}

static void tryAddCastlingMoves(MoveGenerator *generator) {
    const Position *position = generator->position;
    if (position->castlingFlags) {
        int kingSquare = position->kingSquare[generator->player];
    }
}

static void addKingMoves(MoveGenerator *generator, int from) {
    const static int directions[] = {North, South, East, West, Ne, Nw, Se, Sw};
    int r = rank(from);
    int f = file(from);
    for (int i = 0; i < 8; i++) {
        int direction = directions[i];
        switch (direction) {
            case North:
                if (r == 7) continue;
            case South:
                if (r == 0) continue;
            case East:
                if (f == 7) continue;
            case West:
                if (f == 0) continue;
            case Ne:
                if (r == 7 || f == 7) continue;
            case Nw:
                if (r == 7 || f == 0) continue;
            case Se:
                if (r == 0 || f == 7) continue;
            case Sw:
                if (r == 0 || f == 0) continue;
        }
        
        tryAddSimpleMove(generator, from, from + direction);
    }
}

static void addKnightMoves(MoveGenerator *generator, int from) {
    int r = rank(from);
    int f = file(from);
    if (r < 6 && f < 7) tryAddSimpleMove(generator, from, from + North + Ne);
    if (r < 7 && f < 6) tryAddSimpleMove(generator, from, from + East + Ne);
    if (r > 0 && f < 6) tryAddSimpleMove(generator, from, from + East + Se);
    if (r > 1 && f < 7) tryAddSimpleMove(generator, from, from + South + Se);
    if (r > 1 && f > 0) tryAddSimpleMove(generator, from, from + South + Sw);
    if (r > 0 && f > 1) tryAddSimpleMove(generator, from, from + West + Sw);
    if (r < 7 && f > 1) tryAddSimpleMove(generator, from, from + West + Nw);
    if (r < 6 && f > 0) tryAddSimpleMove(generator, from, from + North + Nw);
}

static void addSinglePawnMove(MoveGenerator *generator, int from, int to) {
    int r = rank(from);
    if (r == generator->seventhRank) {
        addPromotionMoves(generator, from, to);
    } else if (abs(from - to) == 8) {
        addDoublePawnMove(generator, from, to);
    } else {
        addSimpleMove(generator, from, to);
    }
}

static void tryAddPawnCapture(MoveGenerator *generator, int from, int direction) {
    int to = from + direction;
    const Position *position = generator->position;
    if (to == position->epSquare || getOwner(position->board[to]) == getOpponent(generator->player)) {
        addSimpleMove(generator, from, to);
    }
}

static void addPawnMoves(MoveGenerator *generator, int from) {
    int r = rank(from);
    int f = file(from);
    const Position *position = generator->position;
    
    // Is single move advance possible?
    int to = from + generator->pawnMoveDirection;
    if (position->board[to] == NoPiece) {
        addSinglePawnMove(generator, from, to);
        
        // How about double move?
        if (r == generator->secondRank) {
            to += generator->pawnMoveDirection;
            if (position->board[to] == NoPiece) {
                addSinglePawnMove(generator, from, to);
            }
        }
    }
    
    // Now look for captures
    if (f > 0) {
        tryAddPawnCapture(generator, from, generator->pawnCaptureDirectionWest);
    }
    
    if (f < 7) {
        tryAddPawnCapture(generator, from, generator->pawnCaptureDirectionEast);
    }
}

static void initMoveGenerator(MoveGenerator *generator, const Position *position, Player player);

static void initMoveGenerator(MoveGenerator *generator, const Position *position, Player player) {
    generator->position = position;
    generator->moveList = getMoveList();
    generator->player = player;
    if (player == White) {
        generator->pawnMoveDirection = North;
        generator->pawnCaptureDirectionEast = Ne;
        generator->pawnCaptureDirectionWest = Nw;
        generator->secondRank = 1;
        generator->seventhRank = 6;
    } else {
        generator->pawnMoveDirection = South;
        generator->pawnCaptureDirectionEast = Se;
        generator->pawnCaptureDirectionWest = Sw;
        generator->secondRank = 6;
        generator->seventhRank = 1;
    }
}

MoveList *generateMoves(const Position *position, Player player) {
    MoveGenerator generator;
    initMoveGenerator(&generator, position, player);
    generator.position = position;
    generator.moveList = getMoveList();
    generator.player = player;
    if (player == White) {
        generator.pawnMoveDirection = North;
        generator.pawnCaptureDirectionEast = Ne;
        generator.pawnCaptureDirectionWest = Nw;
        generator.secondRank = 1;
        generator.seventhRank = 6;
    } else {
        generator.pawnMoveDirection = South;
        generator.pawnCaptureDirectionEast = Se;
        generator.pawnCaptureDirectionWest = Sw;
        generator.secondRank = 6;
        generator.seventhRank = 1;
    }
    
    for (int i = 0; i < 64; i++) {
        PieceType type = getPieceType(position->board[i]);
        switch (type) {
            case Pawn:
                addPawnMoves(&generator, i);
                break;
            case Knight:
                addKnightMoves(&generator, i);
                break;
            case Bishop:
                addBishopMoves(&generator, i);
                break;
            case Rook:
                addRookMoves(&generator, i);
                break;
            case Queen:
                addQueenMoves(&generator, i);
                break;
            case King:
                addKingMoves(&generator, i);
                break;
            default:
                break;
        }
    }
    
    return generator.moveList;
}
