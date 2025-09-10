#!/bin/bash

# =========================================================================
#                      TEST: CASOS ESPECIALES
# =========================================================================

echo "🧪 TESTING: Casos especiales y edge cases"
echo "========================================="

# Colores para output
RED='\033[0;31m'
GREEN='\033[0;32m'
BLUE='\033[0;34m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Función para mostrar test
show_test() {
    echo -e "\n${BLUE}📋 Test: $1${NC}"
    echo "----------------------------------------"
}

# Función para comparar resultados
compare_outputs() {
    local test_name="$1"
    local commands="$2"
    
    echo -e "${YELLOW}Comando(s):${NC} $commands"
    
    # Ejecutar en bash
    echo -e "\n${BLUE}🐚 BASH:${NC}"
    bash_output=$(echo -e "$commands" | bash 2>&1)
    echo "$bash_output"
    
    # Ejecutar en minishell
    echo -e "\n${BLUE}🔧 MINISHELL:${NC}"
    minishell_output=$(echo -e "$commands\nexit" | ../minishell 2>&1 | grep -v "Welcome to Minishell!" | grep -v "minishell:(" | grep -v "#" | grep -v "│" | grep -v "╭" | grep -v "╯" | grep -v "╰" | grep -v "┃" | grep -v "┏" | grep -v "┗" | sed '/^$/d' | tail -n +2 | sed '$d')
    echo "$minishell_output"
    
    echo -e "\n${GREEN}✓ Test completado${NC}"
    echo "========================================="
}

# Verificar que minishell existe
if [ ! -f "../minishell" ]; then
    echo -e "${RED}❌ Error: ../minishell no encontrado. Ejecuta 'make' primero.${NC}"
    exit 1
fi

echo -e "${GREEN}✅ Minishell encontrado${NC}\n"

# Tests de casos especiales
show_test "Comando vacío (solo Enter)"
echo -e "${YELLOW}Comando(s):${NC} [línea vacía]"

echo -e "\n${BLUE}🐚 BASH:${NC}"
bash_empty=$(echo -e "\n" | bash 2>&1)
echo "[$bash_empty]"

echo -e "\n${BLUE}🔧 MINISHELL:${NC}"
minishell_empty=$(echo -e "\nexit" | ../minishell 2>&1 | grep -v "Welcome to Minishell!" | grep -v "minishell:(" | grep -v "#" | grep -v "│" | grep -v "╭" | grep -v "╯" | grep -v "╰" | grep -v "┃" | grep -v "┏" | grep -v "┗" | sed '/^$/d' | tail -n +2 | sed '$d')
echo "[$minishell_empty]"

echo -e "\n${GREEN}✓ Test completado${NC}"
echo "========================================="

show_test "Espacios múltiples"
compare_outputs "espacios múltiples" "echo    hello     world    "

show_test "Comillas vacías"
compare_outputs "comillas vacías" "echo '' \"\" ''"

show_test "Variables con espacios"
compare_outputs "variable con espacios" "echo \"Mi directorio es: \$PWD y mi usuario es \$USER\""

show_test "Echo sin argumentos"
compare_outputs "echo sin args" "echo"

show_test "Comando con solo espacios"
echo -e "${YELLOW}Comando(s):${NC} [solo espacios]"

echo -e "\n${BLUE}🐚 BASH:${NC}"
bash_spaces=$(echo -e "   \n" | bash 2>&1)
echo "[$bash_spaces]"

echo -e "\n${BLUE}🔧 MINISHELL:${NC}"
minishell_spaces=$(echo -e "   \nexit" | ../minishell 2>&1 | grep -v "Welcome to Minishell!" | grep -v "minishell:(" | grep -v "#" | grep -v "│" | grep -v "╭" | grep -v "╯" | grep -v "╰" | grep -v "┃" | grep -v "┏" | grep -v "┗" | sed '/^$/d' | tail -n +2 | sed '$d')
echo "[$minishell_spaces]"

echo -e "\n${GREEN}✓ Test completado${NC}"
echo "========================================="

show_test "Comando muy largo"
long_command="echo $(printf 'a%.0s' {1..100})"
compare_outputs "comando largo" "$long_command"

show_test "PATH con comando"
compare_outputs "comando en PATH" "/bin/echo hello"

show_test "Comando con ./path"
compare_outputs "comando relativo" "echo 'testing relative path'"

show_test "Variable con caracteres especiales"
compare_outputs "variable especial" "echo \$PWD/src/../README.md"

show_test "Exit status en diferentes contextos"
compare_outputs "exit status complejo" "true\necho \$?\nfalse\necho \$?\ntrue\necho \$?"

show_test "Comando con muchos argumentos"
compare_outputs "muchos argumentos" "echo arg1 arg2 arg3 arg4 arg5 arg6 arg7 arg8 arg9 arg10"

show_test "CD a directorio con espacios (si existe)"
echo -e "${YELLOW}Comando(s):${NC} cd a directorio con espacios"
echo -e "\n${BLUE}Nota:${NC} Test básico de cd a HOME"

echo -e "\n${BLUE}🐚 BASH:${NC}"
bash_cd_home=$(bash -c "cd && pwd" 2>&1)
echo "$bash_cd_home"

echo -e "\n${BLUE}🔧 MINISHELL:${NC}"
minishell_cd_home=$(echo -e "cd\npwd\nexit" | ../minishell 2>&1 | grep -v "Welcome to Minishell!" | grep -v "minishell:(" | grep -v "#" | grep -v "│" | grep -v "╭" | grep -v "╯" | grep -v "╰" | grep -v "┃" | grep -v "┏" | grep -v "┗" | sed '/^$/d' | tail -n +2 | sed '$d')
echo "$minishell_cd_home"

echo -e "\n${GREEN}✓ Test completado${NC}"
echo "========================================="

show_test "Handling de caracteres especiales en echo"
compare_outputs "caracteres especiales" "echo 'Caracteres: !@#\$%^&*()'"

echo -e "\n${GREEN}🎉 TODOS LOS TESTS DE CASOS ESPECIALES COMPLETADOS${NC}"
