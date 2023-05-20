CC = gcc
CFLAGS = -I../unpv22e/lib -lpthread -lrt
objectname = AddOnePosixNameSem AddOnePosixMemSem AddOneShareMemoryObjectServer AddOneShareMemoryObjectClient

.PHONY : TARGET
TARGET : $(objectname)


$(objectname) : %:%.c
	$(CC) $< $(CFLAGS) -o $@



.PHONY : clean
clean :
	-rm $(objectname)
	




