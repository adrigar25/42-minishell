# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/09/03 13:19:55 by adriescr          #+#    #+#              #
#    Updated: 2025/09/28 18:23:32 by adriescr         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Makefile for minishell project
# Makefile para el proyecto minishell
NAME := minishell

# Compiler and archiver (English): Define the C compiler and archiver to use.
# Compilador y archivador (Español): Define el compilador de C y el archivador a usar.
CC := cc
AR := ar rcs

# Directories (English): Locations of source, object and library directories used by the Makefile.
# Directorios (Español): Ubicaciones de los directorios de código fuente, objetos y librerías usados por el Makefile.
SRC_DIR := src
MANDATORY_DIR := $(SRC_DIR)/mandatory
BONUS_DIR := $(SRC_DIR)/bonus
LIBS_DIR := libs
LIBFT_DIR := $(LIBS_DIR)/libft
OBJ_DIR := objs

# Readline detection (English): Try to locate Homebrew's readline installation; fall back to a common macOS path.
# Detección de readline (Español): Intenta localizar readline instalado por Homebrew; si no, usa una ruta común en macOS.
BREW_READLINE := $(shell brew --prefix readline 2>/dev/null || true)
ifeq ($(BREW_READLINE),)
READLINE_INC := /opt/homebrew/opt/readline/include
READLINE_LIB := /opt/homebrew/opt/readline/lib
else
READLINE_INC := $(BREW_READLINE)/include
READLINE_LIB := $(BREW_READLINE)/lib
endif

CFLAGS := -Wall -Wextra -Werror -I$(LIBFT_DIR) -I$(READLINE_INC)
LDFLAGS := -L$(READLINE_LIB) -lreadline -lncurses

# Quiet mode (English): Silence compiler/linker commands by default. Use VERBOSE=1 to print full commands.
# Modo silencioso (Español): Silencia los comandos del compilador/enlazador por defecto. Usar VERBOSE=1 para mostrarlos.
ifeq ($(VERBOSE),1)
Q :=
else
Q := @
endif

# Sources discovery (English): Find .c sources under mandatory and bonus directories and keep lists sorted.
# Descubrimiento de fuentes (Español): Encuentra archivos .c bajo los directorios mandatory y bonus y mantiene las listas ordenadas.
SRCS_M := $(shell find $(MANDATORY_DIR) -type f -name "*.c" | sort)
SRCS_B := $(shell [ -d $(BONUS_DIR) ] && find $(BONUS_DIR) -type f -name "*.c" | sort || true)

# Object layout (English): Preserve the source directory structure when placing .o files under objs/.
# Estructura de objetos (Español): Conserva la estructura de directorios de las fuentes al colocar los .o bajo objs/.
OBJS_M := $(patsubst $(MANDATORY_DIR)/%.c,$(OBJ_DIR)/mandatory/%.o,$(SRCS_M))
OBJS_B := $(patsubst $(BONUS_DIR)/%.c,$(OBJ_DIR)/bonus/%.o,$(SRCS_B))

# Libft (English): Path to the libft static library.
# Libft (Español): Ruta a la librería estática libft.
LIBFT := $(LIBFT_DIR)/libft.a

# Color codes for output (English): Define color codes for terminal output messages.
# Códigos de color para salida (Español): Define códigos de color para mensajes en terminal.
RED := \033[31m
GRN := \033[32m
YEL := \033[33m
BLU := \033[34m
RST := \033[0m

# all: Build both libft and the mandatory executable (English)
# all: Compila tanto libft como el ejecutable obligatorio (Español)
all: $(LIBFT) $(NAME)

# Mandatory build (English): Build the mandatory executable by linking its objects and libft.
# Compilación obligatoria (Español): Compila el ejecutable obligatorio enlazando sus objetos y libft.
$(NAME): $(OBJS_M)
	@echo "$(BLU)==> Linking $(NAME)$(RST)"
	$(Q)$(CC) $(CFLAGS) $(OBJS_M) $(LIBFT) $(LDFLAGS) -o $(NAME) || (printf "$(RED)❌ Linking $(NAME) failed$(RST)\\n" >&2; exit 1)
	@echo "$(GRN)✅ $(NAME) built$(RST)"

# Bonus build (if there are bonus sources)
bonus: $(LIBFT) $(OBJS_B)
	@if [ -z "$(SRCS_B)" ]; then \
		echo "$(YEL)No bonus sources found. Nothing to build.$(RST)"; exit 0; \
	fi
	@echo "$(BLU)==> Linking $(NAME)_bonus$(RST)"
	$(Q)$(CC) $(CFLAGS) $(OBJS_B) $(LIBFT) $(LDFLAGS) -o $(NAME)_bonus || (printf "$(RED)❌ Linking $(NAME)_bonus failed$(RST)\\n" >&2; exit 1)
	@echo "$(GRN)✅ $(NAME)_bonus built$(RST)"

# Build libft (English): Delegate building the libft library to its own Makefile in libs/libft.
# Compilar libft (Español): Delegar la compilación de la librería libft a su propio Makefile en libs/libft.
$(LIBFT):
	@echo "$(BLU)==> Building libft...$(RST)"
	@$(MAKE) -C $(LIBFT_DIR) || (printf "$(RED)❌ Building libft failed$(RST)\\n" >&2; exit 1)
	@echo "$(GRN)✅ libft ready$(RST)"

# Object rules (English): Ensure target directory exists, then compile the source file to an object.
# Reglas de objetos (Español): Asegura que el directorio destino exista y compila la fuente a un objeto.
$(OBJ_DIR)/mandatory/%.o: $(MANDATORY_DIR)/%.c
	@mkdir -p $(dir $@)
	$(Q)$(CC) $(CFLAGS) -c $< -o $@ || (printf "$(RED)❌ Compiling $< failed$(RST)\\n" >&2; exit 1)

# Bonus object rules (English): Same as above but for bonus sources.
# Reglas de objetos bonus (Español): Igual que arriba pero para fuentes bonus.
$(OBJ_DIR)/bonus/%.o: $(BONUS_DIR)/%.c
	@mkdir -p $(dir $@)
	$(Q)$(CC) $(CFLAGS) -c $< -o $@ || (printf "$(RED)❌ Compiling $< failed$(RST)\\n" >&2; exit 1)

# clean (English): Remove all generated object directories and invoke libft clean.
# clean (Español): Elimina los directorios de objetos generados e invoca clean de libft.
clean:
	@echo "$(YEL)==> Cleaning objects...$(RST)"
	@rm -rf $(OBJ_DIR)
	@$(MAKE) -C $(LIBFT_DIR) clean || true
	@echo "$(GRN)✅ Cleaned$(RST)"

fclean: clean
	@echo "$(YEL)==> Removing executables...$(RST)"
	@rm -f $(NAME) $(NAME)_bonus
	@$(MAKE) -C $(LIBFT_DIR) fclean || true
	@echo "$(GRN)✅ Removed binaries$(RST)"

# re: clean and rebuild the mandatory executable (English)
# re: clean y recompilar el ejecutable obligatorio (Español)
re: fclean all

# rebonus: clean and rebuild the bonus executable (English)
# rebonus: limpiar y recompilar el ejecutable bonus (Español)
rebonus: fclean bonus

# show-vars: Display current Makefile variable values for debugging (English)
# show-vars: Muestra los valores actuales de las variables del Makefile para depuración (Español)
show-vars:
	@echo "NAME: $(NAME)"
	@echo "CC: $(CC)"
	@echo "CFLAGS: $(CFLAGS)"
	@echo "LDFLAGS: $(LDFLAGS)"
	@echo "SRCS_M count: $(words $(SRCS_M))"
	@echo "SRCS_B count: $(words $(SRCS_B))"

# Helpful default message when running make without args
# Ayuda por defecto al ejecutar make sin argumentos
ifndef MAKECMDGOALS
MAKECMDGOALS := all
endif

# Phony targets (English): Declare phony targets to avoid conflicts with files of the same name.
# Objetivos phony (Español): Declarar objetivos phony para evitar conflictos con archivos del mismo nombre.
.PHONY: all bonus libft clean fclean re show-vars rebonus
