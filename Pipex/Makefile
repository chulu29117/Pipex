# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: clu <clu@student.hive.fi>                  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/01/15 16:06:47 by clu               #+#    #+#              #
#    Updated: 2025/02/08 00:43:01 by clu              ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Program Name
NAME = pipex
BONUS = pipex_bonus

# Comiler and Flags
CC 		= cc
CFLAGS 	= -Wall -Werror -Wextra

# Directories
SRC_DIR 		= src
OBJ_DIR 		= obj
BONUS_SRC_DIR 	= bonus_src
BONUS_OBJ_DIR 	= bonus_obj
LIBFT_DIR 		= Libft

# Libraries
LIBFT = $(LIBFT_DIR)/libft.a

# Source files for the main program
SRC = 	pipex.c \
		substr_for_split.c \
		split_cmd.c \
		get_path.c \
		exec_cmd.c \
		exec_pipe.c \
		init_process.c \
		pipex_error.c

SRC := $(addprefix $(SRC_DIR)/, $(SRC))
OBJ = $(SRC:$(SRC_DIR)%.c=$(OBJ_DIR)/%.o)

# Source files for the bonus program
BONUS_SRC = pipex_bonus.c
BONUS_SRC := $(addprefix $(BONUS_SRC_DIR)/, $(BONUS_SRC))
BONUS_OBJ = $(BONUS_SRC:$(BONUS_SRC_DIR)%.c=$(BONUS_OBJ_DIR)/%.o)

all: $(NAME)

$(NAME): $(OBJ) $(LIBFT)
	@echo "Compiling $(NAME)"
	$(CC) $(CFLAGS) $(OBJ) -L$(LIBFT_DIR) -lft -o $(NAME)
	@echo "Done"

bonus: $(BONUS_OBJ) $(OBJ) $(LIBFT)
	@echo "Compiling $(BONUS)"
	$(CC) $(CFLAGS) $(BONUS_OBJ) $(OBJ) -L$(LIBFT_DIR) -lft -o $(BONUS)
	@echo "Done"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -Iincludes -I$(LIBFT_DIR) -c $< -o $@

$(BONUS_OBJ_DIR)/%.o: $(BONUS_SRC_DIR)/%.c
	@mkdir -p $(BONUS_OBJ_DIR)
	@echo "Compiling bonus $<..."
	$(CC) $(CFLAGS) -Iincludes -I$(LIBFT_DIR) -c $< -o $@

$(LIBFT):
	@echo "Compiling Libft..."
	@make -C $(LIBFT_DIR)

clean:
	@make -C $(LIBFT_DIR) clean
	@rm -rf $(OBJ_DIR) $(BONUS_OBJ_DIR)
	@echo "Cleaned up all object files"

fclean: clean
	@make -C $(LIBFT_DIR) fclean
	@rm -f $(NAME) $(BONUS)
	@echo "Cleaned up $(NAME)"
	@echo "Cleaned up everything"

re: fclean all
	@echo "Remade everything"

# Phony targets
.PHONY: all clean fclean re
