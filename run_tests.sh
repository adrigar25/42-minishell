#!/bin/bash

# =========================================================================
#                    MINISHELL - MASTER TEST RUNNER
# =========================================================================

echo "🧪 MINISHELL TESTING SUITE"
echo "========================================="

# Colores para output
RED='\033[0;31m'
GREEN='\033[0;32m'
BLUE='\033[0;34m'
YELLOW='\033[1;33m'
CYAN='\033[0;36m'
BOLD='\033[1m'
NC='\033[0m' # No Color

# Banner
echo -e "${CYAN}${BOLD}"
echo "╔══════════════════════════════════════════════════════════════╗"
echo "║                    MINISHELL TEST SUITE                     ║"
echo "║                                                              ║"
echo "║  Conjunto completo de tests para verificar funcionalidad    ║"
echo "║  comparando comportamiento con bash real                    ║"
echo "╚══════════════════════════════════════════════════════════════╝"
echo -e "${NC}"

# Verificar que minishell existe
if [ ! -f "./minishell" ]; then
    echo -e "${RED}❌ Error: ./minishell no encontrado${NC}"
    echo -e "${YELLOW}💡 Ejecuta 'make' primero para compilar minishell${NC}"
    exit 1
fi

echo -e "${GREEN}✅ Minishell encontrado${NC}\n"

# Lista de tests disponibles
tests=(
    "test_basic_commands.sh:Comandos básicos"
    "test_environment_vars.sh:Variables de entorno"
    "test_builtins.sh:Comandos builtin"
    "test_redirections.sh:Redirecciones"
    "test_pipes.sh:Pipes"
    "test_signals.sh:Signals"
    "test_edge_cases.sh:Casos especiales"
)

# Función para mostrar menú
show_menu() {
    echo -e "${BOLD}📋 TESTS DISPONIBLES:${NC}"
    echo "----------------------------------------"
    
    for i in "${!tests[@]}"; do
        IFS=':' read -r script_name description <<< "${tests[$i]}"
        if [ -f "tests/$script_name" ]; then
            status="${GREEN}✅${NC}"
        else
            status="${RED}❌${NC}"
        fi
        echo -e "$status  $((i+1)). $description"
    done
    
    echo
    echo -e "${BOLD}⚡ OPCIONES ESPECIALES:${NC}"
    echo "  A. Ejecutar TODOS los tests"
    echo "  C. Compilar minishell"
    echo "  Q. Salir"
    echo
}

# Función para ejecutar un test
run_test() {
    local script_name="$1"
    local description="$2"
    
    echo -e "\n${CYAN}${BOLD}🚀 EJECUTANDO: $description${NC}"
    echo "================================================================"
    
    if [ -f "tests/$script_name" ]; then
        chmod +x "tests/$script_name"
        cd tests && ./"$script_name" && cd ..
        echo -e "\n${GREEN}✅ Test '$description' completado${NC}"
    else
        echo -e "${RED}❌ Error: tests/$script_name no encontrado${NC}"
    fi
    
    echo "================================================================"
}

# Función para ejecutar todos los tests
run_all_tests() {
    echo -e "\n${CYAN}${BOLD}🔥 EJECUTANDO TODOS LOS TESTS${NC}"
    echo "================================================================"
    
    for test in "${tests[@]}"; do
        IFS=':' read -r script_name description <<< "$test"
        if [ -f "tests/$script_name" ]; then
            run_test "$script_name" "$description"
            echo -e "\n${YELLOW}Presiona Enter para continuar con el siguiente test...${NC}"
            read -r
        else
            echo -e "${RED}❌ Saltando '$description' - archivo no encontrado${NC}"
        fi
    done
    
    echo -e "\n${GREEN}${BOLD}🎉 TODOS LOS TESTS COMPLETADOS${NC}"
}

# Función para compilar minishell
compile_minishell() {
    echo -e "\n${BLUE}🔨 COMPILANDO MINISHELL...${NC}"
    if make re; then
        echo -e "${GREEN}✅ Compilación exitosa${NC}"
    else
        echo -e "${RED}❌ Error en compilación${NC}"
    fi
}

# Crear directorio de tests si no existe
mkdir -p tests

# Loop principal
while true; do
    clear
    echo -e "${CYAN}${BOLD}🧪 MINISHELL TEST SUITE${NC}\n"
    show_menu
    
    echo -e "${YELLOW}Selecciona una opción:${NC} "
    read -r choice
    
    case $choice in
        [1-7])
            index=$((choice-1))
            if [ $index -lt ${#tests[@]} ]; then
                IFS=':' read -r script_name description <<< "${tests[$index]}"
                run_test "$script_name" "$description"
                echo -e "\n${YELLOW}Presiona Enter para continuar...${NC}"
                read -r
            else
                echo -e "${RED}❌ Opción inválida${NC}"
            fi
            ;;
        [Aa])
            run_all_tests
            echo -e "\n${YELLOW}Presiona Enter para volver al menú...${NC}"
            read -r
            ;;
        [Cc])
            compile_minishell
            echo -e "\n${YELLOW}Presiona Enter para continuar...${NC}"
            read -r
            ;;
        [Qq])
            echo -e "\n${GREEN}👋 ¡Hasta luego!${NC}"
            exit 0
            ;;
        *)
            echo -e "${RED}❌ Opción inválida. Presiona Enter para continuar...${NC}"
            read -r
            ;;
    esac
done
