//
//  AnalysisDataTests.m
//  ChessEngineTests
//
//  Created by Tony on 02/06/2024.
//

#import <XCTest/XCTest.h>

#include "TestSupport.h"
#include "GameState.h"

@interface GameStateTests : XCTestCase

@end

@implementation GameStateTests

- (void)setUp {
    // Put setup code here. This method is called before the invocation of each test method in the class.
}

- (void)tearDown {
    // Put teardown code here. This method is called after the invocation of each test method in the class.
}

- (void)testIsActivePlayerInCheckFalse {
    GameState *gameState = parseFen("4k3/8/8/8/8/3q4/3P4/R3K2R w");
    XCTAssertFalse(isActivePlayerInCheck(gameState));
    releaseGameState(gameState);
}

- (void)testIsPassivePlayerInCheckFalse {
    GameState *gameState = parseFen("4k3/8/8/8/8/3q4/3P4/R3K2R b");
    XCTAssertFalse(isActivePlayerInCheck(gameState));
    releaseGameState(gameState);
}

- (void)testIsActivePlayerInCheckTrue {
    GameState *gameState = parseFen("4k3/8/8/q7/8/8/8/4K3 w");
    XCTAssertTrue(isActivePlayerInCheck(gameState));
    releaseGameState(gameState);
}

- (void)testIsPassivePlayerInCheckTrue {
    GameState *gameState = parseFen("4k3/8/8/q7/8/8/8/4K3 b");
    XCTAssertFalse(isPassivePlayerInCheck(gameState));
    releaseGameState(gameState);
}

@end
