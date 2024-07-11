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
    EngParseFenResult parseFenResult = engParseFen(fen);
    EngPosition *position = parseFenResult.position;
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
    EngParseFenResult parseFenResult = engParseFen(fen);
    EngPosition *position = parseFenResult.position;
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

- (void)testEngGetResultNone {
    const char *fen = "rnb1kb1r/ppppqppp/8/4n3/2P2B2/5N2/PP1NPPPP/R2QKB1R b KQkq - 100 0";
    EngParseFenResult parseFenResult = engParseFen(fen);
    struct EngGame *game = engStartGame(parseFenResult.position);
    
    EngGameResult gameResult = engGetResult(game);
    XCTAssertEqual(NoResult, gameResult);

    engFreeGame(game);
    engFreePosition(parseFenResult.position);
}

- (void)testEngGetResultCheckmate {
    const char *fen = "rnb1kb1r/ppppqppp/8/8/2P2B2/3n1N2/PP1NPPPP/R2QKB1R w KQkq - 100 0";
    EngParseFenResult parseFenResult = engParseFen(fen);
    struct EngGame *game = engStartGame(parseFenResult.position);
    
    EngGameResult gameResult = engGetResult(game);
    XCTAssertEqual(WinByCheckmate, gameResult);

    engFreeGame(game);
    engFreePosition(parseFenResult.position);
}

- (void)testEngGetResultStalemate {
    const char *fen = "8/8/8/3b4/6n1/8/6R1/5k1K w - - 100 0";
    EngParseFenResult parseFenResult = engParseFen(fen);
    struct EngGame *game = engStartGame(parseFenResult.position);
    
    EngGameResult gameResult = engGetResult(game);
    XCTAssertEqual(DrawByStalemate, gameResult);

    engFreeGame(game);
    engFreePosition(parseFenResult.position);
}

@end
