#include "../include/Utils.h"

int string_to_int(const char *str, int *success)
{
    if (str == NULL || *str == '\0')
    {
        if (success)
            *success = 0;
        return 0;
    }

    int result = 0;
    int sign = 1;
    int i = 0;

    while (isspace((unsigned char)str[i]))
        i++;

    if (str[i] == '-')
    {
        sign = -1;
        i++;
    }
    else if (str[i] == '+')
        i++;

    while (str[i] != '\0')
    {
        if (!isdigit((unsigned char)str[i]))
        {
            if (success)
                *success = 0;
            return 0;
        }

        if (result > (INT_MAX - (str[i] - '0')) / 10)
        {
            if (success)
                *success = 0;
            return (sign == 1) ? INT_MAX : INT_MIN;
        }

        result = result * 10 + (str[i] - '0');
        i++;
    }

    if (success)
        *success = 1;
    return result * sign;
}
