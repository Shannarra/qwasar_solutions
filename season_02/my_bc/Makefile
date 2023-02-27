OUT	= my_bc
CC	 = gcc
SOURCES = *.c
FLAGS	 = -Wall -Wextra -Werror 
LFLAGS	 = -g3 -fsanitize=address --pedantic

run:
	$(CC) $(FLAGS) $(LFLAGS) $(SOURCES) -o $(OUT)

clean:
	rm my_bc

re: clean run
