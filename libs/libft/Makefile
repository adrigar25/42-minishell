# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/09/03 13:25:07 by adriescr          #+#    #+#              #
#    Updated: 2025/09/09 23:27:31 by agarcia          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft.a

# Compiler
CC = cc
# Archiver
AR = ar rcs
# Compilation flags
CFLAGS = -Wall -Wextra -Werror

# Directories
GET_NEXT_LINE_DIR = get_next_line
# File utils
FILES_DIR = files

# File utils functions
FILE_UTILS = \
	$(FILES_DIR)/ft_open_file_read.c \
	$(FILES_DIR)/ft_open_file_write.c \
	$(FILES_DIR)/ft_print_file.c \
	$(FILES_DIR)/ft_close_file.c \
	$(FILES_DIR)/ft_putstr_fd.c \
	$(FILES_DIR)/ft_putchar_fd.c \

# My libft functions
MY_LIBFT = \
	ft_is_digit.c \
	ft_putstr.c \
	ft_putstr_error.c \
	ft_putchar_error.c \
	ft_putchar.c \
	ft_is_number.c \
	ft_atoi.c \
	ft_split.c \
	ft_count_words.c \
	ft_memcpy.c \
	ft_strlen.c \
	ft_strdup.c \
	ft_strjoin.c \
	ft_substr.c \
	ft_strchr.c \
	ft_strcmp.c \
	ft_strncmp.c \
	ft_rand.c \
	ft_putnbr.c \
	ft_strstr.c \
	ft_strcpy.c \
	ft_strlcpy.c \
	ft_skip_whitespace.c \
	ft_isspace.c \
	$(GET_NEXT_LINE_DIR)/get_next_line.c \
	$(FILE_UTILS) \

# OBJECTS
OBJS = $(MY_LIBFT:.c=.o)

# Targets
all: $(NAME)

# Compile the library
$(NAME): $(OBJS)
	@echo "\033[34mCompilando ${NAME}...\033[0m"
	@$(AR) $(NAME) $(OBJS)
	@chmod 755 $(NAME)
	@if [ $$? -ne 0 ]; then \
		echo "\033[31mError durante la compilación de la libreria ${NAME}.\033[0m"; \
	fi

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -f $(OBJS)
	@if [ $$? -ne 0 ]; then \
		echo "\033[31mError al eliminar los archivos objeto.\033[0m"; \
	fi

fclean:
	@rm -f $(OBJS)
	@rm -f $(NAME)
	@if [ $$? -ne 0 ]; then \
		echo "\033[31mError al eliminar los archivos objeto y biblioteca.\033[0m"; \
	fi

re: fclean all

# Phony targets
.PHONY: all clean fclean re
