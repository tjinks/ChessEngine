//
//  MoveGeneratorTests.m
//  ChessEngineTests
//
//  Created by Tony on 28/05/2024.
//

#import <XCTest/XCTest.h>

#include <stdbool.h>
#include "TestSupport.h"
#include "MoveGenerator.h"

static bool findMove(const MoveList *moveList, int from, int to) {
    for (int i = 0; i < moveList->size; i++) {
        Move move = moveList->moves[i];
        if (move.atoms[0].square == from) {
            if (move.atoms[1].square == to) {
                return true;
            }
        }
    }
    
    return false;
}

static int countFrom(const MoveList *moveList, int from) {
    int result = 0;
    for (int i = 0; i < moveList->size; i++) {
        if (moveList->moves[i].atoms[0].square == from) {
            result++;
        }
    }
    
    return result;
}

@interface MoveGeneratorTests : XCTestCase

@end

@implementation MoveGeneratorTests

- (void)setUp {
    // Put setup code here. This method is called before the invocation of each test method in the class.
}

- (void)tearDown {
    // Put teardown code here. This method is called after the invocation of each test method in the class.
}

- (void)testKingMoves {
    static const char *fen = "k7/3K4/2pP4/8/8/8/8/8 w";
    GameState *gameState = parseFen(fen);
    XCTAssertTrue(gameState != NULL);
    if (gameState) {
        MoveGenerator generator;
        initMoveGenerator(&generator, &gameState->position, White);
        MoveList *moveList = generator.moveList;
        addNonCastlingMoves(&generator);
        XCTAssertEqual(7, countFrom(moveList, d7));
        XCTAssertTrue(findMove(moveList, d7, d8));
        XCTAssertTrue(findMove(moveList, d7, e8));
        XCTAssertTrue(findMove(moveList, d7, e7));
        XCTAssertTrue(findMove(moveList, d7, e6));
        XCTAssertTrue(findMove(moveList, d7, c6));
        XCTAssertTrue(findMove(moveList, d7, c7));
        XCTAssertTrue(findMove(moveList, d7, c8));

        releaseMoveList(moveList);
        initMoveGenerator(&generator, &gameState->position, Black);
        moveList = generator.moveList;
        addNonCastlingMoves(&generator);
        XCTAssertEqual(3, countFrom(moveList, a8));
        XCTAssertTrue(findMove(moveList, a8, b8));
        XCTAssertTrue(findMove(moveList, a8, b7));
        XCTAssertTrue(findMove(moveList, a8, a7));

        releaseMoveList(moveList);
    }
}

- (void)testKnightMoves {
    static const char *fen = "k7/8/1n1K4/8/2N5/8/8/8 w";
    GameState *gameState = parseFen(fen);
    XCTAssertTrue(gameState != NULL);
    if (gameState) {
        MoveGenerator generator;
        initMoveGenerator(&generator, &gameState->position, White);
        MoveList *moveList = generator.moveList;
        addNonCastlingMoves(&generator);
        XCTAssertEqual(7, countFrom(moveList, c4));
        XCTAssertTrue(findMove(moveList, c4, b6));
        XCTAssertTrue(findMove(moveList, c4, e5));
        XCTAssertTrue(findMove(moveList, c4, e3));
        XCTAssertTrue(findMove(moveList, c4, d2));
        XCTAssertTrue(findMove(moveList, c4, b2));
        XCTAssertTrue(findMove(moveList, c4, a3));
        XCTAssertTrue(findMove(moveList, c4, a5));

        releaseMoveList(moveList);
        initMoveGenerator(&generator, &gameState->position, Black);
        moveList = generator.moveList;
        addNonCastlingMoves(&generator);
        XCTAssertEqual(5, countFrom(moveList, b6));
        XCTAssertTrue(findMove(moveList, b6, c8));
        XCTAssertTrue(findMove(moveList, b6, d7));
        XCTAssertTrue(findMove(moveList, b6, d5));
        XCTAssertTrue(findMove(moveList, b6, c4));
        XCTAssertTrue(findMove(moveList, b6, a4));

        releaseMoveList(moveList);
    }
}




@end
