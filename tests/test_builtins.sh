#!/bin/bash

# =========================================================================
#                        TEST: COMANDOS BUILTIN
# =========================================================================

echo "🧪 TESTING: Comandos builtin"
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

# Función para comparar resultados (especial para builtins)
compare_builtin() {
    local test_name="$1"
    local commands="$2"
    
    echo -e "${YELLOW}Comando(s):${NC} $commands"
    
    # Ejecutar en bash
    echo -e "\n${BLUE}🐚 BASH:${NC}"
    bash_output=$(cd /tmp && echo -e "$commands" | bash 2>&1)
    echo "$bash_output"
    
    # Ejecutar en minishell
    echo -e "\n${BLUE}🔧 MINISHELL:${NC}"
    minishell_output=$(echo -e "$commands\nexit" | ../minishell 2>&1 | grep -v "Welcome to Minishell!" | grep -v "#" | grep -v "│" | grep -v "╭" | grep -v "╯" | grep -v "╰" | grep -v "┃" | grep -v "┏" | grep -v "┗" | sed '/^$/d' | tail -n +2)

    # Filtrar solo las líneas útiles para builtins
    echo "$minishell_output" | grep -v "minishell:(" | sed '$d'
    
    echo -e "\n${GREEN}✓ Test completado${NC}"
    echo "========================================="
}

# Verificar que minishell existe
if [ ! -f "../minishell" ]; then
    echo -e "${RED}❌ Error: ../minishell no encontrado. Ejecuta 'make' primero.${NC}"
    exit 1
fi

echo -e "${GREEN}✅ Minishell encontrado${NC}\n"

# Tests de echo
show_test "Echo simple"
compare_builtin "echo simple" "echo Hello World"

show_test "Echo con flag -n"
compare_builtin "echo -n" "echo -n Hello"

show_test "Echo con múltiples argumentos"
compare_builtin "echo múltiple" "echo uno dos tres"

show_test "Echo con comillas"
compare_builtin "echo comillas" "echo \"Hello World\""

# Tests de pwd
show_test "PWD builtin"
compare_builtin "pwd" "pwd"

# Tests de env
show_test "Env (primeras 5 líneas)"
compare_builtin "env" "env | head -5"

# Test de cd (especial)
show_test "CD y PWD"
echo -e "${YELLOW}Comando(s):${NC} cd /tmp && pwd"

echo -e "\n${BLUE}🐚 BASH:${NC}"
bash_cd_output=$(bash -c "cd /tmp && pwd")
echo "$bash_cd_output"

echo -e "\n${BLUE}🔧 MINISHELL:${NC}"
minishell_cd_output=$(echo -e "cd /tmp\npwd\nexit" | ../minishell 2>&1 | grep -v "Welcome to Minishell!" | grep -v "minishell:(" | grep -v "#" | grep -v "│" | grep -v "╭" | grep -v "╯" | grep -v "╰" | grep -v "┃" | grep -v "┏" | grep -v "┗" | sed '/^$/d' | tail -n +2 | sed -n '1p')
echo "$minishell_cd_output"

echo -e "\n${GREEN}✓ Test completado${NC}"
echo "========================================="

# Test de export (básico)
show_test "Export básico"
compare_builtin "export" "export TEST_VAR=hello\necho \$TEST_VAR"

# Test de unset (básico)
show_test "Unset básico"
compare_builtin "unset" "export TEST_VAR=hello\nunset TEST_VAR\necho \$TEST_VAR"

echo -e "\n${GREEN}🎉 TODOS LOS TESTS DE BUILTINS COMPLETADOS${NC}"
