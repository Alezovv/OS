#include "../include/Validation.h"
#include "../include/Utils.h"

ValidationStatus ValidArgument(int argc, char *argv[])
{
    if (argc != 2)
    {
        return ERROR_INVALID_COUNT_ARGUMENTS;
    }

    if (argv[1] == NULL || strlen(argv[1]) == 0)
        return ERROR_INVALID_COUNT_THREADS;

    char *str = argv[1];
    size_t count = strlen(argv[1]);
    for (size_t i = 0; i < count; i++)
    {
        if (!isdigit(str[i]))
            return ERROR_INVALID_COUNT_THREADS;
    }

    int success = 0;
    int result = 0;

    if (argv[1])
    {
        result = string_to_int(argv[1], &success);
        if (!success)
            return ERROR_INVALID_COUNT_THREADS;
    }

    if (result <= 0)
        return ERROR_INVALID_COUNT_THREADS;

    return VALIDATION_SUCCESS;
}