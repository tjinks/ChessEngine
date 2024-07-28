//
//  EngCommon.c
//  ChessBE
//
//  Created by Tony on 18/05/2024.
//

#include <stdlib.h>
#include <stdio.h>
#include "EngCommon.h"

void *getMem(size_t required) {
    void *result = malloc(required);
    if (result == NULL) {
        fatalError("Out of memory!");
    }

    return  result;
}

void freeMem(const void *p) {
    free((void *)p);
}

void fatalError(const char *message) {
    fprintf(stderr, "%s\n", message);
    exit(1);
}

