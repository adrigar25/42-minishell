#!/bin/bash

# Script para probar bash sin environment

echo "=========================================="
echo "PROBANDO BASH SIN ENVIRONMENT"
echo "=========================================="
echo ""

echo "1. Ejecutando bash sin environment..."
env -i bash --norc --noprofile << 'EOF'
echo "--- Bash sin environment ---"
echo ""

echo "Test 1: Variables de entorno"
echo "HOME = [$HOME]"
echo "USER = [$USER]"
echo "PWD = [$PWD]"
echo "PATH = [$PATH]"
echo "SHELL = [$SHELL]"
echo ""

echo "Test 2: pwd"
pwd
echo ""

echo "Test 3: cd (sin argumentos, debería fallar sin HOME)"
cd
echo "Exit code: $?"
echo ""

echo "Test 4: cd con ruta absoluta"
cd /tmp
echo "Exit code: $?"
pwd
echo ""

echo "Test 5: cd con ~"
cd ~
echo "Exit code: $?"
echo ""

echo "Test 6: cd -"
cd -
echo "Exit code: $?"
echo ""

echo "Test 7: export y echo"
export TEST=valor
echo "TEST = [$TEST]"
echo ""

echo "Test 8: env"
env
echo ""

echo "Test 9: echo básico"
echo "Hola mundo"
echo ""

echo "Test 10: exit status"
ls /tmp > /dev/null 2>&1
echo "Exit status después de ls: $?"
comandoinexistente 2>/dev/null
echo "Exit status después de comando inexistente: $?"
echo ""

echo "Test 11: Comandos externos (con PATH vacío)"
ls 2>&1 | head -1
echo ""

echo "Test 12: export PATH y probar comando"
export PATH=/usr/bin:/bin
ls /tmp | head -3
echo ""

exit 0
EOF

echo ""
echo "=========================================="
echo "FIN DE TESTS DE BASH"
echo "=========================================="
