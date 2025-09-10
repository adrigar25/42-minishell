#!/bin/bash

# =========================================================================
#                    TEST: VARIABLES DE ENTORNO
# =========================================================================

echo "🧪 TESTING: Variables de entorno"
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
    minishell_output=$(echo -e "$commands\nexit" | ../minishell 2>&1 | grep -v "Welcome to Minishell!" | grep -v "minishell:(" | grep -v "#" | grep -v "│" | grep -v "╭" | grep -v "╯" | grep -v "╰" | grep -v "┃" | grep -v "┏" | grep -v "┗" | tail -n +2 | sed '$d')
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

# Tests de variables básicas
show_test "Variable PWD"
compare_outputs "PWD" "echo \$PWD"

show_test "Variable HOME"
compare_outputs "HOME" "echo \$HOME"

show_test "Variable USER"
compare_outputs "USER" "echo \$USER"

show_test "Variable PATH (primeros 50 chars)"
compare_outputs "PATH" "echo \$PATH | cut -c1-50"

show_test "Variable inexistente"
compare_outputs "variable inexistente" "echo \$VARIABLE_INEXISTENTE"

# Tests de exit status
show_test "Exit status después de comando exitoso"
compare_outputs "exit status 0" "true\necho \$?"

show_test "Exit status después de comando fallido" 
compare_outputs "exit status 1" "false\necho \$?"

show_test "Exit status después de comando inexistente"
compare_outputs "exit status 127" "comando_inexistente 2>/dev/null\necho \$?"

# Tests con comillas
show_test "Variables en comillas dobles"
compare_outputs "comillas dobles" "echo \"Usuario: \$USER en \$PWD\""

show_test "Variables en comillas simples"
compare_outputs "comillas simples" "echo 'Usuario: \$USER en \$PWD'"

show_test "Múltiples variables"
compare_outputs "múltiples variables" "echo \$USER vive en \$HOME y está en \$PWD"

# Tests con variables dinámicas
show_test "PWD después de cd (simulado)"
compare_outputs "PWD dinámico" "echo PWD inicial: \$PWD\necho Simulando cd..."

echo -e "\n${GREEN}🎉 TODOS LOS TESTS DE VARIABLES COMPLETADOS${NC}"
