CC = gcc
OBJS = shell.o

shell:$(OBJS)
	$(CC) $^ -o shell
	
%.o:%.c
	$(CC) -c $^