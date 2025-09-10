#!/bin/bash

# =========================================================================
#                        TEST: COMANDOS BÁSICOS  
# =========================================================================

echo "🧪 TESTING: Comandos básicos (sin pipes)"
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

# Función para comparar resultados con timeout
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
    
    # Crear timeout manual para macOS
    (
        sleep 8 && pkill -f "../minishell.*$commands" 2>/dev/null
    ) &
    timeout_pid=$!
    
    minishell_output=$(echo -e "$commands\nexit" | ../minishell 2>&1 | grep -v "Welcome to Minishell!" | grep -v "#" | grep -v "│" | grep -v "╭" | grep -v "╯" | grep -v "╰" | grep -v "┃" | grep -v "┏" | grep -v "┗" | sed '/^$/d' | tail -n +2)
    
    # Matar timeout si el comando terminó
    kill $timeout_pid 2>/dev/null
    wait $timeout_pid 2>/dev/null
    
    if [ -z "$minishell_output" ]; then
        echo -e "${RED}⏰ Sin output - posible timeout o cuelgue${NC}"
    else
        echo "$minishell_output" | sed '$d'  # Eliminar última línea (compatible con macOS)
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

# Tests
show_test "Comando simple (ls)"
compare_outputs "ls simple" "ls"

show_test "Comando con argumentos (ls -la)"
compare_outputs "ls con flags" "ls -la"

show_test "Comando echo básico"
compare_outputs "echo" "echo Hello World"

show_test "Comando pwd"
compare_outputs "pwd" "pwd"

show_test "Comando que no existe"
compare_outputs "comando inexistente" "comando_que_no_existe"

show_test "Comando cat con archivo"
compare_outputs "cat README" "cat README.md"

show_test "Comando simple con argumentos"
compare_outputs "echo args" "echo uno dos tres"

echo -e "\n${GREEN}🎉 TODOS LOS TESTS DE COMANDOS BÁSICOS COMPLETADOS${NC}"
