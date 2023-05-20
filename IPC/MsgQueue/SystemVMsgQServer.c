#include "SystemVMsgQ.h"

void server(int readMqid,int writeMqid);

int main(void)
{
    int readMqid,writeMqid;     //消息队列的标识符为整型
    key_t key1;     //创建消息队列标识符先用ftok创建key_t的键值，然后利用键值创建消息队列标识符
    key_t key2;
   
    key1 = ftok(MQ_PATH1,0);        //这里路径定义在头文件中，需要事先创建好相应的文件
    key2 = ftok(MQ_PATH2,1);
    printf("read:%d,write:%d\n",key1,key2);     //调试语句，用来检测是否返回错误
    readMqid = msgget(key1,IPC_CREAT | SVMSG_MODE);  //关于SVMSG_MODE的定义，要注意8进制使以0开头的如 0400
    writeMqid = msgget(key2,IPC_CREAT | SVMSG_MODE);    //IPC_CREAT表示如果存在返回标识符，不存在就创建

    printf("read:%d,write:%d\n",readMqid,writeMqid);
    server(readMqid,writeMqid);     //主功能函数
    exit(0);
}
void server(int readMqid,int writeMqid)
{
    int fd;
    ssize_t n;
    struct mymsg msg;       //根据消息模板创建的消息类型，其中type用来区分不同进程，从而进行单个服务器，多个客户端任务

    msg.type = 0;   //接收第一个消息，可以来自任何客户，所以接收任何类型的消息
    if( ( n = msgrcv(readMqid,&msg,MAXMESGDATA,msg.type,0)) <= 0)   //接收函数一定要注意，传递的缓冲区需要包含type部分，但是指定大小只指定数据部分，其中指定的类型表示要接收哪一种类型的消息
    {
        printf("None of filePath!");
        exit(-1);
    }
    msg.data[n] = '\0';
    printf("server print path %s\n",msg.data);

    //关于消息类型一下都没有修改，也就是使用了上面msgrcv收到的消息类型字段
    if( (fd = open(msg.data,O_RDONLY)) < 0 )  //处理文件无法打开的情况
    {
        snprintf(msg.data + n,sizeof(msg.data) - n,":can not open %s\n",strerror(errno));
        n = strlen(msg.data);
        msgsnd(writeMqid,&msg,n,0);
    }
    else
    {
        while( ( n = read(fd,msg.data,MAXMESGDATA)) > 0)//打开文件后，读出内容并通过管道发给客户端，当遇到EOF时，退出循环，从而结束服务端
        {
            msgsnd(writeMqid,&msg,n,0);
        }
        close(fd);
    }
    msgsnd(writeMqid,&msg,0,0); //该语句为了发送消息给客户端，让客户端知悉遇到EOF从而退出客户程序，否则客户将阻塞在msgrcv函数中
}

