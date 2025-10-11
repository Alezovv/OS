#ifndef VALIDATION_H
#define VALIDATION_H

#include <stddef.h>
#include <unistd.h>
#include <string.h>

typedef enum
{
    VALIDATION_SUCCESS = 0,
    ERROR_INVALID_COUNT_ARGUMENTS,
    ERROR_INVALID_COUNT_THREADS,
    ERROR_COUNT_CORES
} ValidationStatus;

ValidationStatus ValidArgument(int argc, char *argv[]);

#endif // VALIDATION_H