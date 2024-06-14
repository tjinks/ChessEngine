//
//  TestSupport.c
//  ChessEngineTests
//
//  Created by Tony on 27/05/2024.
//

#include "TestSupport.h"
#include "Public.h"

GameState *parseFen(const char *fen) {
    EngPosition *position = engParseFen(fen, NULL);
    struct EngGame *engGame = engStartGame(position);
    GameState *result = getMem(sizeof(GameState));
    *result = *(engGame->gameState);
    engFreeGame(engGame);
    return result;
}
