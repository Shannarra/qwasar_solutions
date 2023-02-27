CC = gcc
WFLAGS = -Wall -Wextra -Werror
EFLAGS = -g3 -fsanitize=address --pedantic-errors
LINKERFLAG = -lm
SRCS = $(wildcard source/*.c)
OBJS = $(SRCS:.c=.o)
MAIN = my_curl

.PHONY = all clean fclean re
.SILENT = all clean fclean re

all: $(MAIN)

$(MAIN): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(LINKERFLAG) $^

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(MAIN)

re: fclean all
