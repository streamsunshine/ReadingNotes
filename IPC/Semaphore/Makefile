CC = gcc
CFLAGS = -I../unpv22e/lib -lpthread
objectname = SemProdCons1 SemMultiprodMulticons SemMultiBuff RecordLockingNone RecordLockingSem

.PHONY : TARGET
TARGET : $(objectname)


$(objectname) : %:%.c
	$(CC) $< $(CFLAGS) -o $@



.PHONY : clean
clean :
	-rm $(objectname)
	




