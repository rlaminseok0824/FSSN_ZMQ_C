C_SRC = $(wildcard *.c)
OBJS = $(C_SRC:.c=.o)

all : $(OBJS)
	echo completed!!

$(OBJS) : $(C_SRC)
	gcc -Wall -o $@ $*.c -lczmq -L/lib/x86_64-linux-gnu

clean : 
	rm *.o
