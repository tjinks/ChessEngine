//
//  EngPiece.h
//  ChessBE
//
//  Created by Tony on 17/05/2024.
//

#ifndef Piece_h
#define Piece_h

#include "Public.h"

typedef EngPlayer Player;
typedef EngPiece Piece;
typedef EngPieceType PieceType;

inline Piece makePiece(Player owner, PieceType type) {
    return owner * 16 + type;
}

inline PieceType getPieceType(Piece piece) {
    return piece % 16;
}

inline Player getOwner(Piece piece) {
    return piece / 16;
}

inline Player getOpponent(Player player) {
    switch (player) {
    case White:
        return Black;
    case Black:
        return White;
    default:
        return NoPlayer;
    }
}

#endif /* EngPiece_h */
