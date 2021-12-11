# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: yyuan <yyuan@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/12/09 19:06:43 by yyuan             #+#    #+#              #
#    Updated: 2021/12/11 17:20:22 by yyuan            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = gcc
CFLAGS = -Wall -Wextra -Werror -pthread
RM = rm -f
NAME= philo
NAME_BONUS= bonus_
SOURCES = ./main.c \
			./parser.c ./print.c ./utils.c
SRCS_BONUS = ./philo_bonus/main_bonus.c \
					./philo_bonus/parser_bonus.c ./philo_bonus/init_bonus.c ./philo_bonus/utils_bonus.c

OBJS = $(SOURCES:.c=.o)
OBJ_BONUS = $(SRCS_BONUS:.c=.o)

#DEPS = -I ./philosophers.h
DEPS = -I.
#DEPS_BONUS = -I ./philo_bonus/philosophers_bonus.h
DEPS_BONUS = -I./philo_bonus

RM	= rm -rf


all: $(NAME)

##.c.o:
#	$(CC) $(CFLAGS) -c $< -o ${<:.c=.o}

$(NAME):$(OBJS) 
	gcc $(CFLAGS) -o $(NAME) $(SOURCES) $(DEPS)

bonus: $(NAME_BONUS)

$(NAME_BONUS): $(OBJS_BONUS)
	gcc $(CFLAGS) -o $(NAME_BONUS) $(SRCS_BONUS) $(DEPS_BONUS)  

clean:
	$(RM) *.o

fclean: clean
	$(RM) $(NAME)
	$(RM) *.o

re: fclean all

.PHONY: all clean fclean re 
