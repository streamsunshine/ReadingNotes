#include "unpipc.h"
#define FIFO1 "/tmp/fifo1"
#define FIFO2 "/tmp/fifo2"

void client(int readPipe,int writePipe);

int main(void)
{
    int fdread;     //打开FIFO2对应的描述符，在服务器中对应写
    int fdwrite;

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

//打开创建好的FIFO
   fdwrite = open(FIFO1,O_WRONLY,0);
   fdread = open(FIFO2,O_RDONLY,0);
   client(fdread,fdwrite);
//关闭描述符，并清理FIFO
   close(fdread);
   close(fdwrite);

   unlink(FIFO1);
   unlink(FIFO2);
    exit(0);

}
void client(int readPipe,int writePipe)
{
    char buff[MAXLINE];
    ssize_t n;
    size_t len;

    fgets(buff,MAXLINE,stdin);
    len = strlen(buff);

    if(buff[len-1] == '\n')
      len--;            //删除fgets获取的换行符

    write(writePipe,buff,len);

    while( (n =read(readPipe,buff,MAXLINE)) > 0)    //从管道读出数据并输出到stdout
    {
        write(fileno(stdout),buff,n);
    }
}
