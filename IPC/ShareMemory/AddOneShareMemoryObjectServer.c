#include "unpipc.h"

#define SEMFILE "/test2"
#define SHAREOBJECT "/shareobject1"

int cycletimes = 300;
int main(void)
{
    int fd;
    int *addr;
    int i,n;
    sem_t *semMu;

    shm_unlink(SHAREOBJECT);  //删除航次没有删除的共享内存区对象
    //初始化共享内存区对象
    fd = shm_open(SHAREOBJECT,O_RDWR | O_CREAT | O_EXCL,FILE_MODE);
    ftruncate(fd,sizeof(int));
    addr = mmap(NULL,sizeof(int),PROT_READ | PROT_WRITE,MAP_SHARED,fd,0);
    *addr = 0;

    close(fd);  //清理描述符，但是没有unlink，所以共享内存区对象仍然可用
    
    sem_unlink(SEMFILE);
    semMu = sem_open(SEMFILE,O_CREAT | O_EXCL,FILE_MODE,1);
    sem_close(semMu);

    exit(0);
}
