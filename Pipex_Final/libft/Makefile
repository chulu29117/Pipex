# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: clu <clu@student.hive.fi>                  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/28 15:08:55 by clu               #+#    #+#              #
#    Updated: 2025/02/25 15:31:17 by clu              ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Program Name
NAME = libft.a

# Compiler and Flags
CC = cc
CFLAGS = -Wall -Werror -Wextra

# Directories
SRC_DIR	= ./src
OBJ_DIR	= ./obj

# Header
HEADER = -I./include

# Source files and object files
SRC = $(shell find $(SRC_DIR) -type f -name "*.c")
OBJ = $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

all: $(NAME)

$(NAME): $(OBJ)
	@echo "Creating static library $(NAME)"
	ar rcs $(NAME) $(OBJ)
	@echo "Done"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) $(HEADER) -c $< -o $@

clean:
	@echo "Cleaning libft object files"
	@rm -f $(OBJ)

fclean: clean
	@echo "Cleanining $(NAME)"
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
