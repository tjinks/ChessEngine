//
//  PositionDto.h
//  ChessEngine
//
//  Created by Tony on 24/05/2024.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface EngPositionDto : NSObject

@property NSInteger playerToMove;
@property NSInteger epSquare;
@property NSInteger halfMoveClock;
@property NSInteger moveNumber;
@property BOOL whiteCanCastleLong;
@property BOOL whiteCanCastleShort;
@property BOOL blackCanCastleLong;
@property BOOL blackCanCastleShort;
@property NSString *board;

@end


@interface ChessGameGame : NSObject
-(id) initWithPosition:(EngPositionDto*) position;
@end


NS_ASSUME_NONNULL_END
