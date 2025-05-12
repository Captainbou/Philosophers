# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: zbouchra <zbouchra@student.1337.ma>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/04/23 23:18:24 by zbouchra          #+#    #+#              #
#    Updated: 2025/05/12 20:25:09 by zbouchra         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME=philo_bonus
CC=cc
CFLAGS=-Wall -Wextra -Werror 
SRCS=ft_atol_bonus.c init_bonus.c monitor_bonus.c philo_bonus.c routine_bonus.c string_bonus.c utils_bonus.c philo_uts_bonus.c
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