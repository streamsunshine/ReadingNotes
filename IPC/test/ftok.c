#include <sys/msg.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <stdio.h>

/**********************************
 *
 *ftok
 *路径名参数；如果不存在或者对于调用调用进程不可访问，ftok返回-1,对应文件不能是服务器存活其间反复创建并删除的文件。
 *id参数：id的低序8位对应ftok返回值的低序8位，所以想获得的ftok返回值，可以设置id 不同。 
 * */
/*
int main(int argc,char *argv[])
{
    int id = 1;
    int n;
    if((n= ftok(argv[1],id)) ==  -1)
    {
        printf("Failed\n");
    }
    else
    {
        printf("returnValue = %d\n",n);
    }
}
*/

#define	SVMSG_MODE 0x666

int main(void)
{
    struct msqid_ds ms;
    int n;
    int mgNum;
    int i,circleNum = 5;
    for(i=0;i<circleNum;i++)
    {
        mgNum = msgget(IPC_PRIVATE,SVMSG_MODE | IPC_CREAT);
        msgctl(mgNum,IPC_STAT,&ms);

        printf("msgNum = %d,seq = %lu\n",mgNum,ms.msg_perm.__seq);
        msgctl(mgNum,IPC_RMID,NULL);

    }
}

