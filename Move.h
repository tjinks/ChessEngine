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
    MoveAtom atoms[4];
    int epSquare;
    int promoteTo;
    double score;
} Move;

#endif /* EngMove_h */
