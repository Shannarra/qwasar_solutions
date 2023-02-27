NASM = nasm
NFLAGS = -felf64
SRCS = $(wildcard lib/*.S)
OBJS = $(SRCS:.S=.o)
CC = gcc
CFLAGS = -Wall -Wextra -Werror -g3 --pedantic -fsanitize=address
MAIN = my_libasm

.PHONY = all clean fclean re

all: $(MAIN)

$(MAIN): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ main.c

%.o : %.S
	$(NASM) $(NFLAGS) -o $@ $^

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(MAIN)

re: fclean all
