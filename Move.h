//
//  EngMove.h
//  ChessBE
//
//  Created by Tony on 17/05/2024.
//

#ifndef EngMove_h
#define EngMove_h

#include "Piece.h"

typedef struct {
    int square;
    Piece newContents;
} MoveAtom;

typedef struct {
    int atomCount;
    int epSquare;
    Piece promoteTo;
    double score;
    MoveAtom atoms[4];
} Move;

#endif /* EngMove_h */
