//
//  BoardGeometryTests.m
//  ChessEngineTests
//
//  Created by Tony on 23/05/2024.
//

#import <XCTest/XCTest.h>

#include "BoardGeometry.h"

@interface BoardGeometryTests : XCTestCase

@end

@implementation BoardGeometryTests

- (void)setUp {
    // Put setup code here. This method is called before the invocation of each test method in the class.
}

- (void)tearDown {
    // Put teardown code here. This method is called after the invocation of each test method in the class.
}

- (void)testDistance {
    XCTAssertEqual(2, distance(a1, c2));
    XCTAssertEqual(2, distance(c2, a1));
    XCTAssertEqual(3, distance(b2, c5));
    XCTAssertEqual(3, distance(c5, b2));
}

- (void)testKingsMovesFrom {
    SquareList result;
    result = kingMovesFrom(e4);
    XCTAssertEqual(8, result.count);
    XCTAssertEqual(e5, result.squares[0]);
    XCTAssertEqual(e3, result.squares[1]);
    XCTAssertEqual(f4, result.squares[2]);
    XCTAssertEqual(d4, result.squares[3]);
    XCTAssertEqual(f5, result.squares[4]);
    XCTAssertEqual(f3, result.squares[5]);
    XCTAssertEqual(d5, result.squares[6]);
    XCTAssertEqual(d3, result.squares[7]);
}

- (void)testKnightMovesFrom {
    SquareList result;
    result = knightMovesFrom(e4);
    XCTAssertEqual(8, result.count);
    XCTAssertEqual(f6, result.squares[0]);
    XCTAssertEqual(g5, result.squares[1]);
    XCTAssertEqual(g3, result.squares[2]);
    XCTAssertEqual(f2, result.squares[3]);
    XCTAssertEqual(d2, result.squares[4]);
    XCTAssertEqual(c3, result.squares[5]);
    XCTAssertEqual(c5, result.squares[6]);
    XCTAssertEqual(d6, result.squares[7]);
}

- (void)testDirections {
    int result;
    result = north(b7);
    XCTAssertEqual(b8, result);
    result = north(b8);
    XCTAssertEqual(NoSquare, result);
    
    result = south(b1);
    XCTAssertEqual(NoSquare, result);
    result = south(b2);
    XCTAssertEqual(b1, result);
    
    result = east(h5);
    XCTAssertEqual(NoSquare, result);
    result = east(g5);
    XCTAssertEqual(h5, result);
    
    result = west(a2);
    XCTAssertEqual(NoSquare, result);
    result = west(b2);
    XCTAssertEqual(a2, result);
    
    result = ne(g8);
    XCTAssertEqual(NoSquare, result);
    result = ne(h7);
    XCTAssertEqual(NoSquare, result);
    result = ne(g7);
    XCTAssertEqual(h8, result);
    
    result = se(h2);
    XCTAssertEqual(NoSquare, result);
    result = se(g1);
    XCTAssertEqual(NoSquare, result);
    result = se(g2);
    XCTAssertEqual(h1, result);
    
    result = nw(b8);
    XCTAssertEqual(NoSquare, result);
    result = nw(a7);
    XCTAssertEqual(NoSquare, result);
    result = nw(b7);
    XCTAssertEqual(a8, result);
    
    result = sw(b1);
    XCTAssertEqual(NoSquare, result);
    result = sw(a2);
    XCTAssertEqual(NoSquare, result);
    result = sw(b2);
    XCTAssertEqual(a1, result);
}

@end
