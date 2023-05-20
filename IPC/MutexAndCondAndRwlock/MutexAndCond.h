#include "unpipc.h"

#define MAXITEMS 100000
#define MAX_PRODUCERPTHREAD 10

struct mutexStru
{
    pthread_mutex_t mutex;
    int buffIndex;
    int buffVal;
    int buff[MAXITEMS];
} mutexCtrl = {
    PTHREAD_MUTEX_INITIALIZER,\
        0,\
        0
};

struct condStru
{
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    int uncheck;
} condCtrl = {
    PTHREAD_MUTEX_INITIALIZER,\
        PTHREAD_COND_INITIALIZER,\
        0
};
