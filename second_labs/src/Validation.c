#include "../include/Validation.h"
#include "../include/Utils.h"

ValidationStatus get_cpu_cores(int *buf_cores)
{
    long cores = sysconf(_SC_NPROCESSORS_ONLN);
    if (cores < 1)
        return ERROR_COUNT_CORES;

    *buf_cores = (int)cores;
    return VALIDATION_SUCCESS;
}

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

    int cur_cores = 0;
    ValidationStatus status = get_cpu_cores(&cur_cores);
    if (status != VALIDATION_SUCCESS)
        return status;

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
    
    int recom_threads = (int)(cur_cores * 1.5) + 1;
    if (result > recom_threads * 3)
    {
        return TO_MANY_THREADS;
    }
    else if (result > recom_threads)
        return WARNING_ARGUMENTS;

    return VALIDATION_SUCCESS;
}