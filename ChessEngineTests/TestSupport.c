//
//  TestSupport.c
//  ChessEngineTests
//
//  Created by Tony on 27/05/2024.
//

#include "TestSupport.h"
#include "Public.h"

GameState *parseFen(const char *fen) {
    EngParseFenResult parseFenResult = engParseFen(fen);
    struct EngGame *engGame = engStartGame(parseFenResult.position);
    GameState *result = getMem(sizeof(GameState));
    *result = *(engGame->gameState);
    engFreeGame(engGame);
    engFreePosition(parseFenResult.position);
    return result;
}
