//
//  TestSupport.c
//  ChessEngineTests
//
//  Created by Tony on 27/05/2024.
//

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include "TestSupport.h"
#include "BoardGeometry.h"

static const char *ws = " \t";

static Piece charToPiece(char c) {
    switch (c) {
        case 'P':
            return WhitePawn;
        case 'p':
            return BlackPawn;
        case 'N':
            return WhiteKnight;
        case 'n':
            return BlackKnight;
        case 'B':
            return WhiteBishop;
        case 'b':
            return BlackBishop;
        case 'R':
            return WhiteRook;
        case 'r':
            return BlackRook;
        case 'Q':
            return WhiteQueen;
        case 'q':
            return BlackQueen;
        case 'K':
            return WhiteKing;
        case 'k':
            return BlackKing;
        default:
            return NoPiece;
    }
}

static Player decodePlayerToMove(const char *encoding) {
    switch (encoding[0]) {
        case 'W':
        case 'w':
            return White;
        case 'B':
        case 'b':
            return Black;
        default:
            return NoPlayer;
    }
}

static bool decodeBoard(const char *encoding, Piece *board) {
    for (int i = 0; i < 64; i++) {
        board[i] = NoPiece;
    }
    
    int rank = 7, file = 0;
    for (int i = 0; encoding[i]; i++) {
        char c = encoding[i];
        Piece piece = charToPiece(c);
        if (piece != NoPiece) {
            if (rank < 0 || file > 7) {
                return false;
            }
            
            board[rank * 8 + file] = charToPiece(c);
            file++;
        } else if (c == '/') {
            if (--rank < 0) {
                return false;
            }
            file = 0;
        } else {
            int inc = c - '0';
            if (inc < 1) {
                return false;
            }
            
            file += inc;
            if (file > 8) {
                return false;
            }
        }
    }
    
    return true;
}

static bool decodeSquare(const char *encoding, int *result) {
    if (strcmp(encoding, "-") == 0) {
        *result = NoSquare;
        return true;
    }
    
    if (strlen(encoding) != 2) {
        return false;
    }
    
    int file = encoding[0] - 'a';
    int rank = encoding[1] - '1';
    if (file < 0 || file > 7 || rank < 0 || rank > 7) {
        return false;
    }
    
    *result = file + 8 * rank;
    return true;
}

static bool decodeCastlingFlags(const char *encoding, CastlingFlags *flags) {
    if (strlen(encoding) != 4) {
        return false;
    }
    
    *flags = 0;
    for (int i = 0; i < 4; i++) {
        switch (encoding[i]) {
            case 'Q':
                *flags |= WhiteLong;
                break;
            case 'K':
                *flags |= WhiteShort;
                break;
            case 'q':
                *flags |= BlackLong;
                break;
            case 'k':
                *flags |= BlackShort;
                break;
            case '-':
                break;
            default: 
                return false;
        }
    }
    
    return true;
}

static int decodeNumber(const char *encoding) {
    char *endptr;
    long result = strtol(encoding, &endptr, 10);
    if (*endptr != '\0' && *endptr != ' ' && *endptr != '\t') {
        return -1;
    }
    
    if (result == 0 || result == LONG_MAX || result == LONG_MIN) {
        return -1;
    }
    
    return (int)result;
}

static void setKingSquares(Position *position) {
    position->kingSquare[White] = NoSquare;
    position->kingSquare[Black] = NoSquare;
    for (int i = 0; i < 64; i++) {
        switch (position->board[i]) {
            case WhiteKing:
                position->kingSquare[White] = i;
                break;
            case BlackKing:
                position->kingSquare[Black] = i;
                break;
            default:
                break;
        }
    }
}

GameState *parseFen(const char *fen) {
    GameState *result = acquireGameState();
    char *fenCopy = strdup(fen);
    if (fenCopy) {
        char *boardEncoding = strtok(fenCopy, ws);
        char *playerToMoveEncoding = strtok(NULL, ws);
        char *castlingFlagsEncoding = strtok(NULL, ws);
        char *epSquareEncoding = strtok(NULL, ws);
        char *halfMoveClockEncoding = strtok(NULL, ws);
        char *fullMoveEncoding = strtok(NULL, ws);
  
        
        if (boardEncoding) {
            if (!decodeBoard(boardEncoding, result->position.board)) goto errExit;
        }
        
        if (playerToMoveEncoding) {
            result->position.playerToMove = decodePlayerToMove(playerToMoveEncoding);
            if (result->position.playerToMove == NoPlayer) goto errExit;
        } else {
            result->position.playerToMove = White;
        }
        
        if (castlingFlagsEncoding) {
            if (!decodeCastlingFlags(castlingFlagsEncoding, &result->position.castlingFlags)) goto errExit;
        } else {
            result->position.castlingFlags = 0;
        }
        
        if (epSquareEncoding) {
            if (!decodeSquare(epSquareEncoding, &result->position.epSquare)) goto errExit;
        } else {
            result->position.epSquare = NoSquare;
        }
        
        if (halfMoveClockEncoding) {
            result->halfMoveClock = decodeNumber(halfMoveClockEncoding);
            if (result->halfMoveClock == -1) goto errExit;
        } else {
            result->halfMoveClock = 0;
        }
        
        if (fullMoveEncoding) {
            result->moveNumber = decodeNumber(fullMoveEncoding);
            if (result->moveNumber == -1) goto errExit;
        } else {
            result->moveNumber = 1;
        }
    }
    
    setKingSquares(&result->position);
    free(fenCopy);
    return result;
errExit:
    releaseGameState(result);
    free(fenCopy);
    return NULL;
}
