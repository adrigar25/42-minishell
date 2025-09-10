#!/bin/bash

# =========================================================================
#                        TEST: REDIRECCIONES
# =========================================================================

echo "🧪 TESTING: Redirecciones"
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

# Función para limpiar archivos de test
cleanup() {
    rm -f test_output.txt test_input.txt test_append.txt 2>/dev/null
}

# Función para comparar archivos
compare_files() {
    local test_name="$1"
    local commands="$2"
    local output_file="$3"
    
    echo -e "${YELLOW}Comando(s):${NC} $commands"
    
    # Limpiar archivos anteriores
    cleanup
    
    # Ejecutar en bash
    echo -e "\n${BLUE}🐚 BASH:${NC}"
    echo -e "$commands" | bash 2>&1
    if [ -f "$output_file" ]; then
        echo "Contenido de $output_file:"
        cat "$output_file"
        mv "$output_file" "${output_file}.bash"
    else
        echo "Archivo $output_file no creado"
    fi
    
    # Ejecutar en minishell
    echo -e "\n${BLUE}🔧 MINISHELL:${NC}"
    echo -e "$commands\nexit" | ../minishell 2>&1 | grep -v "Welcome to Minishell!" | grep -v "minishell:(" | grep -v "#" | grep -v "│" | grep -v "╭" | grep -v "╯" | grep -v "╰" | grep -v "┃" | grep -v "┏" | grep -v "┗" >/dev/null
    
    if [ -f "$output_file" ]; then
        echo "Contenido de $output_file:"
        cat "$output_file"
        
        # Comparar archivos si ambos existen
        if [ -f "${output_file}.bash" ]; then
            echo -e "\n${YELLOW}Comparación:${NC}"
            if diff "${output_file}.bash" "$output_file" >/dev/null 2>&1; then
                echo -e "${GREEN}✅ Archivos idénticos${NC}"
            else
                echo -e "${RED}❌ Archivos diferentes${NC}"
                echo "Diferencias:"
                diff "${output_file}.bash" "$output_file" 2>/dev/null || echo "Error en diff"
            fi
        fi
    else
        echo "Archivo $output_file no creado en minishell"
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

# Crear archivo de input para tests
echo "Esta es una línea de prueba" > test_input.txt
echo "Segunda línea de prueba" >> test_input.txt

# Tests de redirección de salida
show_test "Redirección de salida (>)"
compare_files "output redirect" "echo 'Hello World' > test_output.txt" "test_output.txt"

show_test "Redirección de salida con comando"
compare_files "ls redirect" "ls README.md > test_output.txt" "test_output.txt"

show_test "Redirección append (>>)"
compare_files "append redirect" "echo 'Primera línea' > test_append.txt\necho 'Segunda línea' >> test_append.txt" "test_append.txt"

# Tests de redirección de entrada
show_test "Redirección de entrada (<)"
echo -e "${YELLOW}Comando(s):${NC} cat < test_input.txt"

echo -e "\n${BLUE}🐚 BASH:${NC}"
bash_input_output=$(cat < test_input.txt 2>&1)
echo "$bash_input_output"

echo -e "\n${BLUE}🔧 MINISHELL:${NC}"
minishell_input_output=$(echo -e "cat < test_input.txt\nexit" | ../minishell 2>&1 | grep -v "Welcome to Minishell!" | grep -v "minishell:(" | grep -v "#" | grep -v "│" | grep -v "╭" | grep -v "╯" | grep -v "╰" | grep -v "┃" | grep -v "┏" | grep -v "┗" | sed '/^$/d' | tail -n +2 | sed '$d')
echo "$minishell_input_output"

echo -e "\n${GREEN}✓ Test completado${NC}"
echo "========================================="

# Test de error de archivo inexistente
show_test "Error: archivo inexistente"
echo -e "${YELLOW}Comando(s):${NC} cat < archivo_inexistente.txt"

echo -e "\n${BLUE}🐚 BASH:${NC}"
bash_error=$(bash -c "cat < archivo_inexistente.txt" 2>&1)
echo "$bash_error"

echo -e "\n${BLUE}🔧 MINISHELL:${NC}"
minishell_error=$(echo -e "cat < archivo_inexistente.txt\nexit" | ../minishell 2>&1 | grep -v "Welcome to Minishell!" | grep -v "minishell:(" | grep -v "#" | grep -v "│" | grep -v "╭" | grep -v "╯" | grep -v "╰" | grep -v "┃" | grep -v "┏" | grep -v "┗" | sed '/^$/d' | tail -n +2 | sed '$d')
echo "$minishell_error"

echo -e "\n${GREEN}✓ Test completado${NC}"
echo "========================================="

# Limpiar archivos de test
cleanup
rm -f test_input.txt test_output.txt.bash test_append.txt.bash 2>/dev/null

echo -e "\n${GREEN}🎉 TODOS LOS TESTS DE REDIRECCIONES COMPLETADOS${NC}"
