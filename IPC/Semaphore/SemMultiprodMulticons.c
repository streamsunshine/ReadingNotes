/*这里创建一个使用基于内存的信号量的多生产者，多消费者程序。
 * 本程序是在SemProdCons1.c进行修改的
 */
#include "unpipc.h"

#define NBUFF 20        //定义缓冲区的大小
#define PRODUCERNUM 10
#define CONSUMERNUM 8

struct semMutex
{
    int pIndex;
    int pValue;
    int cIndex;
    int cValue;

    sem_t semMu,semEmpty,semStore;//分别对缓冲区进行保护，表示缓冲区空余空间和被占据的空间从而控制生产者和消费者
    int buff[NBUFF];
}shared = {
    0,0,0,0
};
int cycleTimes = 1000;   //生产次数，也是消费次数，表示程序经过多少个生产消费过程结束

void *producer(void *);
void *consumer(void *);

int main(void)
{
    pthread_t pthreadProd[PRODUCERNUM],pthreadCons[CONSUMERNUM];
    int i;
    int countP[PRODUCERNUM],countC[CONSUMERNUM];

    sem_init(&shared.semMu,0,1);
    sem_init(&shared.semEmpty,0,NBUFF);
    sem_init(&shared.semStore,0,0);

    pthread_setconcurrency(PRODUCERNUM + CONSUMERNUM);      //告知系统运行的线程数目
    for(i = 0;i < PRODUCERNUM;i++)
    {
        countP[i] = 0;
        pthread_create(&pthreadProd[i],NULL,producer,&countP[i]);        //创建线程
    }
    for(i = 0;i < CONSUMERNUM;i++)
    {
        countC[i] = 0;
        pthread_create(&pthreadCons[i],NULL,consumer,&countC[i]);
    }
    for(i = 0;i < PRODUCERNUM;i++)
    {
        printf("producer[%d] is %d\n",i,countP[i]);
        pthread_join(pthreadProd[i],NULL); //回收线程
    }
    for(i = 0;i < CONSUMERNUM;i++)
    {
        printf("consumer[%d] is %d\n",i,countC[i]);
        pthread_join(pthreadCons[i],NULL);
    }

    sem_destroy(&shared.semMu);
    sem_destroy(&shared.semEmpty);
    sem_destroy(&shared.semStore);
    exit(0);
}

void *producer(void *arg)
{
    while(1)
    {
        sem_wait(&shared.semEmpty);  //这里等待的顺序很重要，否则容易造成死锁
        sem_wait(&shared.semMu);
        if(shared.pIndex >= cycleTimes)
        {
            sem_post(&shared.semMu);
            sem_post(&shared.semEmpty);
            return NULL;
        }
        shared.buff[shared.pIndex % NBUFF] = shared.pValue;
        shared.pIndex++;
        shared.pValue++;
        sem_post(&shared.semMu);
        sem_post(&shared.semStore);
        *((int *)arg) += 1;
    }
    return NULL;
}

void * consumer(void *arg)
{
    while(1)
    {
        sem_wait(&shared.semStore);
        sem_wait(&shared.semMu);
        if(shared.cIndex >= cycleTimes)
        {
            sem_post(&shared.semMu);
            sem_post(&shared.semStore);
            return NULL;
        }

        if(shared.buff[shared.cIndex % NBUFF] != shared.cValue)
        {
          printf("something wrong at %d value is %d\n",shared.cIndex,shared.buff[shared.cIndex % NBUFF]);
          exit(-1);
        }
        shared.cIndex++;
        shared.cValue++;
        if(shared.cIndex >= cycleTimes)
        {
            sem_post(&shared.semStore);
        }
        sem_post(&shared.semMu);
        sem_post(&shared.semEmpty);
        *((int *)arg) += 1;
    }
    return NULL;
}
