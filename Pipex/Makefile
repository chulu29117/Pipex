# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: clu <clu@student.hive.fi>                  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/01/15 16:06:47 by clu               #+#    #+#              #
#    Updated: 2025/02/11 18:40:53 by clu              ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Program Name
NAME = pipex

# Comiler and Flags
CC 		= cc
CFLAGS 	= -Wall -Werror -Wextra

# Directories
SRC_DIR 		= src
OBJ_DIR 		= obj
BONUS_SRC_DIR 	= bonus_src
BONUS_OBJ_DIR 	= bonus_obj
LIBFT_DIR 		= Libft
HEADER_DIR 		= includes

# Libraries
LIBFT = $(LIBFT_DIR)/libft.a

# Mandatory source files (excluding the one with main)
MAND_SRC = substr_for_split.c \
           split_cmd.c \
           get_path.c \
           exec_cmd.c \
           exec_pipe.c \
           init_process.c \
           pipex_error.c

MAND_SRC := $(addprefix $(SRC_DIR)/, $(MAND_SRC))
MAND_OBJ = $(MAND_SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Mandatory main file (contains main)
MAIN_SRC = pipex.c
MAIN_SRC := $(addprefix $(SRC_DIR)/, $(MAIN_SRC))
MAIN_OBJ = $(MAIN_SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Bonus source files (including bonus main and bonus_utils.c for alloc_pipes)
BONUS_SRC = pipex_bonus.c \
            exec_bonus.c \
            here_doc.c \
            bonus_utils.c

BONUS_SRC := $(addprefix $(BONUS_SRC_DIR)/, $(BONUS_SRC))
BONUS_OBJ = $(BONUS_SRC:$(BONUS_SRC_DIR)/%.c=$(BONUS_OBJ_DIR)/%.o)

# Default target: build mandatory version
all: $(NAME)

$(NAME): $(MAIN_OBJ) $(MAND_OBJ) $(LIBFT)
	@echo "Compiling $(NAME) (mandatory)..."
	$(CC) $(CFLAGS) -I$(HEADER_DIR) -I$(LIBFT_DIR) \
		$(MAIN_OBJ) $(MAND_OBJ) -L$(LIBFT_DIR) -lft -o $(NAME)
	@echo "Done"

# Bonus target: build bonus version (exclude mandatory main)
bonus: $(BONUS_OBJ) $(MAND_OBJ) $(LIBFT)
	@echo "Compiling $(NAME) (bonus)..."
	$(CC) $(CFLAGS) -I$(HEADER_DIR) -I$(LIBFT_DIR) \
		$(BONUS_OBJ) $(MAND_OBJ) -L$(LIBFT_DIR) -lft -o $(NAME)
	@echo "Done"

# Compile mandatory sources (excluding main)
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -I$(HEADER_DIR) -I$(LIBFT_DIR) -c $< -o $@

# Compile bonus sources
$(BONUS_OBJ_DIR)/%.o: $(BONUS_SRC_DIR)/%.c
	@mkdir -p $(BONUS_OBJ_DIR)
	@echo "Compiling bonus $<..."
	$(CC) $(CFLAGS) -I$(HEADER_DIR) -I$(LIBFT_DIR) -c $< -o $@

# Build libft
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
.PHONY: all clean fclean re bonus
