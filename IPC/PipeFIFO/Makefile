CC = gcc
CFLAGS = -I../unpv22e/lib
objectname = Pipe Popen FifoServer FifoClient

.PHONY : TARGET
TARGET : $(objectname)


$(objectname) : %:%.c
	$(CC) $< $(CFLAGS) -o $@



.PHONY : clean
clean :
	-rm $(objectname)
	




