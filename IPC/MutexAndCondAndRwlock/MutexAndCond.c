#include "MutexAndCond.h"

void *producer(void *);
void *consumer(void *);

int main(void)
{
    int i;
    pthread_t producerPthread[MAX_PRODUCERPTHREAD];
    pthread_t consumerPthread;
    int countProducer[MAX_PRODUCERPTHREAD];

    pthread_setconcurrency(MAX_PRODUCERPTHREAD + 1);
    for(i = 0;i < MAX_PRODUCERPTHREAD;i++)
    {
        countProducer[i] = 0;
        pthread_create(&producerPthread[i],NULL,producer,&countProducer[i]);
    }

    pthread_create(&consumerPthread,NULL,consumer,NULL);

    for(i = 0;i < MAX_PRODUCERPTHREAD;i++)
    {
        pthread_join(producerPthread[i],NULL);
        printf("count[%d] = %d\n",i,countProducer[i]);
    }

    pthread_join(consumerPthread,NULL);
    exit(0);
}


void *producer(void * arg)
{
    while(1)
    {
        pthread_mutex_lock(&mutexCtrl.mutex);
        if(mutexCtrl.buffIndex >= MAXITEMS)
        {
            pthread_mutex_unlock(&mutexCtrl.mutex);
            return NULL;
        }
        mutexCtrl.buff[mutexCtrl.buffIndex++] = mutexCtrl.buffVal++;
        pthread_mutex_unlock(&mutexCtrl.mutex);

        pthread_mutex_lock(&condCtrl.mutex);
        condCtrl.uncheck++;
        pthread_cond_signal(&condCtrl.cond);
        pthread_mutex_unlock(&condCtrl.mutex);

        *((int *)arg) += 1;
    }
}

void *consumer(void *arg)
{
    int i;

    for(i = 0;i < MAXITEMS;)
    {
        pthread_mutex_lock(&condCtrl.mutex);
        while(condCtrl.uncheck == 0)
        {
            pthread_cond_wait(&condCtrl.cond,&condCtrl.mutex);
        }
        for(;condCtrl.uncheck > 0;condCtrl.uncheck--)
        {
            if(mutexCtrl.buff[i] != i)
            {
                printf("Wrong in buffer[%d] with %d\n",i,mutexCtrl.buff[i]);
                exit(-1);
            }
            i++;
        }
        pthread_mutex_unlock(&condCtrl.mutex);
    }
}
