//
//  Evaluation.c
//  ChessEngine
//
//  Created by Tony on 31/05/2024.
//

#include <stdio.h>
#include "Evaluation.h"

static const double QueenValue = 9.0;
static const double RookValue = 5.0;
static const double BishopValue = 3.2;
static const double KnightValue = 3.0;
static const double PawnValue = 1.0;

static bool alwaysSelect(const Position *p) {
    return true;
}

static double evaluateMaterial(const Position *position, Player player) {
    double result = 0;
    for (int i = 0; i < 64; i++) {
        switch (position->board[i]) {
            case WhiteQueen:
                result += QueenValue;
                break;
            case BlackQueen:
                result -= QueenValue;
                break;
            case WhiteRook:
                result += RookValue;
                break;
            case BlackRook:
                result -= RookValue;
                break;
            case WhiteBishop:
                result += BishopValue;
                break;
            case BlackBishop:
                result -= BishopValue;
                break;
            case WhiteKnight:
                result += KnightValue;
                break;
            case BlackKnight:
                result -= KnightValue;
                break;
            case WhitePawn:
                result += PawnValue;
                break;
            case BlackPawn:
                result -= PawnValue;
                break;
            default:
                break;
        }
    }
    
    return player == White ? result : -result;
}

static const Evaluator Evaluators[] = 
{
    {alwaysSelect, evaluateMaterial}
};

static const int EvaluatorCount = sizeof(Evaluators)/sizeof(Evaluator);

double evaluate(const Position *position, Player player) {
    for (int i = 0; i < EvaluatorCount; i++) {
        if (Evaluators[i].isApplicable(position)) {
            return Evaluators[i].evaluate(position, player);
        }
    }
    
    return 0.0;
}
