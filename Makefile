# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ncolomer <ncolomer@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/12/09 19:06:43 by ncolomer          #+#    #+#              #
#    Updated: 2019/12/12 23:24:03 by ncolomer         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#SRCS			= main.c utils.c exit.c messages.c init.c forks.c eat.c
SRCS			= main.c
OBJS			= $(SRCS:.c=.o)

CC				= gcc
RM				= rm -f
#CFLAGS			= -Wall -Wextra -Werror -I.
CFLAGS			= -I.
LIBS			= -lpthread

NAME			= philo

all:			$(NAME)

$(NAME):		$(OBJS)
				gcc ${CFLAGS} -o ${NAME} ${OBJS} ${LIBS}

clean:
				$(RM) $(OBJS) $(BONUS_OBJS)

fclean:			clean
				$(RM) $(NAME)

re:				fclean $(NAME)

.PHONY:			all clean fclean re
