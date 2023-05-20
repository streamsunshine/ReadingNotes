#include "unpipc.h"
#define FIFO1 "/tmp/fifo1"
#define FIFO2 "/tmp/fifo2"

void serve(int readPipe,int writePipe);

int main(void)
{
    int fdread;
    int fdwrite;
    int n;      

    //如果当前没有创建FIFO，则创建，如果已经创建，函数将置errno为EEXIST，如果都不是，说明出错，清理后返回
    if( (mkfifo(FIFO1,FILE_MODE) < 0) && (errno != EEXIST))
    {
        printf("can not create %s\n",FIFO1);
        exit(-1);
    }
    if( (mkfifo(FIFO2,FILE_MODE) < 0) && (errno != EEXIST))
    {
        unlink(FIFO1);
        printf("can not create %s\n",FIFO2);
        exit(-1);
    }

   fdread = open(FIFO1,O_RDONLY,0);
   fdwrite = open(FIFO2,O_WRONLY,0);
   serve(fdread,fdwrite);
    exit(0);

}

void serve(int readPipe,int writePipe)
{
    int fd;
    ssize_t n;
    char buffer[MAXLINE+1];

    if( ( n = read(readPipe,buffer,MAXLINE)) == 0)
    {
        printf("None of filePath!\n");
        exit(-1);
    }
    buffer[n] = '\0';

    if( (fd = open(buffer,O_RDONLY)) < 0 )  //处理文件无法打开的情况
    {
        snprintf(buffer + n,sizeof(buffer) - n,":can not open %s\n",strerror(errno));
        n = strlen(buffer);
        write(writePipe,buffer,n);
    }
    else
    {
        while( ( n = read(fd,buffer,MAXLINE)) > 0)//打开文件后，读出内容并通过管道发给客户端
        {
            write(writePipe,buffer,n);
        }
        close(fd);
    }
}
