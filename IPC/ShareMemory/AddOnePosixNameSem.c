#include "unpipc.h"

#define SEMFILE "/test1"
#define SEQFILE "seqfile"

int cycletimes = 30;
int main(void)
{
    int fd,zero = 0;
    void *addr;
    int i,n;
    sem_t *semMu;

    semMu = sem_open(SEMFILE,O_CREAT,FILE_MODE,1);
    fd = open(SEQFILE,O_RDWR | O_CREAT,FILE_MODE);
    write(fd,&zero,sizeof(int));
    addr = mmap(NULL,sizeof(int),PROT_READ | PROT_WRITE,MAP_SHARED,fd,0);
    close(fd);
    sem_unlink(SEMFILE);
    setbuf(stdout,NULL);
    if( ( n = fork()) == 0)
    {
        for(i = 0;i < cycletimes;i++)
        {
            sem_wait(semMu);
            printf("pid is %d,seq num is %d\n",getpid(),*( (int *)addr));
            *( (int *)addr) += 1;
            sem_post(semMu);
        }
        exit(0);
    }
    for(i = 0;i < cycletimes;i++)
    {
        sem_wait(semMu);
        printf("pid is %d,seq num is %d\n",getpid(),*( (int *)addr));
        *( (int *)addr) += 1;
        sem_post(semMu);
    }
    exit(0);
}
