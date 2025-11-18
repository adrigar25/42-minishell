#!/bin/bash

# =============================================================================
# QUICK LEAK TEST - Test rápido para desarrollo
# =============================================================================

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
CYAN='\033[0;36m'
NC='\033[0m'

echo -e "${CYAN}Quick Leak Test${NC}"

if [ ! -f "./minishell" ]; then
    echo -e "${YELLOW}Compilando...${NC}"
    make > /dev/null 2>&1
fi

echo -e "${YELLOW}Ejecutando test rápido...${NC}"

valgrind --leak-check=full --show-leak-kinds=all --track-fds=yes \
    --log-file=quick_test.log \
    ./minishell << 'EOF' > /dev/null 2>&1
echo "Quick test"
export VAR=test
echo $VAR
pwd
cd /tmp
cd -
echo "redirect" > /tmp/quick.txt
cat < /tmp/quick.txt
rm /tmp/quick.txt
echo "pipe" | cat
cat << END
heredoc
END
exit
EOF

echo ""
if grep -q "definitely lost: 0 bytes" quick_test.log && \
   grep -q "indirectly lost: 0 bytes" quick_test.log; then
    echo -e "${GREEN}✓ Sin leaks críticos${NC}"
    
    # Verificar FDs
    FD_COUNT=$(grep -c "Open file descriptor" quick_test.log)
    if [ $FD_COUNT -gt 0 ]; then
        echo -e "${YELLOW}⚠ Advertencia: $FD_COUNT file descriptor(s) sin cerrar${NC}"
    fi
else
    echo -e "${RED}✗ Leaks detectados${NC}"
    echo ""
    grep "definitely lost:" quick_test.log
    grep "indirectly lost:" quick_test.log
    grep "possibly lost:" quick_test.log
    echo ""
    echo -e "${YELLOW}Ver detalles en: quick_test.log${NC}"
fi
