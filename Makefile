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
SRCS_BONUS		= main_bonus.c
OBJS			= $(SRCS:.c=.o)
OBJS_BONUS		= $(SRCS_BONUS:.c=.o)

CC				= gcc
RM				= rm -f
#CFLAGS			= -Wall -Wextra -Werror -I.
CFLAGS			= -I.
LIBS			= -lpthread

NAME			= philo
NAME_BONUS		= philo_bonus

all:			$(NAME)

$(NAME):		$(OBJS)
				gcc ${CFLAGS} -o ${NAME} ${OBJS} ${LIBS}

bonus:			$(OBJS_BONUS)
				gcc ${CFLAGS} -o ${NAME_BONUS} ${OBJS_BONUS} ${LIBS}

clean:
				$(RM) $(OBJS) $(OBJS_BONUS)

fclean:			clean
				$(RM) $(NAME) ${NAME_BONUS}

re:				fclean $(NAME)

.PHONY:			all clean fclean re
