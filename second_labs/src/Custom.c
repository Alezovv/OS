#include "../include/Custom.h"

CustomStatus custom_pthread_create(pthread_t *thread,
                                   const pthread_attr_t *attr,
                                   void *(*start_routine)(void *),
                                   void *arg)
{
    if (pthread_create(thread, attr, start_routine, arg) != 0)
        return ERROR_THREAD_CREATE;

    return CUSTOM_SUCCESS;
}

CustomStatus custom_pthread_join(pthread_t thread, void **retval)
{
    if (pthread_join(thread, retval) != 0)
        return ERROR_THREAD_JOIN;

    return CUSTOM_SUCCESS;
}