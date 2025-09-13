# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/09/03 13:19:55 by adriescr          #+#    #+#              #
#    Updated: 2025/09/13 20:20:38 by adriescr         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

#
# Compilation flags (use Homebrew readline/ncurses for macOS ARM64)
CFLAGS = -I/opt/homebrew/opt/readline/include
#
# Linker flags (use Homebrew readline/ncurses for macOS ARM64)
LDFLAGS = -L/opt/homebrew/opt/readline/lib -lreadline -lncurses

# Compilator
CC = cc

# Archiver
AR = ar rcs

# Directories
SRC_DIR = src
UTILS_DIR = $(SRC_DIR)/utils
LIBS_DIR = libs
LIBFT_DIR = $(LIBS_DIR)/libft
OBJ_DIR = objs

# Library files
LIB_NAME = $(LIBFT_DIR)/libft.a

# Main Source Project
MAIN_SOURCE = $(SRC_DIR)/main.c

# Source files
MINISHELL_SRCS = \
	$(SRC_DIR)/ft_minishell.c \
	$(UTILS_DIR)/ft_msg_start.c \
	$(UTILS_DIR)/ft_search_file/ft_is_dot_or_dotdot.c \
	$(UTILS_DIR)/ft_search_file/ft_search_file.c \
	$(UTILS_DIR)/ft_search_file/ft_search_in_dir.c \
	$(UTILS_DIR)/ft_search_file/ft_search_in_subdirs.c \
	$(UTILS_DIR)/ft_search_file/ft_build_path.c \
	$(UTILS_DIR)/ft_count_args.c \
	$(UTILS_DIR)/ft_redir_io.c \
	$(UTILS_DIR)/ft_free_char_array.c \
	$(UTILS_DIR)/input/ft_read_input.c \
	$(UTILS_DIR)/input/ft_process_input.c \
	$(UTILS_DIR)/debug/ft_show_debug.c \
	$(UTILS_DIR)/prompt/ft_get_directory_path.c \
	$(UTILS_DIR)/prompt/ft_generate_prompt.c \
	$(UTILS_DIR)/env/ft_cpyenv.c \
	$(UTILS_DIR)/env/ft_getenv.c \
	$(UTILS_DIR)/env/ft_setenv.c \
	$(UTILS_DIR)/redirections/ft_handle_infile.c \
	$(UTILS_DIR)/redirections/ft_handle_outfile.c \
	$(UTILS_DIR)/execution/ft_exec_cmd.c \
	$(UTILS_DIR)/execution/ft_get_cmd_path.c \
	$(UTILS_DIR)/execution/ft_close_unused_fds.c \
	$(UTILS_DIR)/execution/ft_finish_execution.c \
	$(UTILS_DIR)/execution/ft_execute_error_command.c \
	$(UTILS_DIR)/execution/ft_execute_pipeline.c \
	$(UTILS_DIR)/execution/ft_handle_heredoc.c \
	$(UTILS_DIR)/parsing/ft_skip_quotes.c \
	$(UTILS_DIR)/parsing/ft_split_input.c \
	$(UTILS_DIR)/parsing/ft_parse_input.c \
	$(UTILS_DIR)/parsing/ft_handle_env_expansion.c \
	$(UTILS_DIR)/parsing/ft_remove_quotes.c \
	$(UTILS_DIR)/parsing/ft_handle_wildcards.c \
	$(UTILS_DIR)/parsing/ft_syntax_check.c \
	$(UTILS_DIR)/signals/ft_sigint_handler.c \
	$(UTILS_DIR)/signals/ft_init_signals.c \
	$(UTILS_DIR)/builtins/ft_echo.c \
	$(UTILS_DIR)/builtins/ft_cd.c \
	$(UTILS_DIR)/builtins/ft_pwd.c \
	$(UTILS_DIR)/builtins/ft_export.c \
	$(UTILS_DIR)/builtins/ft_unset.c \
	$(UTILS_DIR)/builtins/ft_env.c \
	$(UTILS_DIR)/builtins/ft_exit.c \
	$(UTILS_DIR)/builtins/ft_handle_builtins.c \

# Main object files
MAIN_OBJECT = $(MAIN_SOURCE:%.c=$(OBJ_DIR)/%.o)

# Source objects

MINISHELL_OBJS = $(MINISHELL_SRCS:%.c=$(OBJ_DIR)/%.o)

all: $(LIB_NAME) $(NAME)
	@echo "\033[35m$(NAME) executable is ready to use!\033[0m"

# Compile the library
$(LIB_NAME):
	@$(MAKE) -C $(LIBFT_DIR)

# Compile the project
$(NAME): $(MINISHELL_OBJS) $(MAIN_OBJECT)
	@$(CC) $(CFLAGS) $(MINISHELL_OBJS) $(MAIN_OBJECT) $(LIB_NAME) $(LDFLAGS) -o $(NAME)
	@if [ $$? -eq 0 ]; then \
		echo "\033[32m✅ Compilation completed successfully!\033[0m"; \
	else \
		echo "\033[31m❌ Error during compilation!\033[0m"; \
	fi

# Compile object files
$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

# Clean up object files
clean:
	@rm -rf $(OBJ_DIR)
	@$(MAKE) -C $(LIBFT_DIR) clean
	@if [ $$? -eq 0 ]; then \
		echo "\033[32m🧹 Object files cleaned successfully!\033[0m"; \
	else \
		echo "\033[31m❌ Error cleaning object files!\033[0m"; \
	fi

# Clean up object files and the executable
fclean:
	@rm -rf $(OBJ_DIR)
	@$(MAKE) -C $(LIBFT_DIR) fclean
	@rm -f $(NAME)
	@if [ $$? -eq 0 ]; then \
		echo "\033[32m🗑️  Libraries, objects and executable cleaned successfully!\033[0m"; \
	else \
		echo "\033[31m❌ Error cleaning libraries, objects and executable!\033[0m"; \
	fi

re: fclean
	@echo "\033[34m🔄 Recompiling everything...\033[0m"
	@$(MAKE) --always-make all
	@if [ $$? -eq 0 ]; then \
		echo "\033[38;5;208m🎉 Recompilation completed successfully!\033[0m"; \
	else \
		echo "\033[31m❌ Error during recompilation!\033[0m"; \
	fi

# Phony targets
.PHONY: all clean fclean re
