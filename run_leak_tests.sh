#!/bin/bash

# =============================================================================
# MINISHELL LEAK DETECTION TEST SUITE
# Script automático para detectar memory leaks en todos los flujos de ejecución
# =============================================================================

# Colores
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
MAGENTA='\033[0;35m'
CYAN='\033[0;36m'
NC='\033[0m' # No Color

echo -e "${CYAN}========================================${NC}"
echo -e "${CYAN}  MINISHELL LEAK DETECTION TEST SUITE  ${NC}"
echo -e "${CYAN}========================================${NC}"

# Verificar que existe el ejecutable o compilar
if [ ! -f "./minishell" ]; then
    echo -e "\n${YELLOW}Compilando minishell...${NC}"
    make re
    if [ $? -ne 0 ]; then
        echo -e "${RED}Error de compilación${NC}"
        exit 1
    fi
else
    echo -e "\n${GREEN}Usando minishell existente${NC}"
fi

# Directorio de logs
mkdir -p leak_test_logs
LOGDIR="leak_test_logs"
TIMESTAMP=$(date +%Y%m%d_%H%M%S)

echo -e "${BLUE}Logs se guardarán en: ${LOGDIR}/${NC}"
echo ""

# =============================================================================
# Test 1: Builtins básicos
# =============================================================================
echo -e "${YELLOW}[Test 1/10] Builtins básicos...${NC}"
valgrind --leak-check=full --show-leak-kinds=all --track-fds=yes \
    --log-file="${LOGDIR}/01_builtins_${TIMESTAMP}.log" \
    ./minishell << 'EOF' > /dev/null 2>&1
echo "test echo"
pwd
cd /tmp
cd -
export VAR=valor
echo $VAR
unset VAR
env | head -1 > /dev/null
exit
EOF

# =============================================================================
# Test 2: Echo con variantes
# =============================================================================
echo -e "${YELLOW}[Test 2/10] Echo con diferentes opciones...${NC}"
valgrind --leak-check=full --show-leak-kinds=all \
    --log-file="${LOGDIR}/02_echo_variants_${TIMESTAMP}.log" \
    ./minishell << 'EOF' > /dev/null 2>&1
echo "hola mundo"
echo -n "sin newline"
echo -n -n -n "múltiples -n"
echo $USER
echo "$USER"
echo '$USER'
echo "comillas 'mixtas'"
echo 'comillas "mixtas"'
exit
EOF

# =============================================================================
# Test 3: CD con diferentes casos
# =============================================================================
echo -e "${YELLOW}[Test 3/10] CD con diferentes rutas...${NC}"
valgrind --leak-check=full --show-leak-kinds=all \
    --log-file="${LOGDIR}/03_cd_cases_${TIMESTAMP}.log" \
    ./minishell << 'EOF' > /dev/null 2>&1
pwd
cd /tmp
pwd
cd /usr
cd -
cd ~
cd ..
cd .
cd $HOME
exit
EOF

# =============================================================================
# Test 4: Export y Unset
# =============================================================================
echo -e "${YELLOW}[Test 4/10] Export y Unset...${NC}"
valgrind --leak-check=full --show-leak-kinds=all \
    --log-file="${LOGDIR}/04_export_unset_${TIMESTAMP}.log" \
    ./minishell << 'EOF' > /dev/null 2>&1
export VAR1=valor1
export VAR2="valor con espacios"
export VAR3=
echo $VAR1 $VAR2 $VAR3
unset VAR1
echo $VAR1
unset VAR2 VAR3
export A=1 B=2 C=3
echo $A $B $C
unset A B C
exit
EOF

# =============================================================================
# Test 5: Redirecciones de Input
# =============================================================================
echo -e "${YELLOW}[Test 5/10] Redirecciones de Input...${NC}"
echo "contenido de prueba" > /tmp/minishell_test_in.txt
valgrind --leak-check=full --show-leak-kinds=all --track-fds=yes \
    --log-file="${LOGDIR}/05_redirects_input_${TIMESTAMP}.log" \
    ./minishell << 'EOF' > /dev/null 2>&1
cat < /tmp/minishell_test_in.txt
exit
EOF
rm -f /tmp/minishell_test_in.txt

# =============================================================================
# Test 6: Redirecciones de Output y Append
# =============================================================================
echo -e "${YELLOW}[Test 6/10] Redirecciones de Output y Append...${NC}"
valgrind --leak-check=full --show-leak-kinds=all --track-fds=yes \
    --log-file="${LOGDIR}/06_redirects_output_${TIMESTAMP}.log" \
    ./minishell << 'EOF' > /dev/null 2>&1
echo "línea 1" > /tmp/minishell_test_out.txt
cat /tmp/minishell_test_out.txt
echo "línea 2" >> /tmp/minishell_test_out.txt
cat /tmp/minishell_test_out.txt
echo "sobrescribir" > /tmp/minishell_test_out.txt
cat /tmp/minishell_test_out.txt
exit
EOF
rm -f /tmp/minishell_test_out.txt

# =============================================================================
# Test 7: Pipes simples y múltiples
# =============================================================================
echo -e "${YELLOW}[Test 7/10] Pipes simples y múltiples...${NC}"
valgrind --leak-check=full --show-leak-kinds=all \
    --log-file="${LOGDIR}/07_pipes_${TIMESTAMP}.log" \
    ./minishell << 'EOF' > /dev/null 2>&1
echo "test pipe" | cat
echo "hola mundo" | grep mundo
echo "múltiples" | cat | cat | cat
ls -la | grep minishell | wc -l
echo "test" | cat | cat | cat | cat
exit
EOF

# =============================================================================
# Test 8: Heredocs
# =============================================================================
echo -e "${YELLOW}[Test 8/10] Heredocs...${NC}"
valgrind --leak-check=full --show-leak-kinds=all --track-fds=yes \
    --log-file="${LOGDIR}/08_heredocs_${TIMESTAMP}.log" \
    ./minishell << 'EOF' > /dev/null 2>&1
cat << END
línea 1
línea 2
línea 3
END
export VAR=test_heredoc
cat << END
Variable: $VAR
END
exit
EOF

# =============================================================================
# Test 9: Expansión de variables y exit status
# =============================================================================
echo -e "${YELLOW}[Test 9/10] Expansión de variables y exit status...${NC}"
valgrind --leak-check=full --show-leak-kinds=all \
    --log-file="${LOGDIR}/09_expansion_${TIMESTAMP}.log" \
    ./minishell << 'EOF' > /dev/null 2>&1
export VAR=valor_test
echo $VAR
echo "$VAR"
echo '$VAR'
echo $NOEXISTE
ls > /dev/null
echo $?
comandoinexistente 2>/dev/null
echo $?
export A=B
export B=C
echo $A $B
exit
EOF

# =============================================================================
# Test 10: Test combinado exhaustivo
# =============================================================================
echo -e "${YELLOW}[Test 10/10] Test combinado exhaustivo...${NC}"
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --track-fds=yes \
    --log-file="${LOGDIR}/10_combined_${TIMESTAMP}.log" \
    ./minishell << 'EOF' > /dev/null 2>&1
echo "Iniciando test combinado"
pwd
cd /tmp
cd -
export VAR1=test
export VAR2="con espacios"
echo $VAR1 $VAR2
echo "redirect" > /tmp/minitest.txt
cat < /tmp/minitest.txt
echo "append" >> /tmp/minitest.txt
cat /tmp/minitest.txt
echo "pipe test" | cat
cat << END
heredoc test
$VAR1
END
ls > /dev/null
echo $?
unset VAR1 VAR2
rm /tmp/minitest.txt
exit
EOF

# =============================================================================
# Análisis de resultados
# =============================================================================
echo ""
echo -e "${CYAN}========================================${NC}"
echo -e "${CYAN}  ANÁLISIS DE RESULTADOS  ${NC}"
echo -e "${CYAN}========================================${NC}"
echo ""

LEAK_COUNT=0
TOTAL_TESTS=10

for i in $(seq -f "%02g" 1 $TOTAL_TESTS); do
    logfile="${LOGDIR}/${i}_*_${TIMESTAMP}.log"
    logfile=$(ls $logfile 2>/dev/null | head -1)
    
    if [ -z "$logfile" ]; then
        continue
    fi
    
    TEST_NAME=$(basename "$logfile" | sed "s/_${TIMESTAMP}.log//")
    
    # Verificar diferentes tipos de leaks
    DEFINITELY_LOST=$(grep "definitely lost:" "$logfile" | grep -oE "[0-9,]+ bytes" | head -1 | tr -d ',' | awk '{print $1}')
    INDIRECTLY_LOST=$(grep "indirectly lost:" "$logfile" | grep -oE "[0-9,]+ bytes" | head -1 | tr -d ',' | awk '{print $1}')
    POSSIBLY_LOST=$(grep "possibly lost:" "$logfile" | grep -oE "[0-9,]+ bytes" | head -1 | tr -d ',' | awk '{print $1}')
    
    # Verificar file descriptors
    FD_LEAKS=$(grep "Open file descriptor" "$logfile" | wc -l)
    
    HAS_LEAKS=0
    
    if [ "$DEFINITELY_LOST" != "0" ] && [ -n "$DEFINITELY_LOST" ]; then
        HAS_LEAKS=1
    fi
    
    if [ "$INDIRECTLY_LOST" != "0" ] && [ -n "$INDIRECTLY_LOST" ]; then
        HAS_LEAKS=1
    fi
    
    if [ "$POSSIBLY_LOST" != "0" ] && [ -n "$POSSIBLY_LOST" ]; then
        HAS_LEAKS=1
    fi
    
    if [ $FD_LEAKS -gt 0 ]; then
        HAS_LEAKS=1
    fi
    
    if [ $HAS_LEAKS -eq 0 ]; then
        if grep -q "no leaks are possible" "$logfile"; then
            echo -e "${GREEN}✓ [${i}] ${TEST_NAME}: PERFECTO (no leaks)${NC}"
        else
            echo -e "${GREEN}✓ [${i}] ${TEST_NAME}: OK${NC}"
        fi
    else
        echo -e "${RED}✗ [${i}] ${TEST_NAME}: LEAKS DETECTADOS${NC}"
        LEAK_COUNT=$((LEAK_COUNT + 1))
        
        # Mostrar detalles
        if [ -n "$DEFINITELY_LOST" ] && [ "$DEFINITELY_LOST" != "0" ]; then
            echo -e "  ${RED}  → Definitely lost: ${DEFINITELY_LOST} bytes${NC}"
        fi
        if [ -n "$INDIRECTLY_LOST" ] && [ "$INDIRECTLY_LOST" != "0" ]; then
            echo -e "  ${RED}  → Indirectly lost: ${INDIRECTLY_LOST} bytes${NC}"
        fi
        if [ -n "$POSSIBLY_LOST" ] && [ "$POSSIBLY_LOST" != "0" ]; then
            echo -e "  ${YELLOW}  → Possibly lost: ${POSSIBLY_LOST} bytes${NC}"
        fi
        if [ $FD_LEAKS -gt 0 ]; then
            echo -e "  ${RED}  → File descriptors sin cerrar: ${FD_LEAKS}${NC}"
        fi
        
        echo -e "  ${MAGENTA}  Ver detalles en: $logfile${NC}"
    fi
done

# Resumen final
echo ""
echo -e "${CYAN}========================================${NC}"
if [ $LEAK_COUNT -eq 0 ]; then
    echo -e "${GREEN}✓✓✓ TODOS LOS TESTS PASARON SIN LEAKS ✓✓✓${NC}"
    echo -e "${GREEN}Tu minishell está libre de memory leaks!${NC}"
else
    echo -e "${RED}✗✗✗ $LEAK_COUNT DE $TOTAL_TESTS TESTS CON LEAKS ✗✗✗${NC}"
    echo -e "${YELLOW}Revisa los logs en: ${LOGDIR}/${NC}"
    echo -e "${YELLOW}Busca 'definitely lost' y 'indirectly lost' en los logs${NC}"
fi
echo -e "${CYAN}========================================${NC}"

# Mostrar comando para ver logs
echo ""
echo -e "${BLUE}Para ver un log específico:${NC}"
echo -e "  cat ${LOGDIR}/01_builtins_${TIMESTAMP}.log | less"
echo ""
echo -e "${BLUE}Para buscar todos los leaks:${NC}"
echo -e "  grep -n 'definitely lost' ${LOGDIR}/*_${TIMESTAMP}.log"
echo ""

# Exit code
if [ $LEAK_COUNT -eq 0 ]; then
    exit 0
else
    exit 1
fi
