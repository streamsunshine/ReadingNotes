CC = gcc
CFLAGS = 
objectname = ForOffer Function StackToQueQ7 testGlobal

.PHONY : TARGET
TARGET : $(objectname)


$(objectname) : %:%.c
	$(CC) $< $(CFLAGS) -o $@



.PHONY : clean
clean :
	-rm ForOffer $(objects)
	




