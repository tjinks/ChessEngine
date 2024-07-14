//
//  AnalysisDataTests.m
//  ChessEngineTests
//
//  Created by Tony on 02/06/2024.
//

#import <XCTest/XCTest.h>

#include "TestSupport.h"
#include "GameState.h"

static const char *moveTestsFen = "r2q1rk1/pPp3pp/4p3/4Pp2/5P2/8/PPP3PP/RN1QK2R w QK f6 50 5";

@interface GameStateTests : XCTestCase

@end

@implementation GameStateTests

- (void)setUp {
    // Put setup code here. This method is called before the invocation of each test method in the class.
}

- (void)tearDown {
    // Put teardown code here. This method is called after the invocation of each test method in the class.
}

- (void)testMakeNormalMove {
    GameState *initial = parseFen(moveTestsFen);
    Move move = {2, 64, NoPiece, 0, { {1, NoPiece}, {18, WhiteKnight} } };
    GameState *afterMove = makeMove(initial, move);
    for (int i = 0; i < 64; i++) {
        switch (i) {
            case 1:
                XCTAssertEqual(NoPiece, afterMove->position.board[i]);
                break;
            case 18:
                XCTAssertEqual(WhiteKnight, afterMove->position.board[i]);
                break;
            default:
                XCTAssertTrue(afterMove->position.board[i] == initial->position.board[i]);
                break;
        }
    }
    
    XCTAssertEqual(WhiteShort | WhiteLong, afterMove->position.castlingFlags);
    XCTAssertEqual(49, afterMove->halfMoveClock);
    XCTAssertFalse(afterMove->isRepetitionBarrier);
    releaseGameState(afterMove);
    releaseGameState(initial);
}

- (void)testChangeInCastlingFlags {
    GameState *initial = parseFen(moveTestsFen);
    Move move = {2, 64, NoPiece, 0, { {7, NoPiece}, {5, WhiteRook} } };
    GameState *afterMove = makeMove(initial, move);
    for (int i = 0; i < 64; i++) {
        switch (i) {
            case 7:
                XCTAssertEqual(NoPiece, afterMove->position.board[i]);
                break;
            case 5:
                XCTAssertEqual(WhiteRook, afterMove->position.board[i]);
                break;
            default:
                XCTAssertTrue(afterMove->position.board[i] == initial->position.board[i]);
                break;
        }
    }
    
    XCTAssertEqual(WhiteLong, afterMove->position.castlingFlags);
    XCTAssertEqual(49, afterMove->halfMoveClock);
    XCTAssertTrue(afterMove->isRepetitionBarrier);
    releaseGameState(afterMove);
    releaseGameState(initial);
}

- (void)testCapture {
    GameState *initial = parseFen(moveTestsFen);
    Move move = {2, 64, NoPiece, 0, { {3, NoPiece}, {59, WhiteQueen} } };
    GameState *afterMove = makeMove(initial, move);
    for (int i = 0; i < 64; i++) {
        switch (i) {
            case 3:
                XCTAssertEqual(NoPiece, afterMove->position.board[i]);
                break;
            case 59:
                XCTAssertEqual(WhiteQueen, afterMove->position.board[i]);
                break;
            default:
                XCTAssertTrue(afterMove->position.board[i] == initial->position.board[i]);
                break;
        }
    }
    
    XCTAssertEqual(WhiteShort | WhiteLong, afterMove->position.castlingFlags);
    XCTAssertEqual(100, afterMove->halfMoveClock);
    XCTAssertTrue(afterMove->isRepetitionBarrier);
    releaseGameState(afterMove);
    releaseGameState(initial);
}

- (void)testPawnMove {
    GameState *initial = parseFen(moveTestsFen);
    Move move = {2, 64, NoPiece, 0, { {3, NoPiece}, {59, WhiteQueen} } };
    GameState *afterMove = makeMove(initial, move);
    for (int i = 0; i < 64; i++) {
        switch (i) {
            case 3:
                XCTAssertEqual(NoPiece, afterMove->position.board[i]);
                break;
            case 59:
                XCTAssertEqual(WhiteQueen, afterMove->position.board[i]);
                break;
            default:
                XCTAssertTrue(afterMove->position.board[i] == initial->position.board[i]);
                break;
        }
    }
    
    XCTAssertEqual(WhiteShort | WhiteLong, afterMove->position.castlingFlags);
    XCTAssertEqual(100, afterMove->halfMoveClock);
    XCTAssertTrue(afterMove->isRepetitionBarrier);
    releaseGameState(afterMove);
    releaseGameState(initial);
}

- (void)testPromotion {
    GameState *initial = parseFen(moveTestsFen);
    Move move = {2, 64, WhiteBishop, 0, { {49, NoPiece}, {57, WhiteBishop} } };
    GameState *afterMove = makeMove(initial, move);
    for (int i = 0; i < 64; i++) {
        switch (i) {
            case 49:
                XCTAssertEqual(NoPiece, afterMove->position.board[i]);
                break;
            case 57:
                XCTAssertEqual(WhiteBishop, afterMove->position.board[i]);
                break;
            default:
                XCTAssertTrue(afterMove->position.board[i] == initial->position.board[i]);
                break;
        }
    }
    
    XCTAssertEqual(WhiteShort | WhiteLong, afterMove->position.castlingFlags);
    XCTAssertEqual(100, afterMove->halfMoveClock);
    XCTAssertTrue(afterMove->isRepetitionBarrier);
    releaseGameState(afterMove);
    releaseGameState(initial);
}

- (void)testEpCapture {
    GameState *initial = parseFen(moveTestsFen);
    Move move = {3, 64, NoPiece, 0, { {36, NoPiece}, {45, WhitePawn}, {37, NoPiece} } };
    GameState *afterMove = makeMove(initial, move);
    for (int i = 0; i < 64; i++) {
        switch (i) {
            case 36:
                XCTAssertEqual(NoPiece, afterMove->position.board[i]);
                break;
            case 45:
                XCTAssertEqual(WhitePawn, afterMove->position.board[i]);
                break;
            case 37:
                XCTAssertEqual(NoPiece, afterMove->position.board[i]);
                break;
            default:
                XCTAssertTrue(afterMove->position.board[i] == initial->position.board[i]);
                break;
        }
    }
    
    XCTAssertEqual(WhiteShort | WhiteLong, afterMove->position.castlingFlags);
    XCTAssertEqual(100, afterMove->halfMoveClock);
    XCTAssertTrue(afterMove->isRepetitionBarrier);
    releaseGameState(afterMove);
    releaseGameState(initial);
}

- (void)testCastling {
    GameState *initial = parseFen(moveTestsFen);
    Move move = {4, 64, NoPiece, 0, { {4, NoPiece}, {6, WhiteKing}, {7, NoPiece}, {5, WhiteRook} } };
    GameState *afterMove = makeMove(initial, move);
    for (int i = 0; i < 64; i++) {
        switch (i) {
            case 4:
                XCTAssertEqual(NoPiece, afterMove->position.board[i]);
                break;
            case 6:
                XCTAssertEqual(WhiteKing, afterMove->position.board[i]);
                break;
            case 7:
                XCTAssertEqual(NoPiece, afterMove->position.board[i]);
                break;
            case 5:
                XCTAssertEqual(WhiteRook, afterMove->position.board[i]);
                break;
            default:
                XCTAssertTrue(afterMove->position.board[i] == initial->position.board[i]);
                break;
        }
    }
    
    XCTAssertEqual(0, afterMove->position.castlingFlags);
    XCTAssertEqual(49, afterMove->halfMoveClock);
    XCTAssertTrue(afterMove->isRepetitionBarrier);
    releaseGameState(afterMove);
    releaseGameState(initial);
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

- (void)testPositionHashPlayerToMove {
    GameState *gameState = parseFen("5r2/8/5p2/5P2/4R2K/8/6k1/8 w");
    XCTAssertEqual(0, gameState->position.hash & 1);
    gameState = parseFen("5r2/8/5p2/5P2/4R2K/8/6k1/8 b");
    XCTAssertEqual(1, gameState->position.hash & 1);
}

- (void)testPositionHashUpdate {
    GameState *initial = parseFen("5r2/8/5p2/5P2/4R2K/8/6k1/8 w");
    
    Move m1 = {2, 64, NoPiece, 0, { {31, NoPiece}, {30, WhiteKing} } };
    GameState *afterMove1 = makeMove(initial, m1);
    
    Move m2 = {2, 64, NoPiece, 0, { {61, NoPiece}, {53, BlackRook} } };
    GameState *afterMove2 = makeMove(afterMove1, m2);
    
    Move m3 = {2, 64, NoPiece, 0, { {28, NoPiece}, {27, WhiteRook} } };
    GameState *afterMove3 = makeMove(afterMove2, m3);
    
    Move m4 = {2, 64, NoPiece, 0, { {14, NoPiece}, {13, BlackKing} } };
    GameState *afterMove4 = makeMove(afterMove3, m4);
    
    Move m5 = {2, 64, NoPiece, 0, { {30, NoPiece}, {31, WhiteKing} } };
    GameState *afterMove5 = makeMove(afterMove4, m5);
    
    Move m6 = {2, 64, NoPiece, 0, { {13, NoPiece}, {14, BlackKing} } };
    GameState *afterMove6 = makeMove(afterMove5, m6);
    
    Move m7 = {2, 64, NoPiece, 0, { {27, NoPiece}, {28, WhiteRook} } };
    GameState *afterMove7 = makeMove(afterMove6, m7);
    
    Move m8 = {2, 64, NoPiece, 0, { {53, NoPiece}, {61, BlackRook} } };
    GameState *afterMove8 = makeMove(afterMove7, m8);

    XCTAssertTrue(initial->position.hash == afterMove8->position.hash);
    
    releaseGameState(afterMove8);
    releaseGameState(afterMove7);
    releaseGameState(afterMove6);
    releaseGameState(afterMove5);
    releaseGameState(afterMove4);
    releaseGameState(afterMove3);
    releaseGameState(afterMove2);
    releaseGameState(afterMove1);
    releaseGameState(initial);
}

- (void)testIsSamePosition {
    GameState *gs1 = parseFen("8/8/8/4Pp2/7K/8/5k2/8 w - -");
    GameState *gs2 = parseFen("8/8/8/4Pp2/7K/8/5k2/8 w - f6");
    XCTAssertTrue(isSamePosition(gs1, gs1));
    XCTAssertTrue(isSamePosition(gs2, gs2));
    XCTAssertFalse(isSamePosition(gs1, gs2));
}

@end
