#!/bin/bash

# ================================================================
# TEST DE LEAKS PARA macOS (usando 'leaks' nativo)
# ================================================================

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
CYAN='\033[0;36m'
NC='\033[0m'

echo -e "${CYAN}========================================${NC}"
echo -e "${CYAN}  MINISHELL LEAK TEST (macOS)         ${NC}"
echo -e "${CYAN}========================================${NC}"
echo ""

if [ ! -f "./minishell" ]; then
    echo -e "${YELLOW}Compilando minishell...${NC}"
    make > /dev/null 2>&1
fi

echo -e "${YELLOW}Ejecutando minishell en background...${NC}"

# Ejecutar minishell en background
./minishell << 'EOF' &
echo "test"
export VAR=valor
echo $VAR
pwd
cd /tmp
pwd
echo "redirect" > /tmp/test.txt
cat < /tmp/test.txt
rm /tmp/test.txt
echo "pipe" | cat
exit
EOF

PID=$!
sleep 2

echo -e "${YELLOW}Verificando leaks con PID: $PID${NC}"
echo ""

# Usar leaks de macOS
if command -v leaks > /dev/null; then
    leaks $PID > leak_macos.log 2>&1
    
    if grep -q "0 leaks for 0 total leaked bytes" leak_macos.log; then
        echo -e "${GREEN}✓ Sin leaks detectados${NC}"
        cat leak_macos.log | grep "Process"
        cat leak_macos.log | grep "leaks for"
    else
        echo -e "${RED}✗ Leaks detectados${NC}"
        echo ""
        cat leak_macos.log | grep -A 5 "LEAK:"
    fi
    
    echo ""
    echo -e "${CYAN}Log completo guardado en: leak_macos.log${NC}"
else
    echo -e "${YELLOW}⚠  Comando 'leaks' no disponible${NC}"
    echo -e "${YELLOW}   Instala Xcode Command Line Tools${NC}"
fi

# Esperar a que termine
wait $PID 2>/dev/null

echo ""
echo -e "${CYAN}========================================${NC}"
echo -e "${CYAN}   PARA TESTS COMPLETOS:${NC}"
echo -e "${CYAN}   Usa Docker o Linux con valgrind${NC}"
echo -e "${CYAN}========================================${NC}"
