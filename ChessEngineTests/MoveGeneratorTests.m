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

static bool findPromotionMove(const MoveList *moveList, int from, int to, PieceType promoteTo);

static bool findMove(const MoveList *moveList, int from, int to) {
    return findPromotionMove(moveList, from, to, NoPieceType);
}

static bool findPromotionMove(const MoveList *moveList, int from, int to, PieceType promoteTo) {
    for (int i = 0; i < moveList->size; i++) {
        Move move = moveList->moves[i];
        if (move.atoms[0].square == from) {
            if (move.atoms[1].square == to) {
                if (getPieceType(move.promoteTo) == promoteTo) {
                    return true;
                }
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
        addNonCastlingMoves(&generator);
        MoveList *moveList = generator.moveList;
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
        addNonCastlingMoves(&generator);
        moveList = generator.moveList;
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
        addNonCastlingMoves(&generator);
        MoveList *moveList = generator.moveList;
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
        addNonCastlingMoves(&generator);
        moveList = generator.moveList;
        XCTAssertEqual(5, countFrom(moveList, b6));
        XCTAssertTrue(findMove(moveList, b6, c8));
        XCTAssertTrue(findMove(moveList, b6, d7));
        XCTAssertTrue(findMove(moveList, b6, d5));
        XCTAssertTrue(findMove(moveList, b6, c4));
        XCTAssertTrue(findMove(moveList, b6, a4));
        
        releaseMoveList(moveList);
    }
}

- (void)testBishopMoves {
    static const char *fen = "1k6/8/2K5/3B4/8/5n2/8/8 w";
    GameState *gameState = parseFen(fen);
    XCTAssertTrue(gameState != NULL);
    if (gameState) {
        MoveGenerator generator;
        initMoveGenerator(&generator, &gameState->position, White);
        addNonCastlingMoves(&generator);
        MoveList *moveList = generator.moveList;
        XCTAssertEqual(8, countFrom(moveList, d5));
        XCTAssertTrue(findMove(moveList, d5, e6));
        XCTAssertTrue(findMove(moveList, d5, f7));
        XCTAssertTrue(findMove(moveList, d5, g8));
        XCTAssertTrue(findMove(moveList, d5, e4));
        XCTAssertTrue(findMove(moveList, d5, f3));
        XCTAssertTrue(findMove(moveList, d5, c4));
        XCTAssertTrue(findMove(moveList, d5, b3));
        XCTAssertTrue(findMove(moveList, d5, a2));
        
        releaseMoveList(moveList);
    }
}

- (void)testRookMoves {
    static const char *fen = "1k6/8/2K5/8/8/2R2n2/8/8 w";
    GameState *gameState = parseFen(fen);
    XCTAssertTrue(gameState != NULL);
    if (gameState) {
        MoveGenerator generator;
        initMoveGenerator(&generator, &gameState->position, White);
        addNonCastlingMoves(&generator);
        MoveList *moveList = generator.moveList;
        XCTAssertEqual(9, countFrom(moveList, c3));
        XCTAssertTrue(findMove(moveList, c3, c4));
        XCTAssertTrue(findMove(moveList, c3, c5));
        XCTAssertTrue(findMove(moveList, c3, d3));
        XCTAssertTrue(findMove(moveList, c3, e3));
        XCTAssertTrue(findMove(moveList, c3, f3));
        XCTAssertTrue(findMove(moveList, c3, c2));
        XCTAssertTrue(findMove(moveList, c3, c1));
        XCTAssertTrue(findMove(moveList, c3, b3));
        XCTAssertTrue(findMove(moveList, c3, a3));
        
        releaseMoveList(moveList);
    }
}

- (void)testWhitePawnMoves {
    static const char *fen = "4k3/1P6/8/5pP1/2n1b3/3P4/5P2/4K3 w ---- f6";
    GameState *gameState = parseFen(fen);
    XCTAssertTrue(gameState != NULL);
    if (gameState) {
        MoveGenerator generator;
        initMoveGenerator(&generator, &gameState->position, White);
        addNonCastlingMoves(&generator);
        MoveList *moveList = generator.moveList;
        XCTAssertEqual(4, countFrom(moveList, b7));
        XCTAssertEqual(3, countFrom(moveList, d3));
        XCTAssertEqual(2, countFrom(moveList, f2));
        XCTAssertEqual(2, countFrom(moveList, g5));
        
        XCTAssertTrue(findPromotionMove(moveList, b7, b8, Queen));
        XCTAssertTrue(findPromotionMove(moveList, b7, b8, Rook));
        XCTAssertTrue(findPromotionMove(moveList, b7, b8, Knight));
        XCTAssertTrue(findPromotionMove(moveList, b7, b8, Bishop));
        
        XCTAssertTrue(findMove(moveList, d3, c4));
        XCTAssertTrue(findMove(moveList, d3, d4));
        XCTAssertTrue(findMove(moveList, d3, e4));
        
        XCTAssertTrue(findMove(moveList, f2, f3));
        XCTAssertTrue(findMove(moveList, f2, f4));
        
        XCTAssertTrue(findMove(moveList, g5, f6));
        XCTAssertTrue(findMove(moveList, g5, g6));
        
        releaseMoveList(moveList);
    }
}


- (void)testBlackPawnMoves {
    static const char *fen = "4k3/5p2/3p4/2N1B3/5Pp1/8/1p6/4K3 b ---- f3";
    GameState *gameState = parseFen(fen);
    XCTAssertTrue(gameState != NULL);
    if (gameState) {
        MoveGenerator generator;
        initMoveGenerator(&generator, &gameState->position, Black);
        addNonCastlingMoves(&generator);
        MoveList *moveList = generator.moveList;
        XCTAssertEqual(4, countFrom(moveList, b2));
        XCTAssertEqual(3, countFrom(moveList, d6));
        XCTAssertEqual(2, countFrom(moveList, f7));
        XCTAssertEqual(2, countFrom(moveList, g4));
        
        XCTAssertTrue(findPromotionMove(moveList, b2, b1, Queen));
        XCTAssertTrue(findPromotionMove(moveList, b2, b1, Rook));
        XCTAssertTrue(findPromotionMove(moveList, b2, b1, Knight));
        XCTAssertTrue(findPromotionMove(moveList, b2, b1, Bishop));
        
        XCTAssertTrue(findMove(moveList, d6, c5));
        XCTAssertTrue(findMove(moveList, d6, d5));
        XCTAssertTrue(findMove(moveList, d6, e5));
        
        XCTAssertTrue(findMove(moveList, f7, f6));
        XCTAssertTrue(findMove(moveList, f7, f5));
        
        XCTAssertTrue(findMove(moveList, g4, f3));
        XCTAssertTrue(findMove(moveList, g4, g3));

        releaseMoveList(moveList);
    }
}


@end
