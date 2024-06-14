//
//  PublicTests.m
//  ChessEngineTests
//
//  Created by Tony on 14/06/2024.
//

#import <XCTest/XCTest.h>

#include "Public.h"
#include "GameState.h"
#include "BoardGeometry.h"

const char *fen = "4k3/1P6/8/8/8/8/6q1/R3K3 w Q - 100 0";

@interface PublicTests : XCTestCase

@end

@implementation PublicTests

- (void)setUp {
    // Put setup code here. This method is called before the invocation of each test method in the class.
}

- (void)tearDown {
    // Put teardown code here. This method is called after the invocation of each test method in the class.
}

- (void)testStartGame {
    EngPosition *position = engParseFen(fen, NULL);
    struct EngGame *game = engStartGame(position);
    EngPosition *result = engGetCurrentPosition(game);
    XCTAssertTrue(result->whiteCanCastleLong);
    XCTAssertFalse(result->whiteCanCastleShort);
    XCTAssertFalse(result->blackCanCastleLong);
    XCTAssertFalse(result->blackCanCastleShort);
    XCTAssertEqual(White, result->playerToMove);
    for (int i = 0; i < 64; i++) {
        Piece piece = result->board[i];
        switch (i) {
            case a1:
                XCTAssertEqual(WhiteRook, piece);
                break;
            case e1:
                XCTAssertEqual(WhiteKing, piece);
                break;
            case g2:
                XCTAssertEqual(BlackQueen, piece);
                break;
            case b7:
                XCTAssertEqual(WhitePawn, piece);
                break;
            case e8:
                XCTAssertEqual(BlackKing, piece);
                break;
            default:
                XCTAssertEqual(NoPiece, piece);
                break;
        }
    }
    
    engFreeGame(game);
    engFreePosition(position);
    engFreePosition(result);
}

- (void)testEngGetTargets {
    EngPosition *position = engParseFen(fen, NULL);
    struct EngGame *game = engStartGame(position);
    SquareMask result = engGetTargets(game, e1);
    for (int i = 0; i < 64; i++) {
        SquareMask m = ((SquareMask)1 << i) & result;
        switch (i) {
            case d1:
            case c1:
                XCTAssertFalse(m == 0);
                break;
            default:
                XCTAssertTrue(m == 0);
                break;
        }
    }
        
    engFreeGame(game);
    engFreePosition(position);
}

@end
