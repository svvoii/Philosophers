# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: sbocanci <sbocanci@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/07/28 12:29:37 by sbocanci          #+#    #+#              #
#    Updated: 2023/07/28 18:23:17by sbocanci         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#############################################

NAME := Philosophers
_NC=`tput sgr0`
_RED=\033[0;31m
_GREEN=\033[0;32m
_YELLOW=\033[0;33m
_BLUE=\033[0;34m
_PURPLE=\033[0;95m
_CYAN=\033[0;36m
_WHITE=\033[0;37m

#############################################

#############################################

SRCS        := 					\
				helpers.c		\
				initialize.c	\
				main.c			\
				monitor.c		\
				routine.c		\
				../_sandbox_/_DEBUG_print.c		\

OBJS        := $(SRCS:%.c=%.o)
CC          := cc
CFLAGS      := -Wall -Wextra -Werror
INCLUDE		:= philosophers.h

#############################################

RM          := rm -f
MAKEFLAGS   += --no-print-directory

#############################################

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(OBJS) -o $(NAME)
	@printf "\e[92;5;118m\n>>> PHILOSOPHERS ARE READY <<<\n\e[0m"

$%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	$(RM) $(OBJS)
	@printf "\033[00;95m>>> objects removed.\n\033[00m"

fclean: clean
	$(RM) $(NAME)
	@printf "\033[00;31m>>> Philosophers dismissed.\n\033[00m"

re:
	$(MAKE) fclean
	$(MAKE) all

#############################################

.PHONY: clean fclean re

.SILENT:

#############################################