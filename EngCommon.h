//
//  EngCommon.h
//  ChessBE
//
//  Created by Tony on 18/05/2024.
//

#ifndef EngCommon_h
#define EngCommon_h

#include <stdlib.h>
#include <stdbool.h>
#include "Public.h"

typedef EngSquareMask SquareMask;

void *getMem(size_t required);

void freeMem(const void *p);

void fatalError(const char *message);

#endif /* EngCommon_h */
