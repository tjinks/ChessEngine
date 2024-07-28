//
//  EngMove.h
//  ChessBE
//
//  Created by Tony on 17/05/2024.
//

#ifndef Move_h
#define Move_h

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

inline int hashMove(Move move) {
    int result = move.atomCount;
    result = (result << 6) | move.promoteTo;
    result = (result << 6) | move.atoms[0].square;
    result = (result << 6) | move.atoms[1].square;
    result = (result << 6) | move.atoms[1].newContents;
    return result;
}

#endif /* Move_h */
