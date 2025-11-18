#!/bin/bash

# Test minishell sin variables de entorno
# Colores
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

echo -e "${YELLOW}=== PROBANDO MINISHELL SIN ENTORNO ===${NC}\n"

# Función para ejecutar comandos
test_command() {
    local cmd="$1"
    local desc="$2"
    echo -e "${YELLOW}TEST:${NC} $desc"
    echo -e "${YELLOW}CMD:${NC} $cmd"
    echo "$cmd" | env -i ./minishell 2>&1 | head -20
    echo -e "\n---\n"
}

# Compilar si es necesario
if [ ! -f "./minishell" ]; then
    echo "Compilando minishell..."
    make
fi

echo -e "${GREEN}=== 1. BUILTINS BÁSICOS ===${NC}\n"

test_command "pwd" "PWD sin entorno"
test_command "env" "ENV sin entorno"
test_command "export" "EXPORT sin entorno"
test_command "echo hola mundo" "ECHO sin entorno"
test_command "cd /tmp" "CD sin entorno"
test_command "exit" "EXIT sin entorno"

echo -e "${GREEN}=== 2. EXPANSIÓN DE VARIABLES ===${NC}\n"

test_command "echo \$PATH" "Expansión PATH sin entorno"
test_command "echo \$HOME" "Expansión HOME sin entorno"
test_command "echo \$USER" "Expansión USER sin entorno"
test_command "echo \$?" "Expansión exit status"

echo -e "${GREEN}=== 3. COMANDOS EXTERNOS ===${NC}\n"

test_command "/bin/ls" "ls con ruta absoluta"
test_command "/bin/echo hola" "echo con ruta absoluta"
test_command "ls" "ls sin PATH (debería fallar)"
test_command "cat" "cat sin PATH (debería fallar)"

echo -e "${GREEN}=== 4. EXPORT Y MODIFICACIÓN DE ENTORNO ===${NC}\n"

test_command $'export PATH=/bin:/usr/bin\nls' "Exportar PATH y usar ls"
test_command $'export VAR=valor\necho $VAR' "Exportar variable y expandir"
test_command $'export A=1\nexport B=2\nenv' "Múltiples exports"

echo -e "${GREEN}=== 5. PIPES Y REDIRECCIONES ===${NC}\n"

test_command "/bin/echo hola > /tmp/test_minishell.txt" "Redirección salida"
test_command "/bin/cat < /tmp/test_minishell.txt" "Redirección entrada"
test_command "/bin/echo hola | /bin/cat" "Pipe simple"

echo -e "${GREEN}=== 6. CD Y PWD ===${NC}\n"

test_command $'cd /tmp\npwd' "CD a /tmp y PWD"
test_command $'export HOME=/tmp\ncd\npwd' "CD sin arg con HOME exportado"
test_command "cd ~" "CD a ~ sin HOME"

echo -e "${YELLOW}=== TEST COMPLETADO ===${NC}"

# Limpiar
rm -f /tmp/test_minishell.txt

echo -e "\n${YELLOW}=== AHORA PRUEBA CON VALGRIND ===${NC}"
echo "Ejecuta: echo 'exit' | env -i valgrind --leak-check=full --show-leak-kinds=all ./minishell"
