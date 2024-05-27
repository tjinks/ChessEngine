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

typedef uint_least64_t SquareMask;

void *getMem(size_t required);

void freeMem(void *p);

#endif /* EngCommon_h */
