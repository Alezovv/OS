#ifndef CUSTOM_H
#define CUSTOM_H

#include "pthread.h"

typedef enum
{
    CUSTOM_SUCCESS = 0,
    ERROR_THREAD_CREATE,
    ERROR_THREAD_JOIN
} CustomStatus;

CustomStatus custom_pthread_create(pthread_t *thread,
                                   const pthread_attr_t *attr,
                                   void *(*start_routine)(void *),
                                   void *arg);

CustomStatus custom_pthread_join(pthread_t thread, void **retval);

#endif // CUSTOM_H