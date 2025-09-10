#!/bin/bash

# =========================================================================
#                           TEST: PIPES
# =========================================================================

echo "🧪 TESTING: Pipes"
echo "========================================="
echo -e "${YELLOW}⚠️  NOTA: Si los tests se cuelgan, hay problemas en la implementación de pipes${NC}"
echo

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

# Función para comparar resultados con timeout para pipes
compare_outputs() {
    local test_name="$1"
    local commands="$2"
    
    echo -e "${YELLOW}Comando(s):${NC} $commands"
    
    # Ejecutar en bash
    echo -e "\n${BLUE}🐚 BASH:${NC}"
    bash_output=$(echo -e "$commands" | bash 2>&1)
    echo "$bash_output"
    
    # Ejecutar en minishell con timeout manual
    echo -e "\n${BLUE}🔧 MINISHELL:${NC}"
    
    # Timeout manual para macOS
    (
        sleep 10 && echo -e "${RED}⏰ TIMEOUT - Matando minishell (problema con pipes)${NC}" && pkill -f "../minishell" 2>/dev/null
    ) &
    timeout_pid=$!
    
    minishell_output=$(echo -e "$commands\nexit" | ../minishell 2>&1 | grep -v "Welcome to Minishell!" | grep -v "minishell:(" | grep -v "#" | grep -v "│" | grep -v "╭" | grep -v "╯" | grep -v "╰" | grep -v "┃" | grep -v "┏" | grep -v "┗" | sed '/^$/d' | tail -n +2 | sed '$d' 2>/dev/null)
    
    # Matar timeout si el comando terminó
    kill $timeout_pid 2>/dev/null
    wait $timeout_pid 2>/dev/null
    
    if [ -z "$minishell_output" ]; then
        echo -e "${RED}⏰ TIMEOUT o CUELGUE - Implementación de pipes tiene problemas${NC}"
        echo -e "${YELLOW}💡 Revisa la lógica de fork/pipe/wait en tu parsing${NC}"
    else
        echo "$minishell_output"
    fi
    
    echo -e "\n${GREEN}✓ Test completado${NC}"
    echo "========================================="
}

# Verificar que minishell existe
if [ ! -f "../minishell" ]; then
    echo -e "${RED}❌ Error: ../minishell no encontrado. Ejecuta 'make' primero.${NC}"
    exit 1
fi

echo -e "${GREEN}✅ Minishell encontrado${NC}\n"

# Crear archivo de test
echo -e "línea uno\nlínea dos\nlínea tres\nlínea cuatro\nlínea cinco" > test_pipe.txt

# Tests básicos de pipes
show_test "Pipe simple (echo | cat)"
compare_outputs "pipe echo cat" "echo 'Hello World' | cat"

show_test "Pipe con wc"
compare_outputs "pipe wc" "echo 'una línea de texto' | wc -w"

show_test "Pipe con grep"
compare_outputs "pipe grep" "echo -e 'apple\nbanana\ncherry' | grep 'a'"

show_test "Pipe con head"
compare_outputs "pipe head" "cat test_pipe.txt | head -2"

show_test "Pipe con tail"
compare_outputs "pipe tail" "cat test_pipe.txt | tail -2"

show_test "Pipe múltiple (3 comandos)"
compare_outputs "pipe múltiple" "cat test_pipe.txt | head -3 | tail -1"

show_test "Pipe con sort"
compare_outputs "pipe sort" "echo -e 'zebra\napple\nbanana' | sort"

# Tests con comandos más complejos
show_test "Pipe con ls"
compare_outputs "pipe ls" "ls | head -3"

show_test "Pipe con echo múltiple"
compare_outputs "pipe echo múltiple" "echo 'uno dos tres' | tr ' ' '\n' | head -2"

# Test con error en pipe
show_test "Error en pipe"
compare_outputs "error pipe" "comando_inexistente | cat"

# Limpiar archivo de test
rm -f test_pipe.txt

echo -e "\n${GREEN}🎉 TODOS LOS TESTS DE PIPES COMPLETADOS${NC}"
