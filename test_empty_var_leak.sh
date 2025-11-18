#!/bin/bash

# Test específico para el leak de variables inexistentes

echo "=========================================="
echo "TEST: Leak de variables inexistentes"
echo "=========================================="
echo ""

cd /Users/agarcia/Desktop/minishell

echo "Probando: \$VARIABLE_INEXISTENTE"
echo ""

./minishell << 'EOF'
$ads
$NOEXISTE
$VAR_QUE_NO_EXISTE
echo "test completado"
exit
EOF

echo ""
echo "=========================================="
echo "Test completado - Verificar con valgrind"
echo "=========================================="
