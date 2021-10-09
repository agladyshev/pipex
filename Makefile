# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: stiffiny <stiffiny@student.21-school.ru>   +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/06/30 14:39:16 by stiffiny          #+#    #+#              #
#    Updated: 2021/10/09 18:55:37 by stiffiny         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= pipex

LIST		=	pipex.c	helpers.c ft_split.c\
			ft_strlen.c

OBJS		= $(patsubst %.c,%.o,$(LIST))

D_FILES		= $(wildcard *.d)

CC			= gcc

CFLAGS 		= -Wall -Wextra -Werror

RM			= rm -f

all	:		$(NAME)

$(NAME)	:	$(OBJS)

.c.o :
	$(CC) $(CFLAGS) -c $< -o ${<:.c=.o} -MD

include	$(wildcard $(D_FILES))

clean	:
			$(RM) $(OBJS) $(D_FILES)

fclean	:	clean
			$(RM) $(NAME)

re	:		fclean all

.PHONY: all clean fclean re
