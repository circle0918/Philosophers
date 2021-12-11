# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: yyuan <yyuan@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/12/09 19:06:43 by yyuan             #+#    #+#              #
#    Updated: 2021/12/11 22:43:14 by yyuan            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = gcc
CFLAGS = -Wall -Wextra -Werror
RM = rm -f
NAME= philo
NAME_BONUS= philo_bonus
SOURCES = ./main.c \
			./parser.c ./print.c ./utils.c
SRCS_BONUS = ./bonus/main_bonus.c \
					./bonus/parser_bonus.c ./bonus/init_bonus.c ./bonus/utils_bonus.c

OBJS = $(SOURCES:.c=.o)
OBJS_BONUS = $(SRCS_BONUS:./bonus/%.c=./bonus/%.o)

DEPS = -I.
DEPS_BONUS = -I./bonus

RM	= rm -rf

all: $(NAME)

$(NAME):$(OBJS) 
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(DEPS) -lpthread

$(OBJS):%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compiled "$<" successfully!"

bonus: $(NAME_BONUS)

$(NAME_BONUS): $(OBJS_BONUS)
	$(CC) $(CFLAGS) -o $(NAME_BONUS) $(OBJS_BONUS) $(DEPS_BONUS) -lpthread

$(OBJS_BONUS): ./bonus/%.o : ./bonus/%.c
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
