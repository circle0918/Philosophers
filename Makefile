# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: yyuan <yyuan@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/12/09 19:06:43 by yyuan             #+#    #+#              #
#    Updated: 2019/12/12 23:24:03 by yyuan            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = gcc
CFLAGS = -Wall -Wextra -Werror -pthread
RM = rm -f
NAME= philo
SOURCES = ./main.c \
		./parser.c ./utils.c ./print.c

all: $(NAME)

$(NAME): fclean
	gcc $(CFLAGS) -o $(NAME) $(SOURCES)

clean:
	$(RM) *.o

fclean: clean
	$(RM) $(NAME)
	$(RM) *.o

re: fclean all

.PHONY: all clean fclean re 
