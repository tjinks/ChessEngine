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

static bool findMoveByFromAndto(const MoveList *moveList, int from, int to) {
    return findPromotionMove(moveList, from, to, NoPieceType);
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
        XCTAssertTrue(findMoveByFromAndto(moveList, d7, d8));
        XCTAssertTrue(findMoveByFromAndto(moveList, d7, e8));
        XCTAssertTrue(findMoveByFromAndto(moveList, d7, e7));
        XCTAssertTrue(findMoveByFromAndto(moveList, d7, e6));
        XCTAssertTrue(findMoveByFromAndto(moveList, d7, c6));
        XCTAssertTrue(findMoveByFromAndto(moveList, d7, c7));
        XCTAssertTrue(findMoveByFromAndto(moveList, d7, c8));
        
        releaseMoveList(moveList);
        initMoveGenerator(&generator, &gameState->position, Black);
        addNonCastlingMoves(&generator);
        moveList = generator.moveList;
        XCTAssertEqual(3, countFrom(moveList, a8));
        XCTAssertTrue(findMoveByFromAndto(moveList, a8, b8));
        XCTAssertTrue(findMoveByFromAndto(moveList, a8, b7));
        XCTAssertTrue(findMoveByFromAndto(moveList, a8, a7));
        
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
        XCTAssertTrue(findMoveByFromAndto(moveList, c4, b6));
        XCTAssertTrue(findMoveByFromAndto(moveList, c4, e5));
        XCTAssertTrue(findMoveByFromAndto(moveList, c4, e3));
        XCTAssertTrue(findMoveByFromAndto(moveList, c4, d2));
        XCTAssertTrue(findMoveByFromAndto(moveList, c4, b2));
        XCTAssertTrue(findMoveByFromAndto(moveList, c4, a3));
        XCTAssertTrue(findMoveByFromAndto(moveList, c4, a5));
        
        releaseMoveList(moveList);
        initMoveGenerator(&generator, &gameState->position, Black);
        addNonCastlingMoves(&generator);
        moveList = generator.moveList;
        XCTAssertEqual(5, countFrom(moveList, b6));
        XCTAssertTrue(findMoveByFromAndto(moveList, b6, c8));
        XCTAssertTrue(findMoveByFromAndto(moveList, b6, d7));
        XCTAssertTrue(findMoveByFromAndto(moveList, b6, d5));
        XCTAssertTrue(findMoveByFromAndto(moveList, b6, c4));
        XCTAssertTrue(findMoveByFromAndto(moveList, b6, a4));
        
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
        XCTAssertTrue(findMoveByFromAndto(moveList, d5, e6));
        XCTAssertTrue(findMoveByFromAndto(moveList, d5, f7));
        XCTAssertTrue(findMoveByFromAndto(moveList, d5, g8));
        XCTAssertTrue(findMoveByFromAndto(moveList, d5, e4));
        XCTAssertTrue(findMoveByFromAndto(moveList, d5, f3));
        XCTAssertTrue(findMoveByFromAndto(moveList, d5, c4));
        XCTAssertTrue(findMoveByFromAndto(moveList, d5, b3));
        XCTAssertTrue(findMoveByFromAndto(moveList, d5, a2));
        
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
        XCTAssertTrue(findMoveByFromAndto(moveList, c3, c4));
        XCTAssertTrue(findMoveByFromAndto(moveList, c3, c5));
        XCTAssertTrue(findMoveByFromAndto(moveList, c3, d3));
        XCTAssertTrue(findMoveByFromAndto(moveList, c3, e3));
        XCTAssertTrue(findMoveByFromAndto(moveList, c3, f3));
        XCTAssertTrue(findMoveByFromAndto(moveList, c3, c2));
        XCTAssertTrue(findMoveByFromAndto(moveList, c3, c1));
        XCTAssertTrue(findMoveByFromAndto(moveList, c3, b3));
        XCTAssertTrue(findMoveByFromAndto(moveList, c3, a3));
        
        releaseMoveList(moveList);
    }
}

- (void)testWhitePawnMoves {
    static const char *fen = "4k3/1P6/8/5pP1/2n1b3/3P4/5P2/4K3 w - f6";
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
        
        XCTAssertTrue(findMoveByFromAndto(moveList, d3, c4));
        XCTAssertTrue(findMoveByFromAndto(moveList, d3, d4));
        XCTAssertTrue(findMoveByFromAndto(moveList, d3, e4));
        
        XCTAssertTrue(findMoveByFromAndto(moveList, f2, f3));
        XCTAssertTrue(findMoveByFromAndto(moveList, f2, f4));
        
        XCTAssertTrue(findMoveByFromAndto(moveList, g5, f6));
        XCTAssertTrue(findMoveByFromAndto(moveList, g5, g6));
        
        releaseMoveList(moveList);
    }
}


- (void)testBlackPawnMoves {
    static const char *fen = "4k3/5p2/3p4/2N1B3/5Pp1/8/1p6/4K3 b - f3";
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
        
        XCTAssertTrue(findMoveByFromAndto(moveList, d6, c5));
        XCTAssertTrue(findMoveByFromAndto(moveList, d6, d5));
        XCTAssertTrue(findMoveByFromAndto(moveList, d6, e5));
        
        XCTAssertTrue(findMoveByFromAndto(moveList, f7, f6));
        XCTAssertTrue(findMoveByFromAndto(moveList, f7, f5));
        
        XCTAssertTrue(findMoveByFromAndto(moveList, g4, f3));
        XCTAssertTrue(findMoveByFromAndto(moveList, g4, g3));

        releaseMoveList(moveList);
    }
}

typedef void (*MoveListChecker)(const MoveList *moveList);
    

static void runCastlingTest(const char *fen, Player player, MoveListChecker checker) {
    GameState *gameState = parseFen(fen);
    XCTAssertTrue(gameState != NULL);
    if (gameState) {
        MoveGenerator opponentGenerator;
        initMoveGenerator(&opponentGenerator, &gameState->position, getOpponent(player));
        addNonCastlingMoves(&opponentGenerator);
        
        MoveGenerator generator;
        initMoveGenerator(&generator, &gameState->position, player);
        addCastlingMoves(&generator, opponentGenerator.moveList);
        
        MoveList *moveList = generator.moveList;
        checker(moveList);
        releaseMoveList(moveList);
        releaseMoveList(opponentGenerator.moveList);
    }
}

static void checkWhiteCanCastleBothSides(const MoveList *moveList) {
    XCTAssertEqual(2, moveList->size);
    XCTAssertTrue(findMoveByFromAndto(moveList, e1, g1));
    XCTAssertTrue(findMoveByFromAndto(moveList, e1, c1));
}

static void checkBlackCanCastleBothSides(const MoveList *moveList) {
    XCTAssertEqual(2, moveList->size);
    XCTAssertTrue(findMoveByFromAndto(moveList, e8, g8));
    XCTAssertTrue(findMoveByFromAndto(moveList, e8, c8));
}

static void checkWhiteCanCastleLongOnly(const MoveList *moveList) {
    XCTAssertEqual(1, moveList->size);
    XCTAssertTrue(findMoveByFromAndto(moveList, e1, c1));
}

static void checkWhiteCanCastleShortOnly(const MoveList *moveList) {
    XCTAssertEqual(1, moveList->size);
    XCTAssertTrue(findMoveByFromAndto(moveList, e1, g1));
}

static void checkBlackCanCastleLongOnly(const MoveList *moveList) {
    XCTAssertEqual(1, moveList->size);
    XCTAssertTrue(findMoveByFromAndto(moveList, e8, c8));
}

static void checkBlackCanCastleShortOnly(const MoveList *moveList) {
    XCTAssertEqual(1, moveList->size);
    XCTAssertTrue(findMoveByFromAndto(moveList, e8, g8));
}

static void checkNoCastling(const MoveList *moveList) {
    XCTAssertEqual(0, moveList->size);
}

- (void)testWhiteUnrestrictedCastling {
    static const char *fen = "4k3/8/8/8/8/8/8/R3K2R w QK";
    runCastlingTest(fen, White, checkWhiteCanCastleBothSides);
}

- (void)testBlackUnrestrictedCastling {
    static const char *fen = "r3k2r/8/8/8/8/8/8/4K3 b qk";
    runCastlingTest(fen, Black, checkBlackCanCastleBothSides);
}

- (void)testWhiteAllowedLongCastlingOnly {
    static const char *fen = "4k3/8/8/8/8/8/8/R3K2R w Q";
    runCastlingTest(fen, White, checkWhiteCanCastleLongOnly);
}

- (void)testBlackAllowedLongCastlingOnly {
    static const char *fen = "r3k2r/8/8/8/8/8/8/4K3 b q";
    runCastlingTest(fen, Black, checkBlackCanCastleLongOnly);
}

- (void)testWhiteAllowedShortCastlingOnly {
    static const char *fen = "4k3/8/8/8/8/8/8/R3K2R w K";
    runCastlingTest(fen, White, checkWhiteCanCastleShortOnly);
}

- (void)testBlackAllowedShortCastlingOnly {
    static const char *fen = "r3k2r/8/8/8/8/8/8/4K3 b k";
    runCastlingTest(fen, Black, checkBlackCanCastleShortOnly);
}

- (void)testWhiteNoCastlingAllowed {
    static const char *fen = "4k3/8/8/8/8/8/8/R3K2R w -";
    runCastlingTest(fen, White, checkNoCastling);
}

- (void)testBlackNoCastlingAllowed {
    static const char *fen = "r3k2r/8/8/8/8/8/8/4K3 b -";
    runCastlingTest(fen, Black, checkNoCastling);
}

- (void)testWhiteCastlingBlockedByIntermediatePiece {
    static const char *fen = "4k3/8/8/8/8/8/8/Rb2KB1R w QK";
    runCastlingTest(fen, White, checkNoCastling);
}

- (void)testBlackCastlingBlockedByIntermediatePiece {
    static const char *fen = "rn2kb1r/8/8/8/8/8/8/4K3 b -";
    runCastlingTest(fen, Black, checkNoCastling);
}

- (void)testWhiteCannotCastleWhenInCheck {
    static const char *fen = "4k3/8/8/8/8/3n4/8/R3K2R w QK";
    runCastlingTest(fen, White, checkNoCastling);
}

- (void)testBlackCannotCastleWhenInCheck {
    static const char *fen = "r3k2r/8/3N4/8/8/8/8/4K3 b qk";
    runCastlingTest(fen, Black, checkNoCastling);
}

- (void)testWhiteCastlingCannotCrossAttackedSquare {
    static const char *fen = "4k3/8/8/8/8/3q4/3P4/R3K2R w QK";
    runCastlingTest(fen, White, checkWhiteCanCastleLongOnly);
}

- (void)testBlackCastlingCannotCrossAttackedSquare {
    static const char *fen = "r3k2r/3p4/3Q4/8/8/8/8/4K3 b qk";
    runCastlingTest(fen, Black, checkBlackCanCastleLongOnly);
}

@end
