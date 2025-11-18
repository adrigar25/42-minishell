#!/bin/bash

# Script para probar minishell sin environment

echo "=========================================="
echo "PROBANDO MINISHELL SIN ENVIRONMENT"
echo "=========================================="
echo ""

if [ ! -f "./minishell" ]; then
    echo "Error: minishell no encontrado. Compilando..."
    make > /dev/null 2>&1
    if [ $? -ne 0 ]; then
        echo "Error al compilar"
        exit 1
    fi
fi

echo "Ejecutando minishell sin environment..."
echo ""

env -i ./minishell << 'EOF'
echo "--- Minishell sin environment ---"
pwd
export TEST=valor
echo $TEST
cd /tmp
pwd
cd -
pwd
env
exit
EOF

EXIT_CODE=$?

echo ""
echo "=========================================="
echo "Exit code de minishell: $EXIT_CODE"
echo "=========================================="
