NAME	= codexion

CC		= cc
CFLAGS	= -Wall -Wextra -Werror -pthread

SRCS	= cleenup.c coder_routine.c heap_helpers.c  \
		  init.c log_state.c main.c monitor_routin.c \
		  parsing.c take_two_dongles.c utils.c init_coders.c \
		  coder_helper.c heap_func.c monitor_helper.c takes_two_dongles_helpers.c

OBJS	= $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re