# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: yyuan <yyuan@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/12/09 19:06:43 by yyuan             #+#    #+#              #
#    Updated: 2021/12/12 11:15:38 by yyuan            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #
NAME     = philo_bonus

CC       = gcc
# compiling flags here
CFLAGS   = -Wall -Wextra -Werror -I.
#CFLAGS   = -Wall -Wextra -Werror -I. -fsanitize=thread -g

LINKER   = gcc
# linking flags here
LFLAGS   = -Wall -Wextra -Werror -I. -lpthread
#LFLAGS   = -Wall -Wextra -Werror -I. -lpthread -fsanitize=thread -g

# change these to proper directories where each file should be
SRCDIR   = src
OBJDIR   = obj
#BINDIR   = bin
BINDIR   = .

SOURCES  := $(wildcard $(SRCDIR)/*.c)
INCLUDES := $(wildcard $(SRCDIR)/*.h)
OBJECTS  := $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
rm       = rm -f

all: $(BINDIR) $(OBJDIR) $(BINDIR)/$(NAME)

$(BINDIR)/$(NAME): $(OBJECTS)
	@$(LINKER) $(OBJECTS) $(LFLAGS) -o $@
	@echo "Linking complete!"

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.c
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compiled "$<" successfully!"

$(OBJDIR):
	@mkdir -p $(OBJDIR)

$(BINDIR):
	@mkdir -p $(BINDIR)

clean:
	@$(rm) $(OBJECTS)
	@$(rm) -r $(OBJDIR)
	@echo "Cleanup complete!"

fclean: clean
	@$(rm) $(BINDIR)/$(NAME)
	@echo "Executable removed!"

re: fclean all

.PHONY: all clean fclean re 
