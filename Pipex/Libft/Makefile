# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: clu <clu@student.hive.fi>                  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/28 15:08:55 by clu               #+#    #+#              #
#    Updated: 2025/02/11 17:11:38 by clu              ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft.a
CC = cc
CFLAGS = -Wall -Werror -Wextra

SOURCES =	ft_atoi.c				ft_atol.c				ft_atoll.c				\
			ft_itoa.c				ft_swap.c				ft_bzero.c				\
			ft_calloc.c				ft_isalnum.c			ft_isalpha.c			\
			ft_isascii.c			ft_isdigit.c			ft_isprint.c			\
			ft_tolower.c			ft_toupper.c			ft_memchr.c				\
			ft_memcmp.c				ft_memcpy.c				ft_memmove.c			\
			ft_memset.c				ft_strchr.c				ft_strdup.c				\
			ft_striteri.c			ft_strjoin.c			ft_strlcat.c			\
			ft_strlcpy.c			ft_strlen.c				ft_strmapi.c			\
			ft_strncmp.c			ft_strnstr.c			ft_strrchr.c			\
			ft_strtrim.c			ft_substr.c				ft_split.c				\
			ft_putchar_fd.c			ft_putstr_fd.c			ft_putendl_fd.c			\
			ft_putnbr_fd.c			ft_lstnew_bonus.c		ft_lstadd_front_bonus.c	\
			ft_lstsize_bonus.c		ft_lstlast_bonus.c		ft_lstadd_back_bonus.c	\
			ft_lstdelone_bonus.c	ft_lstclear_bonus.c		ft_lstiter_bonus.c		\
			ft_lstmap_bonus.c		get_next_line.c			ft_printf.c				\
			ft_format_check.c		ft_putchar.c			ft_putstr.c				\
			ft_putnbr.c				ft_putnbr_i.c			ft_puthex.c				\
			ft_puthex_low.c			ft_puthex_up.c			ft_putptr.c				\
			ft_free_array.c			ft_strcmp.c
						
OBJECTS = $(SOURCES:.c=.o)

all: $(NAME)

$(NAME): $(OBJECTS)
	@echo "Linking library $(NAME)"
	@ar rcs $(NAME) $(OBJECTS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@echo "Cleaning libft object files"
	@rm -f $(OBJECTS)

fclean: clean
	@echo "Cleanining $(NAME)"
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
