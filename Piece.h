//
//  EngPiece.h
//  ChessBE
//
//  Created by Tony on 17/05/2024.
//

#ifndef Piece_h
#define Piece_h

typedef enum {
    Pawn = 0
    ,Knight
    ,Bishop
    ,Rook
    ,Queen
    ,King
    ,NoPieceType
} PieceType;

typedef enum {
    White = 0,
    Black,
    NoPlayer
} Player;

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
} Piece;

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
