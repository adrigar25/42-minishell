#!/bin/bash
# test_heredoc_ctrl_c.sh

echo "========================================="
echo "   TEST: HEREDOC CON Ctrl+C"
echo "========================================="
echo ""

echo "Test 1: Heredoc normal (sin Ctrl+C)"
echo "Comando: cat << EOF"
echo ""
echo -e "cat << EOF\nlinea1\nlinea2\nEOF\nexit" | ./minishell

echo ""
echo "========================================="
echo ""

echo "Test 2: Probar manualmente con Ctrl+C"
echo "Ejecuta: ./minishell"
echo "Luego escribe: cat << EOF"
echo "Presiona Ctrl+C para interrumpir"
echo "El heredoc debe cancelarse inmediatamente y volver al prompt"
echo ""
echo "Para probar, ejecuta: ./minishell"
