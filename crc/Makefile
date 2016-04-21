SRCS	= $(wildcard	*.c)
SRCS	+= $(wildcard	src/*.c)
INCS	= -I./ -I./src
OBJS	= $(SRCS:.c=.o)	#same as $(patsubst %.c,%.o,$(SRCS))
TARGET	=main.x

CC	= gcc
CFLAGS	= -Wall -std=c99	#-static -lpthread -lm

$(TARGET):$(OBJS)
	$(CC) $(CFLAGS) -o $@ $^


#%.o:%.c  
#	$(CC) $(CFLAGS) $(INC) -o $@ -c $< 


clean:
	rm -rf $(TARGET) $(OBJS)
	rm -rf *.ko
	rm -rf *.mod.*
	rm -rf .*.cmd
	rm -rf modules.order
	rm -rf Module.symvers
	rm -rf *~
