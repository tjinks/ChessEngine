//
//  Evaluation.h
//  ChessEngine
//
//  Created by Tony on 31/05/2024.
//

#ifndef Evaluation_h
#define Evaluation_h

#include "GameState.h"

typedef bool (*EvaluationFuncSelector)(const Position *);
typedef double (*EvaluationFunc)(const Position *, Player);

typedef struct {
    EvaluationFuncSelector isApplicable;
    EvaluationFunc evaluate;
} Evaluator;

double evaluate(const Position *position, Player player);

#endif /* Evaluation_h */
