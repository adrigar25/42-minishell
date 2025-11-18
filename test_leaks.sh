#!/bin/bash

# ================================================================
# TEST DE LEAKS - MULTIPLATAFORMA
# Usa valgrind (Linux/Docker) o leaks (macOS)
# ================================================================

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
CYAN='\033[0;36m'
NC='\033[0m'

echo -e "${CYAN}========================================${NC}"
echo -e "${CYAN}  MINISHELL LEAK TEST                 ${NC}"
echo -e "${CYAN}========================================${NC}"
echo ""

# Verificar si existe minishell
if [ ! -f "./minishell" ]; then
    echo -e "${YELLOW}Compilando minishell...${NC}"
    make > /dev/null 2>&1
    if [ $? -ne 0 ]; then
        echo -e "${RED}Error al compilar${NC}"
        exit 1
    fi
fi

# Detectar SO y herramienta disponible
if command -v valgrind > /dev/null 2>&1; then
    echo -e "${GREEN}✓ Usando valgrind${NC}"
    TOOL="valgrind"
elif [ "$(uname)" = "Darwin" ]; then
    echo -e "${YELLOW}⚠  macOS detectado - valgrind no disponible${NC}"
    echo -e "${YELLOW}   Opciones:${NC}"
    echo -e "${YELLOW}   1. Usa Docker: docker run -it -v \$(pwd):/minishell ubuntu${NC}"
    echo -e "${YELLOW}   2. Ejecuta en 42 Linux${NC}"
    echo ""
    
    # Verificar si Docker está disponible
    if command -v docker > /dev/null 2>&1; then
        echo -e "${CYAN}¿Quieres ejecutar el test en Docker? (y/n)${NC}"
        read -t 5 -n 1 -r REPLY
        echo ""
        if [[ $REPLY =~ ^[Yy]$ ]]; then
            echo -e "${YELLOW}Ejecutando en Docker...${NC}"
            docker run --rm -v "$(pwd)":/minishell -w /minishell ubuntu bash -c \
                "apt-get update -qq && apt-get install -y -qq valgrind gcc make > /dev/null 2>&1 && \
                make re > /dev/null 2>&1 && \
                ./run_leak_tests.sh"
            exit $?
        fi
    fi
    
    echo -e "${YELLOW}Ejecutando test básico sin valgrind...${NC}"
    TOOL="none"
else
    echo -e "${RED}✗ No se encontró valgrind${NC}"
    echo -e "${YELLOW}Instala valgrind: sudo apt-get install valgrind${NC}"
    exit 1
fi

# Test básico sin valgrind (macOS)
if [ "$TOOL" = "none" ]; then
    echo ""
    echo -e "${CYAN}Test funcional (sin análisis de memoria):${NC}"
    
    ./minishell << 'EOF'
echo "=== Test básico ==="
pwd
export TEST=valor
echo $TEST
cd /tmp
pwd
echo "test" > /tmp/test.txt
cat < /tmp/test.txt
rm /tmp/test.txt
echo "pipe test" | cat
exit
EOF
    
    if [ $? -eq 0 ]; then
        echo ""
        echo -e "${GREEN}✓ Test funcional PASADO${NC}"
        echo -e "${YELLOW}⚠  Para test completo de leaks, usa Linux/Docker${NC}"
    else
        echo ""
        echo -e "${RED}✗ Test funcional FALLÓ${NC}"
        exit 1
    fi
    
    echo ""
    echo -e "${CYAN}========================================${NC}"
    echo -e "${CYAN}  RESUMEN:${NC}"
    echo -e "${CYAN}  - Funcionalidad: ✓ OK${NC}"
    echo -e "${CYAN}  - Leaks: Requiere valgrind (Linux)${NC}"
    echo -e "${CYAN}========================================${NC}"
    
    exit 0
fi

# Test con valgrind (Linux)
if [ "$TOOL" = "valgrind" ]; then
    echo ""
    echo -e "${YELLOW}Ejecutando suite completa de tests...${NC}"
    ./run_leak_tests.sh
    exit $?
fi
