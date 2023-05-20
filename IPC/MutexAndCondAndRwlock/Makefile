CC = gcc
CFLAGS = -I../unpv22e/lib -lpthread
objectname = MutexAndCond

.PHONY : TARGET
TARGET : $(objectname)


$(objectname) : %:%.c
	$(CC) $< $(CFLAGS) -o $@



.PHONY : clean
clean :
	-rm $(objectname)
	




