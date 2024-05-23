//
//  EngPieceTests.m
//  ChessBETests
//
//  Created by Tony on 17/05/2024.
//

#import <XCTest/XCTest.h>

#include "Piece.h"

@interface EngPieceTests : XCTestCase

@end

@implementation EngPieceTests

- (void)setUp {
    // Put setup code here. This method is called before the invocation of each test method in the class.
}

- (void)tearDown {
    // Put teardown code here. This method is called after the invocation of each test method in the class.
}

- (void)testGetPiece {
    Piece piece1 = makePiece(White, Rook);
    Piece piece2 = makePiece(Black, King);
    Piece piece3 = makePiece(NoPlayer, NoPieceType);
    XCTAssertEqual(WhiteRook, piece1);
    XCTAssertEqual(BlackKing, piece2);
    XCTAssertEqual(NoPiece, piece3);
    
    PieceType pieceType1 = getPieceType(piece1);
    Player player1 = getOwner(piece1);
    XCTAssertEqual(Rook, pieceType1);
    XCTAssertEqual(White, player1);

    PieceType pieceType2 = getPieceType(piece2);
    Player player2 = getOwner(piece2);
    XCTAssertEqual(King, pieceType2);
    XCTAssertEqual(Black, player2);

    PieceType pieceType3 = getPieceType(piece3);
    Player player3 = getOwner(piece3);
    XCTAssertEqual(NoPieceType, pieceType3);
    XCTAssertEqual(NoPlayer, player3);
}

@end
