# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: yyuan <yyuan@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/12/09 19:06:43 by yyuan             #+#    #+#              #
#    Updated: 2021/12/11 22:06:52 by yyuan            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = gcc
CFLAGS = -Wall -Wextra -Werror
RM = rm -f
NAME= philo
NAME_BONUS= bonus_
SOURCES = ./main.c \
			./parser.c ./print.c ./utils.c
SRCS_BONUS = ./philo_bonus/main_bonus.c \
					./philo_bonus/parser_bonus.c ./philo_bonus/init_bonus.c ./philo_bonus/utils_bonus.c

OBJS = $(SOURCES:.c=.o)
OBJS_BONUS = $(SRCS_BONUS:./philo_bonus/%.c=./philo_bonus/%.o)

DEPS = -I.
DEPS_BONUS = -I./philo_bonus

RM	= rm -rf

all: $(NAME)

$(NAME):$(OBJS) 
	gcc $(CFLAGS) -o $(NAME) $(OBJS) $(DEPS) -lpthread

bonus: $(NAME_BONUS)

$(NAME_BONUS): $(OBJS_BONUS)
	@gcc $(CFLAGS) -o $(NAME_BONUS) $(OBJS_BONUS) $(DEPS_BONUS) -lpthread

$(OBJS_BONUS): ./philo_bonus/%.o : ./philo_bonus/%.c
	$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compiled "$<" successfully!"

$(OBJS):%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compiled "$<" successfully!"

clean:
	$(RM) $(OBJS)
	$(RM) $(OBJS_BONUS)

fclean: clean
	$(RM) $(NAME)
	$(RM) $(NAME_BONUS)

re: fclean all

.PHONY: all clean fclean re 
