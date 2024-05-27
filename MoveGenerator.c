//
//  EngMoveGenerator.c
//  ChessBE
//
//  Created by Tony on 19/05/2024.
//

#include <stdlib.h>
#include <string.h>
#include "MoveGenerator.h"
#include "MoveList.h"
#include "BoardGeometry.h"
#include "EngCommon.h"
#include "GameState.h"
#include "Piece.h"
#include "BoardGeometry.h"

typedef struct {
    const Position *position;
    MoveList *moveList;
    Player player;
    Direction pawnMoveDirection;
    Direction pawnCaptureDirections[2];
    int secondRank;
    int seventhRank;
} MoveGenerator;

static Move createSimpleMove(const Position *position, int from, int to) {
    Move result;
    result.atomCount = 2;
    result.atoms[1].square = to;
    result.atoms[1].newContents = position->board[from];
    result.atoms[0].square = from;
    result.atoms[0].newContents = NoPiece;
    result.promoteTo = NoPiece;
    result.epSquare = NoSquare;
    return result;
}

static void addSimpleMove(MoveGenerator *generator, int from, int to) {
    generator->moveList = addMove(generator->moveList, createSimpleMove(generator->position, from, to));
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
    move.epSquare = generator->pawnMoveDirection(from);
    generator->moveList = addMove(generator->moveList, move);
}

static bool tryAddSimpleMove(MoveGenerator *generator, int from, int to) {
    Piece piece = generator->position->board[to];
    Player owner = getOwner(piece);
    if (owner == NoPlayer) {
        addSimpleMove(generator, from, to);
        return true;
    } else if (owner == getOpponent(generator->player)) {
        addSimpleMove(generator, from, to);
        return false;
    } else {
        return false;
    }
}

static void addRookMoves(MoveGenerator *generator, int from) {
    int to;
    to = north(from);
    while (to != NoSquare) {
        if (!tryAddSimpleMove(generator, from, to)) break;
        to = north(to);
    }
    
    to = south(from);
    while (to != NoSquare) {
        if (!tryAddSimpleMove(generator, from, to)) break;
        to = south(to);
    }

    to = east(to);
    while (to != NoSquare) {
        if (!tryAddSimpleMove(generator, from, to)) break;
        to = east(to);
    }

    to = west(to);
    while (to != NoSquare) {
        if (!tryAddSimpleMove(generator, from, to)) break;
        to = west(to);
    }
}

static void addBishopMoves(MoveGenerator *generator, int from) {
    int to;
    to = nw(from);
    while (to != NoSquare) {
        if (!tryAddSimpleMove(generator, from, to)) break;
        to = nw(to);
    }
    
    to = ne(from);
    while (to != NoSquare) {
        if (!tryAddSimpleMove(generator, from, to)) break;
        to = ne(to);
    }

    to = sw(to);
    while (to != NoSquare) {
        if (!tryAddSimpleMove(generator, from, to)) break;
        to = sw(to);
    }

    to = se(to);
    while (to != NoSquare) {
        if (!tryAddSimpleMove(generator, from, to)) break;
        to = se(to);
    }
}

static void addQueenMoves(MoveGenerator *generator, int from) {
    addBishopMoves(generator, from);
    addRookMoves(generator, from);
}

inline static int initialKingSquare(Player player) {
    switch (player) {
        case White: return e1;
        case Black: return e8;
        default: return NoSquare;
    }
}

static void addKingMoves(MoveGenerator *generator, int from) {
    SquareList targets = kingMovesFrom(from);
    for (int i = 0; i < targets.count; i++) {
        tryAddSimpleMove(generator, from, targets.squares[i]);
    }
}

static void addKnightMoves(MoveGenerator *generator, int from) {
    SquareList targets = knightMovesFrom(from);
    for (int i = 0; i < targets.count; i++) {
        tryAddSimpleMove(generator, from, targets.squares[i]);
    }
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

static void tryAddPawnCapture(MoveGenerator *generator, int from, Direction direction) {
    int to = direction(from);
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
    int to = generator->pawnMoveDirection(from);
    if (position->board[to] == NoPiece) {
        addSinglePawnMove(generator, from, to);
        
        // How about double move?
        if (r == generator->secondRank) {
            to = generator->pawnMoveDirection(to);
            if (position->board[to] == NoPiece) {
                addSinglePawnMove(generator, from, to);
            }
        }
    }
    
    // Now look for captures
    to = generator->pawnCaptureDirections[0](from);
    tryAddPawnCapture(generator, from, generator->pawnCaptureDirections[0]);
    tryAddPawnCapture(generator, from, generator->pawnCaptureDirections[1]);
}

static void initMoveGenerator(MoveGenerator *generator, const Position *position, Player player) {
    generator->position = position;
    generator->moveList = acquireMoveList();
    generator->player = player;
    if (player == White) {
        generator->pawnMoveDirection = north;
        generator->pawnCaptureDirections[0] = ne;
        generator->pawnCaptureDirections[1] = nw;
        generator->secondRank = 1;
        generator->seventhRank = 6;
    } else {
        generator->pawnMoveDirection = south;
        generator->pawnCaptureDirections[0] = se;
        generator->pawnCaptureDirections[1] = sw;
        generator->secondRank = 6;
        generator->seventhRank = 1;
    }
}

MoveList *generateMoves(const Position *position, Player player) {
    MoveGenerator generator;
    initMoveGenerator(&generator, position, player);
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

void addNonCastlingMoves(const Position *position, MoveList *moveList, Player player);

static const SquareMask whiteShortCastlingMask = (SquareMask)1 << e1 | (SquareMask)1 << f1 | (SquareMask)1 << g1;
static const SquareMask whiteLongCastlingMask = (SquareMask)1 << e1 | (SquareMask)1 << d1 | (SquareMask)1 << c1;
static const SquareMask blackShortCastlingMask = (SquareMask)1 << e8 | (SquareMask)1 << f8 | (SquareMask)1 << g8;
static const SquareMask blackLongCastlingMask = (SquareMask)1 << e8 | (SquareMask)1 << d8 | (SquareMask)1 << c8;

static void tryAddCastlingMove(const Position *position, MoveList *moveList, int player, int direction, const MoveList *opponentMoves) {
    int kingSquare;
    Piece king, rook;
    if (player == White) {
        if ((position->castlingFlags && WhiteLong) == 0 && direction == -1) return;
        if ((position->castlingFlags && WhiteShort) == 0 && direction == 1) return;
        kingSquare = e1;
        king = WhiteKing;
        rook = WhiteRook;
    } else {
        if ((position->castlingFlags && BlackLong) == 0 && direction == -1) return;
        if ((position->castlingFlags && BlackShort) == 0 && direction == 1) return;
        kingSquare = e8;
        king = BlackKing;
        rook = BlackRook;
    }

    SquareMask mask = 0;
    int rookSquare = NoSquare;
    int i = 0;
    for (int i = 0; rookSquare == NoSquare; i++) {
        int square = kingSquare + direction * i;
        if (i <= 2) {
            mask |= (SquareMask)1 << square;
        }
        
        switch (square) {
            case e1:
            case e8:
                if (position->board[square] != king) return;
                break;
            case a1:
            case a8:
            case h1:
            case h8:
                if (position->board[square] != rook) return;
                rookSquare = square;
                break;
            default:
                if (position->board[square] != NoPiece) return;
                break;
        }
    }
    
    if (containsTargets(opponentMoves, mask)) return;
    
    Move move = {4, NoSquare, NoPiece, 0.0};
    move.atoms[0].square = kingSquare;
    move.atoms[0].newContents = NoPiece;
    move.atoms[1].square = kingSquare + 2 * direction;
    move.atoms[1].newContents = king;
    move.atoms[2].square = rookSquare;
    move.atoms[2].newContents = NoPiece;
    move.atoms[3].square = kingSquare + direction;
    move.atoms[3].newContents = rook;
    
    addMove(moveList, move);
}

void addCastlingMoves(const Position *position, MoveList *moveList, Player player, const MoveList *opponentMoves) {
    if (position->castlingFlags == 0) {
        return;
    }
    
    tryAddCastlingMove(position, moveList,player, 1, opponentMoves);
    tryAddCastlingMove(position, moveList, player, -1, opponentMoves);
}

