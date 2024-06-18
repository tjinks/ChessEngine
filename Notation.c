//
//  Notation.c
//  ChessEngine
//
//  Created by Tony on 13/06/2024.
//

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#include "Public.h"
#include "EngCommon.h"

static const char *TooManyRanks = "Too many ranks";
static const char *TooFewRanks = "Too few ranks";
static const char *TooManyFiles = "Too many files";
static const char *InvalidPiece = "Invalid piece '(%c)'";
static const char *InvalidPlayer = "Invalid player '(%s)'";
static const char *InvalidSquare = "Invalid square '(%s)'";
static const char *InvalidCastlingRights = "Invalid castling rights '(%s)'";
static const char *InvalidHalfMoveClock = "Invalid half move clock '(%s)'";
static const char *InvalidMoveNumber = "Invalid move number '(%s)'";

static const int MaxErrorMessageSize = 255;

typedef struct {
    EngPosition *result;
    char errorMessage[MaxErrorMessageSize + 1];
} FenParsingContext;

typedef const char *StringPtr;

static const char *copyErrorMessage(const FenParsingContext *ctx) {
    char *result = getMem(strlen(ctx->errorMessage) + 1);
    return strcpy(result, ctx->errorMessage);
}

static const char *nextToken(StringPtr *s) {
    while (**s && isspace(**s)) {
        (*s)++;
    }
    
    if (!**s) {
        return NULL;
    }
    
    const char *start = *s;
    while (**s && !isspace(**s)) {
        (*s)++;
    }
    
    int len = (int)(*s - start);
    char *result = getMem(len + 1);
    memcpy(result, start, len);
    result[len] = '\0';
    return result;
}

static EngPiece fenCharToPiece(char c) {
    switch (c) {
        case 'K':
            return WhiteKing;
        case 'k':
            return  BlackKing;
        case 'Q':
            return WhiteQueen;
        case 'q':
            return BlackQueen;
        case 'R':
            return WhiteRook;
        case 'r':
            return BlackRook;
        case 'B':
            return WhiteBishop;
        case 'b':
            return BlackBishop;
        case 'N':
            return WhiteKnight;
        case 'n':
            return BlackKnight;
        case 'P':
            return WhitePawn;
        case 'p':
            return BlackPawn;
        default:
            return NoPiece;
    }
}

static bool parseBoard(FenParsingContext *ctx, const char *encoding) {
    for (int i = 0; i < 64; i++) ctx->result->board[i] = NoPiece;
    int rank = 7;
    int file = 0;
    int index = 0;
    for (;;) {
        char c = encoding[index++];
        if (c == '\0') {
            if (rank > 0) {
                strcpy(ctx->errorMessage, TooFewRanks);
                return false;
            }
            
            return true;
        } else if (c == '/') {
            rank--;
            if (rank < 0) {
                strcpy(ctx->errorMessage, TooManyRanks);
                return false;
            }
            
            file = 0;
        } else if (isdigit(c)) {
            file += c - '0';
            if (file > 8) {
                strcpy(ctx->errorMessage, TooManyFiles);
                return false;
            }
        } else {
            EngPiece piece = fenCharToPiece(c);
            if (piece == NoPiece) {
                sprintf(ctx->errorMessage, InvalidPiece, c);
                return false;
            }
            
            if (file >= 8) {
                strcpy(ctx->errorMessage, TooManyFiles);
                return false;
            }
            
            ctx->result->board[8 * rank + file] = piece;
            file++;
        }
    }
}

static bool parsePlayerToMove(FenParsingContext *ctx, const char *encoding) {
    if (strlen(encoding) != 1) {
        goto errorExit;
    }
    
    switch (*encoding) {
        case 'W':
        case 'w':
            ctx->result->playerToMove = White;
            break;
        case 'B':
        case 'b':
            ctx->result->playerToMove = White;
            break;
        default:
            goto errorExit;
    }

    return true;
    
errorExit:
    sprintf(ctx->errorMessage, InvalidPlayer, encoding);
    return false;
}

static bool parseCastlingRights(FenParsingContext *ctx, const char *encoding) {
    ctx->result->whiteCanCastleLong = false;
    ctx->result->blackCanCastleLong = false;
    ctx->result->whiteCanCastleShort = false;
    ctx->result->blackCanCastleShort = false;
    if (encoding == NULL || strcmp(encoding, "-") == 0) {
        return true;
    }

    if (strlen(encoding) > 4) {
        goto errorExit;
    }
    
    for (int i = 0; encoding[i]; i++) {
        switch (encoding[i]) {
            case 'K':
                ctx->result->whiteCanCastleShort = true;
                break;
            case 'k':
                ctx->result->blackCanCastleShort = true;
                break;
            case 'Q':
                ctx->result->whiteCanCastleLong = true;
                break;
            case 'q':
                ctx->result->blackCanCastleLong = true;
                break;
            default:
                goto errorExit;
        }
    }
    
    return true;
    
errorExit:
    sprintf(ctx->errorMessage, InvalidCastlingRights, encoding);
    return false;
}

static int parseSquare(const char *square) {
    if (strlen(square) != 2) {
        return EngNoSquare;
    }
    
    int file = (int)square[0] - (int)'a';
    int rank = (int)square[1] - (int)'1';
    if (file < 0 || file > 7 || rank < 0 || rank > 7) {
        return EngNoSquare;
    }
    
    return file + 8 * rank;
}

static bool parseEpSquare(FenParsingContext *ctx, const char *encoding) {
    ctx->result->epSquare = EngNoSquare;
    if (encoding == NULL || strcmp(encoding, "-") == 0) {
        return true;
    }
    
    int epSquare = parseSquare(encoding);
    if (epSquare == EngNoSquare) {
        sprintf(ctx->errorMessage, InvalidSquare, encoding);
        return false;
    }
    
    ctx->result->epSquare = epSquare;
    return true;
}

static int parseNumber(const char *encoding) {
    char *endptr;
    long result = strtol(encoding, &endptr, 10);
    if (*endptr) {
        return -1;
    }
    
    if (result < 0) {
        return -1;
    }
    
    return (int)result;
}

static bool parseHalfMoveClock(FenParsingContext *ctx, const char *encoding) {
    int halfMoveClock = 100;
    if (encoding) {
        halfMoveClock = parseNumber(encoding);
        if (halfMoveClock < 0 || halfMoveClock > 100) {
            sprintf(ctx->errorMessage, InvalidHalfMoveClock, encoding);
            return false;
        }
    }
    
    ctx->result->halfMoveClock = halfMoveClock;
    return true;
}

static bool parseMoveNumber(FenParsingContext *ctx, const char *encoding) {
    int moveNumber = 1;
    if (encoding) {
        moveNumber = parseNumber(encoding);
        if (moveNumber < 0) {
            sprintf(ctx->errorMessage, InvalidMoveNumber, encoding);
            return false;
        }
    }
    
    ctx->result->moveNumber = moveNumber;
    return true;
}

EngParseFenResult engParseFen(const char *fen) {
    FenParsingContext ctx;
    ctx.result = engCreatePosition();
    ctx.errorMessage[0] = '\0';
    const char *boardEncoding = nextToken(&fen);
    const char *playerToMoveEncoding = nextToken(&fen);
    const char *castlingRightsEncoding = nextToken(&fen);
    const char *epSquareEncoding = nextToken(&fen);
    const char *halfMoveClockEncoding = nextToken(&fen);
    const char *moveNumberEncoding = nextToken(&fen);
    
    bool success = parseBoard(&ctx, boardEncoding);
    success = success && parsePlayerToMove(&ctx, playerToMoveEncoding);
    success = success && parseCastlingRights(&ctx, castlingRightsEncoding);
    success = success && parseEpSquare(&ctx, epSquareEncoding);
    success = success && parseHalfMoveClock(&ctx, halfMoveClockEncoding);
    success = success && parseMoveNumber(&ctx, moveNumberEncoding);
    
    EngParseFenResult result = {success};
    if (success) {
        result.position = ctx.result;
    } else {
        engFreePosition(ctx.result);
        result.errorMessage = copyErrorMessage(&ctx);
    }
    
    return result;
}

