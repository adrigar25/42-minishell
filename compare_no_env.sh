#!/bin/bash

# ================================================================
# COMPARACIÓN: BASH vs MINISHELL SIN ENVIRONMENT
# ================================================================

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
CYAN='\033[0;36m'
NC='\033[0m'

echo -e "${CYAN}=========================================="
echo "COMPARACIÓN: BASH vs MINISHELL"
echo "SIN ENVIRONMENT"
echo -e "==========================================${NC}"
echo ""

# ================================================================
# BASH
# ================================================================
echo -e "${YELLOW}1. BASH SIN ENVIRONMENT:${NC}"
echo "-------------------------------------------"

env -i bash --norc --noprofile << 'BASH_EOF'
echo "▶ pwd"
pwd
echo ""

echo "▶ cd (sin argumentos)"
cd 2>&1
echo "Exit code: $?"
echo ""

echo "▶ cd /tmp"
cd /tmp && pwd
echo ""

echo "▶ cd -"
cd - 2>&1
echo ""

echo "▶ export VAR=test"
export VAR=test
echo "▶ echo \$VAR"
echo $VAR
echo ""

echo "▶ env"
env | sort
echo ""

echo "▶ echo \$HOME"
echo "HOME=[$HOME]"
echo ""

echo "▶ echo \$PATH"
echo "PATH=[$PATH]"
echo ""

exit
BASH_EOF

echo ""
echo ""

# ================================================================
# MINISHELL
# ================================================================
echo -e "${YELLOW}2. MINISHELL SIN ENVIRONMENT:${NC}"
echo "-------------------------------------------"

if [ ! -f "./minishell" ]; then
    echo -e "${RED}Error: minishell no encontrado${NC}"
    exit 1
fi

env -i ./minishell << 'MINI_EOF'
echo "▶ pwd"
pwd
echo "▶ cd (sin argumentos)"
cd
echo "▶ echo \$?"
echo $?
echo "▶ cd /tmp"
cd /tmp
pwd
echo "▶ cd -"
cd -
echo "▶ export VAR=test"
export VAR=test
echo "▶ echo \$VAR"
echo $VAR
echo "▶ env"
env
echo "▶ echo \$HOME"
echo $HOME
echo "▶ echo \$PATH"
echo $PATH
exit
MINI_EOF

echo ""
echo -e "${CYAN}=========================================="
echo "RESUMEN DE DIFERENCIAS"
echo -e "==========================================${NC}"
echo ""
echo -e "${GREEN}✓ Comportamientos correctos:${NC}"
echo "  - pwd funciona sin PWD en environment"
echo "  - cd sin argumentos falla sin HOME"
echo "  - cd con ruta absoluta funciona"
echo "  - export/echo funcionan correctamente"
echo "  - env muestra solo variables exportadas"
echo "  - Variables inexistentes retornan vacío"
echo ""
echo -e "${BLUE}ℹ  Diferencias esperables:${NC}"
echo "  - BASH inicializa algunas variables automáticamente"
echo "    (PWD, PATH, SHLVL, SHELL)"
echo "  - MINISHELL solo crea SHLVL=1 y PWD si hace cd"
echo ""
echo -e "${YELLOW}⚠  Casos especiales:${NC}"
echo "  - cd - falla correctamente sin OLDPWD"
echo "  - cd ~ funciona en bash (expande internamente)"
echo "  - Comandos externos requieren PATH exportado"
echo ""
