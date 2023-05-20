#include "unpipc.h"
#define SEQFILE "seqfile"

#define SEMFILE "/tmp/semfile"

void mylock(int);
void myunlock(int);

int semid,initflag = 0;//注意一个进程只尝试一次初始化

int main(int argc,char *argv[])
{
    int i;
    int cycletimes = 30;
    char buff[MAXLINE];
    pid_t pid;
    long seqNum;
    ssize_t n;
    int fd;

    fd = open(SEQFILE,O_RDWR,FILE_MODE);
    pid = getpid();
    for(i = 0;i < cycletimes;i++)
    {
        mylock(fd);

        lseek(fd,0L,SEEK_SET);
        n = read(fd,buff,MAXLINE);
        buff[n] = '\0';
        sscanf(buff,"%ld\n",&seqNum);

        printf("%s:pid = %ld,seq = %ld\n",argv[0],(long)pid,seqNum);

        seqNum++;
        snprintf(buff,MAXLINE,"%ld\n",seqNum);
        lseek(fd,0L,SEEK_SET);
        write(fd,buff,strlen(buff));
        myunlock(fd);
    }
    close(fd);
    exit(0);
}

void mylock(int fd)
{
    int oflag = IPC_CREAT | SVSEM_MODE | IPC_EXCL;
    struct sembuf optBuf[1];
    union semun arg;
    struct semid_ds ds;

    if(initflag == 0)
    {
        if( (semid = semget(ftok(SEMFILE,0),1,oflag)) >= 0)
        {
            arg.val = 1;
            semctl(semid,0,SETVAL,arg);
        }
        else if(errno == EEXIST)
        {
            semid = semget(ftok(SEMFILE,0),1,SVSEM_MODE);
            do
            {
                arg.buf = &ds;
                semctl(semid,0,IPC_STAT,arg);
            }while(arg.buf->sem_otime == 0);
        }
        else
        {
            printf("Semget error");
            exit(-1);
        }
        initflag = 1;
    }
    optBuf[0].sem_num = 0;
    optBuf[0].sem_op = -1;
    optBuf[0].sem_flg = SEM_UNDO;
    semop(semid,optBuf,1);
}
void myunlock(int fd)
{
    struct sembuf optBuf[1];

    optBuf[0].sem_num = 0;
    optBuf[0].sem_op = 1;
    optBuf[0].sem_flg = SEM_UNDO;
    semop(semid,optBuf,1);
}

