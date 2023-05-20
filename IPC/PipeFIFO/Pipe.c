#include "unpipc.h"

void serve(int readPipe,int writePipe);
void client(int readPipe,int writePipe);

int main(void)
{
    int pathPipe[2];    //传递路径的管道
    int dataPipe[2];
    int childpid;   //用于回收子进程（服务器进程）

    pipe(pathPipe); //创建管道
    pipe(dataPipe);

    if( (childpid = fork()) == 0)
    {
        close(pathPipe[1]);     //服务器进程读取路径，返回文件内容
        close(dataPipe[0]);
        serve(pathPipe[0],dataPipe[1]);
        exit(0);
    }
    close(pathPipe[0]);         //客户进程写路径，返回内容
    close(dataPipe[1]);
    client(dataPipe[0],pathPipe[1]);
    
    waitpid(childpid,NULL,0);
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
