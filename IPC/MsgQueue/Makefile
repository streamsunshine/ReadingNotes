CC = gcc
CFLAGS = -I../unpv22e/lib
objectname = SystemVMsgQClient SystemVMsgQServer

.PHONY : TARGET
TARGET : $(objectname)


$(objectname) : %:%.c
	$(CC) $< $(CFLAGS) -o $@



.PHONY : clean
clean :
	-rm $(objectname)
	




