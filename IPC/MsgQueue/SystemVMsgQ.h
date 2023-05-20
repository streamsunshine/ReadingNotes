#include "unpipc.h"

#define MQ_PATH1 "/tmp/test11"
#define MQ_PATH2 "/tmp/test12"

#define MAXMESGDATA (PIPE_BUF - 1*sizeof(long))
struct mymsg
{
    long type;  //消息的模板要求第一个需要是long 
    char data[MAXMESGDATA];
};
