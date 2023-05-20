#include "unpipc.h"
#define SEQFILE "seqfile"

void mylock(int);
void myunlock(int);

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
    struct flock lockInfo;
    lockInfo.l_type = F_WRLCK;
    lockInfo.l_whence =  SEEK_SET;
    lockInfo.l_start = 0;
    lockInfo.l_len = 0;

    fcntl(fd,F_SETLKW,&lockInfo);
}
void myunlock(int fd)
{
    struct flock lockInfo;
    lockInfo.l_type = F_UNLCK;
    lockInfo.l_whence =  SEEK_SET;
    lockInfo.l_start = 0;
    lockInfo.l_len = 0;

    fcntl(fd,F_SETLK,&lockInfo);
}

