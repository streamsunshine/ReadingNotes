#include "unpipc.h"

#define SEMFILE "/test2"
#define SHAREOBJECT "/shareobject1"

int cycletimes = 300;
int main(void)
{
    int fd,zero = 0;
    int *addr;
    int i,n;
    sem_t *semMu;

    fd = shm_open(SHAREOBJECT,O_RDWR,FILE_MODE);
    addr = mmap(NULL,sizeof(int),PROT_READ | PROT_WRITE,MAP_SHARED,fd,0);
    close(fd);

    semMu = sem_open(SEMFILE,O_CREAT,FILE_MODE,1);

    setbuf(stdout,NULL);
    for(i = 0;i < cycletimes;i++)
    {
        sem_wait(semMu);
        printf("pid is %d,seq num is %d\n",getpid(),*addr);
        *addr += 1;
        sem_post(semMu);
    }
    sem_close(semMu);
    exit(0);
}
