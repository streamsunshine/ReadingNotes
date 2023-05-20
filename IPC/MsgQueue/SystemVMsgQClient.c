#include "SystemVMsgQ.h"

void client(int,int);

int main(int argc,char *argv[])
{
//关于消息队列标识符的创建参见服务器程序的注释
    int readMqid,writeMqid;
    key_t key1;
    key_t key2;
   
    key1 = ftok(MQ_PATH1,0);
    key2 = ftok(MQ_PATH2,1);

    printf("read:%d,write:%d\n",key1,key2);
    writeMqid = msgget(key1,SVMSG_MODE | IPC_CREAT); // 
    readMqid = msgget(key2,SVMSG_MODE | IPC_CREAT);

    printf("read:%d,write:%d\n",readMqid,writeMqid);

    client(readMqid,writeMqid);

    msgctl(readMqid,IPC_RMID,NULL);         //在传递完消息后，删除相应的消息队列
    msgctl(writeMqid,IPC_RMID,NULL);
    exit(0);

}
void client(int readMqid,int writeMqid)
{
    struct mymsg msg;
    int n;
    ssize_t len;

    fgets(msg.data,MAXMESGDATA,stdin);      //从标准输入读入想要由服务器打开的路径名
    len = strlen(msg.data);
    if(msg.data[len-1] == '\n')     //丢掉fgets读入的换行符
      len--;

    msg.type = getpid();        //将进程的pid作为消息类型传递给服务器，从而在不同客户之间进行区分,将pid_t转换为long是能够保证完整显示pid的

    msgsnd(writeMqid,&msg,len,0);

    while( (n = msgrcv(readMqid,&msg,MAXMESGDATA,msg.type,0)) > 0)     //msg.type指定读出类型为进程pid的消息
    {
        write(fileno(stdout),msg.data,n);
    }
    
}
