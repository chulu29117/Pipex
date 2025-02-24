# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: clu <clu@student.hive.fi>                  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/01/15 16:06:47 by clu               #+#    #+#              #
#    Updated: 2025/02/24 11:01:59 by clu              ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Program Name
NAME = pipex

# Comiler and Flags
CC 		= cc
CFLAGS 	= -Wall -Werror -Wextra

# Directories
SRC_DIR 		= ./src
OBJ_DIR 		= ./obj
LIBFT_DIR 		= ./Libft

# Libraries
LIBFT = $(LIBFT_DIR)/libft.a

# Header files
HEADERS = -I./include -I$(LIBFT_DIR)

# Source files
SRC = $(shell find $(SRC_DIR) -type f -name "*.c")
OBJ = $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

all: $(LIBFT) $(NAME)

$(NAME): $(OBJ)
	@echo "Compiling $(NAME) ..."
	$(CC) $(CFLAGS) $(OBJ) $(LIBFT) $(HEADERS) -o $(NAME)
	@echo "Done"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) $(HEADERS) -c $< -o $@

$(LIBFT):
	@echo "Compiling Libft..."
	@make -C $(LIBFT_DIR)
	@echo "Done"

clean:
	@make -C $(LIBFT_DIR) clean
	@rm -rf $(OBJ_DIR)
	@echo "Cleaned up all object files"

fclean: clean
	@rm -f $(NAME)
	@make -C $(LIBFT_DIR) fclean
	@echo "Cleaned up $(NAME)"
	@echo "Cleaned up everything"

re: fclean all
	@echo "Remade everything"

.PHONY: all clean fclean re
