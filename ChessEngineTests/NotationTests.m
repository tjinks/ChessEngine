//
//  NotationTests.m
//  ChessEngineTests
//
//  Created by Tony on 13/06/2024.
//

#import <XCTest/XCTest.h>

#include "Public.h"
#include "BoardGeometry.h"

@interface NotationTests : XCTestCase

@end

@implementation NotationTests

- (void)setUp {
}

- (void)tearDown {
}

- (void)testValidFenNoEmptyFields {
    const char *fen = "r3k2r/8/8/8/P7/8/8/R3K2R w KQkq a3 25 10";
    EngPosition *result = engParseFen(fen, NULL);
    XCTAssertTrue(result != NULL);
    if (result == NULL) {
        return;
    }
    
    for (int i = 0; i <  64; i++) {
        EngPiece piece = result->board[i];
        switch (i) {
            case a1:
            case h1:
                XCTAssertEqual(WhiteRook, piece);
                break;
            case e1:
                XCTAssertEqual(WhiteKing, piece);
                break;
            case a4:
                XCTAssertEqual(WhitePawn, piece);
                break;
            case a8:
            case h8:
                XCTAssertEqual(BlackRook, piece);
                break;
            case e8:
                XCTAssertEqual(BlackKing, piece);
                break;
            default:
                XCTAssertEqual(NoPiece, piece);
                break;
        }
    }
    
    XCTAssertEqual(White, result->playerToMove);
    
    XCTAssertTrue(result->blackCanCastleLong);
    XCTAssertTrue(result->blackCanCastleShort);
    XCTAssertTrue(result->whiteCanCastleLong);
    XCTAssertTrue(result->whiteCanCastleShort);

    XCTAssertEqual(a3, result->epSquare);
    
    XCTAssertEqual(25, result->halfMoveClock);
    XCTAssertEqual(10, result->moveNumber);
    
    engFreePosition(result);
}

- (void)testValidNoCastlingAllowedNoEpSquare {
    const char *fen = "r3k2r/8/8/8/P7/8/8/R3K2R w - - 25 10";
    EngPosition *result = engParseFen(fen, NULL);
    XCTAssertTrue(result != NULL);
    if (result == NULL) {
        return;
    }

    XCTAssertFalse(result->blackCanCastleLong);
    XCTAssertFalse(result->blackCanCastleShort);
    XCTAssertFalse(result->whiteCanCastleLong);
    XCTAssertFalse(result->whiteCanCastleShort);

    XCTAssertEqual(EngNoSquare, result->epSquare);
 
    engFreePosition(result);

}

@end
