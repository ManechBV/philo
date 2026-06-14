SRCS = srcs/main.c srcs/utils.c
OBJS = $(SRCS:srcs/%.c=objs/%.o)
CC = cc
FLAGS = -Wall -Wextra -Werror -g -pthread
NAME = philo
INCLUDE_DIRS = -I includes

all: objs $(NAME)

$(NAME): $(OBJS) 
	$(CC) $(OBJS) $(FLAGS) $(INCLUDE_DIRS) -o $(NAME)

$(OBJS): objs/%.o: srcs/%.c
	$(CC) $(FLAGS) $(INCLUDE_DIRS) -c $< -o $@

objs:
	mkdir objs

re: fclean all

clean:
	rm -rf $(OBJS)

fclean:
	rm -rf $(NAME)

.PHONY: all re clean fclean
