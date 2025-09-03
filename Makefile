# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/09/03 13:19:55 by adriescr          #+#    #+#              #
#    Updated: 2025/09/03 16:09:56 by agarcia          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

# Compilation flags
CFLAGS = -Wall -Wextra -Werror

# Compilator
CC = cc

# Archiver
AR = ar rcs

# Directories
SRC_DIR = src
LIB_DIR = libft
OBJ_DIR = objs

# Library files
LIB_NAME = ${LIB_DIR}/libft.a

# Main Source Project
MAIN_SOURCE = ${SRC_DIR}/main.c

# Source files
MINISHELL_SRCS = ${SRC_DIR}/main.c

# Main object files
MAIN_OBJECT = $(MAIN_SOURCE:%.c=$(OBJ_DIR)/%.o)

# Source objects

MINISHELL_OBJS = $(MINISHELL_SOURCE:%.c=$(OBJ_DIR)/%.o)

all: ${LIB_NAME} ${NAME}
	@echo "\033[36mEl ejecutable '$(NAME)' está listo para usarse.\033[0m"

# Compile the library
$(LIB_NAME):
	@$(MAKE) -C $(LIB_DIR)

# Compile the project
${NAME}: ${MAIN_OBJECT} $(MINISHELL_OBJS)
	@${CC} ${CFLAGS} ${MAIN_OBJECT} -o ${NAME} ${LIB_NAME}
	@chmod 755 $(NAME)
	@if [ $$? -eq 0 ]; then \
		echo "\033[32mCompilación finalizada correctamente.\033[0m"; \
	else \
		echo "\033[31mError durante la compilación.\033[0m"; \
	fi

# Compile object files
$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

# Clean up object files
clean:
	@rm -rf $(OBJ_DIR)
	@$(MAKE) -C $(LIB_DIR) clean
	@if [ $$? -eq 0 ]; then \
		echo "\033[32mArchivos objeto eliminados correctamente.\033[0m"; \
	else \
		echo "\033[31mError al eliminar los archivos objeto.\033[0m"; \
	fi

# Clean up object files and the executable
fclean:
	@rm -rf $(OBJ_DIR)
	@$(MAKE) -C $(LIB_DIR) fclean
	@rm -f ${NAME}
	@if [ $$? -eq 0 ]; then \
		echo "\033[32mLibrerias, objetos y ejecutable eliminados correctamente.\033[0m"; \
	else \
		echo "\033[31mError al eliminar librerias, objetos y ejecutable.\033[0m"; \
	fi

re: fclean
	@echo "\033[34mRecompilando todo...\033[0m"
	@$(MAKE) --always-make all
	@if [ $$? -eq 0 ]; then \
		echo "\033[35mRecompilación finalizada correctamente.\033[0m"; \
	else \
		echo "\033[31mError durante la recompilación.\033[0m"; \
	fi

# Phony targets
.PHONY: all clean fclean re
