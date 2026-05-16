NAME	= codexion

CC		= cc

CFLAGS	= -Wall -Wextra -Werror -pthread

SRCS	= ./coders/cleanup.c ./coders/init.c ./coders/monitor_routine.c  ./coders/take_two_dongles.c \
		  ./coders/coder_routine.c  ./coders/lock_unlock.c  ./coders/parsing.c ./coders/utils.c \
		  ./coders/heap_func.c ./coders/logs.c ./coders/push_to_queue.c \
		  ./coders/heap_helpers.c ./coders/main.c ./coders/state.c ./coders/init_coder.c

HEADER = ./coders/codexion.h
OBJS	= $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

%.o: %.c $(HEADER)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
