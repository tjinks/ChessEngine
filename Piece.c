//
//  Piece.c
//  ChessBE
//
//  Created by Tony on 17/05/2024.
//

#include "Piece.h"

extern Piece makePiece(Player owner, PieceType type);

extern PieceType getPieceType(Piece piece);

extern Player getOwner(Piece piece);

inline Player getOpponent(Player player);
