#!/bin/bash

# =========================================================================
#                         TEST: SIGNALS
# =========================================================================

echo "🧪 TESTING: Signals (Ctrl+C, Ctrl+\\)"
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

# Verificar que minishell existe
if [ ! -f "../minishell" ]; then
    echo -e "${RED}❌ Error: ../minishell no encontrado. Ejecuta 'make' primero.${NC}"
    exit 1
fi

echo -e "${GREEN}✅ Minishell encontrado${NC}\n"

show_test "Comportamiento básico de signals"
echo -e "${YELLOW}Nota:${NC} Los tests de signals son interactivos."
echo -e "${YELLOW}Para testear signals manualmente:${NC}"
echo
echo -e "${BLUE}1. Ctrl+C (SIGINT):${NC}"
echo "   - Ejecuta: ../minishell"
echo "   - Presiona Ctrl+C"
echo "   - Debería mostrar nueva línea de prompt"
echo "   - No debería salir del shell"
echo
echo -e "${BLUE}2. Ctrl+\\ (SIGQUIT):${NC}"
echo "   - En minishell, presiona Ctrl+\\"
echo "   - No debería hacer nada (ignorado)"
echo
echo -e "${BLUE}3. Signal durante comando:${NC}"
echo "   - Ejecuta: sleep 5"
echo "   - Presiona Ctrl+C"
echo "   - Debería interrumpir sleep y volver al prompt"

# Test automático básico
show_test "Test automático: exit status después de signal"
echo -e "${YELLOW}Simulando comportamiento de signal...${NC}"

echo -e "\n${BLUE}🐚 BASH (signal simulation):${NC}"
bash_signal=$(bash -c "sleep 1 & pid=\$!; sleep 0.5; kill -SIGINT \$pid 2>/dev/null; wait \$pid 2>/dev/null; echo \"Exit status: \$?\"" 2>/dev/null)
echo "$bash_signal"

echo -e "\n${BLUE}🔧 MINISHELL:${NC}"
echo "Para test manual, ejecuta: echo 'sleep 2' | timeout 1 ../minishell"

echo -e "\n${GREEN}✓ Test de información completado${NC}"
echo "========================================="

show_test "Verificación de signal handlers"
echo -e "${YELLOW}Verificando que minishell tiene manejadores de señales...${NC}"

# Test simple para verificar que el programa no se cuelga
echo -e "\n${BLUE}Test básico de responsividad:${NC}"
timeout 3 bash -c "echo 'echo Signal test; exit' | ../minishell" >/dev/null 2>&1
if [ $? -eq 0 ]; then
    echo -e "${GREEN}✅ Minishell responde correctamente${NC}"
else
    echo -e "${RED}❌ Minishell puede tener problemas de responsividad${NC}"
fi

echo -e "\n${GREEN}✓ Test completado${NC}"
echo "========================================="

show_test "Instrucciones para tests manuales completos"
echo -e "${YELLOW}Para hacer tests completos de signals:${NC}"
echo
echo -e "${BLUE}Test 1:${NC} ../minishell"
echo "        Presiona Ctrl+C varias veces"
echo "        Verifica que aparece nueva línea de prompt"
echo
echo -e "${BLUE}Test 2:${NC} En minishell, ejecuta: sleep 10"
echo "        Presiona Ctrl+C durante el sleep"
echo "        Verifica que se interrumpe y vuelve al prompt"
echo
echo -e "${BLUE}Test 3:${NC} En minishell, presiona Ctrl+\\"
echo "        Verifica que no pasa nada (ignorado)"
echo
echo -e "${BLUE}Test 4:${NC} En minishell, ejecuta: cat"
echo "        Presiona Ctrl+C"
echo "        Verifica que se interrumpe cat"

echo -e "\n${GREEN}🎉 INFORMACIÓN DE TESTS DE SIGNALS COMPLETADA${NC}"
echo -e "${YELLOW}💡 Ejecuta los tests manuales para verificación completa${NC}"
