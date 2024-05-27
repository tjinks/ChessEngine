//
//  MoveListTests.m
//  ChessEngineTests
//
//  Created by Tony on 23/05/2024.
//

#import <XCTest/XCTest.h>

#include "MoveList.h"
#include "Piece.h"
#include "BoardGeometry.h"

@interface MoveListTests : XCTestCase

@end

@implementation MoveListTests

static Move createMove(int target, Piece piece, double score) {
    Move result = {2, NoSquare, NoPiece, score, { {NoSquare, NoPiece}, {target, piece} }};
    return result;
}

- (void)setUp {
    // Put setup code here. This method is called before the invocation of each test method in the class.
}

- (void)tearDown {
    // Put teardown code here. This method is called after the invocation of each test method in the class.
}

- (void)testAddMove {
    MoveList *moveList = acquireMoveList();
    MoveList *result = moveList;
    Move move;
    for (int i = 0; i < 16; i++) {
        move.atomCount = i;
        result = addMove(result, move);
        XCTAssertTrue(moveList == result);
        XCTAssertEqual(i + 1, result->size);
        XCTAssertEqual(16, result->capacity);
    }
    
    move.atomCount = 16;
    result = addMove(result, move);
    XCTAssertFalse(moveList == result);
    XCTAssertEqual(17, result->size);
    XCTAssertEqual(32, result->capacity);
    for (int i = 0; i < 17; i++) {
        XCTAssertEqual(i, result->moves[i].atomCount);
    }
    
    releaseMoveList(moveList);
}

- (void)testSortMoves {
    Move move1 = createMove(NoSquare, NoPiece, 1.0);
    Move move2 = createMove(NoSquare, NoPiece, 3.0);
    Move move3 = createMove(NoSquare, NoPiece, 2.0);
    
    MoveList *moveList = acquireMoveList();
    moveList = addMove(moveList, move1);
    moveList = addMove(moveList, move2);
    moveList = addMove(moveList, move3);
    sortMoveList(moveList);
    XCTAssertEqual(3.0, moveList->moves[0].score);
    XCTAssertEqual(2.0, moveList->moves[1].score);
    XCTAssertEqual(1.0, moveList->moves[2].score);
    releaseMoveList(moveList);
}

- (void)testContainsTargets {
    Move move1 = createMove(e1, BlackBishop, 0.0);
    Move move2 = createMove(d5, BlackKnight, 0.0);
    Move move3 = createMove(c3, BlackPawn, 0.0);
    MoveList *moveList = acquireMoveList();
    moveList = addMove(moveList, move1);
    moveList = addMove(moveList, move2);
    moveList = addMove(moveList, move3);
    
    XCTAssertTrue(containsTargets(moveList, (1UL << d5) | (1UL << a7)));
    XCTAssertFalse(containsTargets(moveList, (1UL << d6) | (1UL << d7)));

    releaseMoveList(moveList);
}

@end
