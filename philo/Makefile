NAME=philo
CC=cc
CFLAGS=-Wall -Wextra -Werror 
SRCS=ft_atol.c init.c monitor.c philo.c routine.c string.c utils.c
OBJS=$(SRCS:.c=.o)
RM=rm -f

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) 

%.o: %.c philo.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: clean