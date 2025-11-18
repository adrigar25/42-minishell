#!/bin/bash

# ================================================================
# RESUMEN RÁPIDO - COMANDOS PARA PROBAR TODOS LOS CASOS
# ================================================================

cat << 'EOF'

╔══════════════════════════════════════════════════════════════╗
║     MINISHELL - GUÍA RÁPIDA DE TESTS DE MEMORY LEAKS        ║
╚══════════════════════════════════════════════════════════════╝

📚 DOCUMENTACIÓN GENERADA:
  1. COMPREHENSIVE_TESTS.md    - Todos los comandos de prueba
  2. EXECUTION_FLOWS.md        - Mapa completo de flujos
  3. run_leak_tests.sh         - Script automático (10 tests)
  4. quick_leak_test.sh        - Test rápido para desarrollo

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

🚀 INICIO RÁPIDO:

1. Test automático completo (recomendado):
   $ ./run_leak_tests.sh

2. Test rápido durante desarrollo:
   $ ./quick_leak_test.sh

3. Test manual interactivo:
   $ valgrind --leak-check=full ./minishell

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

📋 CASOS CRÍTICOS A PROBAR:

┌─ BUILTINS ─────────────────────────────────────────────────┐
│ echo "test"                                                 │
│ echo -n "sin newline"                                       │
│ cd /tmp && cd - && cd ~                                     │
│ export VAR=valor && echo $VAR && unset VAR                  │
│ env | head                                                  │
│ exit 42                                                     │
└─────────────────────────────────────────────────────────────┘

┌─ REDIRECCIONES ────────────────────────────────────────────┐
│ echo "test" > /tmp/out.txt                                  │
│ cat < /tmp/out.txt                                          │
│ echo "append" >> /tmp/out.txt                               │
│ cat < /tmp/in.txt > /tmp/out.txt                            │
└─────────────────────────────────────────────────────────────┘

┌─ PIPES ────────────────────────────────────────────────────┐
│ echo "test" | cat                                           │
│ echo "hola mundo" | grep mundo                              │
│ ls -la | grep minishell | wc -l                             │
│ echo "multi" | cat | cat | cat                              │
└─────────────────────────────────────────────────────────────┘

┌─ HEREDOCS ─────────────────────────────────────────────────┐
│ cat << EOF                                                  │
│ línea 1                                                     │
│ línea 2                                                     │
│ EOF                                                         │
│                                                             │
│ export VAR=test                                             │
│ cat << EOF                                                  │
│ Variable: $VAR                                              │
│ EOF                                                         │
└─────────────────────────────────────────────────────────────┘

┌─ EXPANSIÓN ────────────────────────────────────────────────┐
│ export VAR=valor                                            │
│ echo $VAR                                                   │
│ echo "$VAR"                                                 │
│ echo '$VAR'                                                 │
│ echo $?                                                     │
│ echo $HOME $USER $PWD                                       │
└─────────────────────────────────────────────────────────────┘

┌─ ERRORES DE SINTAXIS ──────────────────────────────────────┐
│ | cat                    # Pipe al inicio                   │
│ echo test |            # Pipe al final                      │
│ cat <                  # Redirección sin archivo            │
│ echo || cat            # Operador doble                     │
└─────────────────────────────────────────────────────────────┘

┌─ SEÑALES ──────────────────────────────────────────────────┐
│ # Presionar Ctrl-C en prompt                                │
│ sleep 5                # Luego Ctrl-C                       │
│ cat << EOF             # Luego Ctrl-C en heredoc            │
│ # Presionar Ctrl-D para salir                               │
└─────────────────────────────────────────────────────────────┘

┌─ CASOS EDGE ───────────────────────────────────────────────┐
│ echo                   # Comando vacío                      │
│ export VAR= && echo $VAR  # Variable vacía                  │
│ echo    múltiples    espacios                               │
│ comandoinexistente     # Comando no encontrado              │
└─────────────────────────────────────────────────────────────┘

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

🔍 COMANDOS DE VALGRIND ÚTILES:

┌─ Básico ───────────────────────────────────────────────────┐
│ valgrind --leak-check=full ./minishell                      │
└─────────────────────────────────────────────────────────────┘

┌─ Completo ─────────────────────────────────────────────────┐
│ valgrind --leak-check=full \                                │
│          --show-leak-kinds=all \                            │
│          --track-origins=yes \                              │
│          --track-fds=yes \                                  │
│          ./minishell                                        │
└─────────────────────────────────────────────────────────────┘

┌─ Con log ──────────────────────────────────────────────────┐
│ valgrind --leak-check=full \                                │
│          --log-file=valgrind.log \                          │
│          ./minishell                                        │
└─────────────────────────────────────────────────────────────┘

┌─ Verificar file descriptors ───────────────────────────────┐
│ valgrind --track-fds=yes ./minishell                        │
└─────────────────────────────────────────────────────────────┘

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

📊 ANALIZAR RESULTADOS:

┌─ Buscar leaks en logs ─────────────────────────────────────┐
│ grep "definitely lost" valgrind.log                         │
│ grep "indirectly lost" valgrind.log                         │
│ grep "possibly lost" valgrind.log                           │
└─────────────────────────────────────────────────────────────┘

┌─ Ver resumen ──────────────────────────────────────────────┐
│ grep "LEAK SUMMARY" valgrind.log -A 5                       │
└─────────────────────────────────────────────────────────────┘

┌─ File descriptors abiertos ────────────────────────────────┐
│ grep "Open file descriptor" valgrind.log                    │
└─────────────────────────────────────────────────────────────┘

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

✅ CHECKLIST ANTES DE ENTREGAR:

 [ ] Test automático sin leaks: ./run_leak_tests.sh
 [ ] Todos los builtins funcionan correctamente
 [ ] Redirecciones (<, >, >>, <<) sin leaks
 [ ] Pipes simples y múltiples sin leaks
 [ ] Expansión de variables correcta
 [ ] Señales (Ctrl-C, Ctrl-D) manejadas
 [ ] Errores de sintaxis detectados
 [ ] File descriptors cerrados correctamente
 [ ] Exit codes correctos
 [ ] Norminette OK

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

🎯 TEST COMBINADO EXHAUSTIVO (copia y pega):

valgrind --leak-check=full --show-leak-kinds=all ./minishell << 'TESTEOF'
echo "=== INICIO TEST EXHAUSTIVO ==="
pwd
cd /tmp
cd -
export TEST=valor
echo $TEST
echo "$TEST"
echo '$TEST'
unset TEST
echo "redirect" > /tmp/test.txt
cat < /tmp/test.txt
echo "append" >> /tmp/test.txt
cat /tmp/test.txt
echo "pipe" | cat
echo "grep test" | grep test
cat << EOF
heredoc test
EOF
ls > /dev/null
echo $?
comandoinexistente 2>/dev/null
echo $?
rm /tmp/test.txt
echo "=== FIN TEST ==="
exit
TESTEOF

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

📖 INTERPRETACIÓN DE RESULTADOS:

✓ "no leaks are possible"
  → PERFECTO: Sin memory leaks

✓ "definitely lost: 0 bytes" + "indirectly lost: 0 bytes"
  → OK: Solo leaks "reachable" (normales en programas con readline)

✗ "definitely lost: X bytes"
  → PROBLEMA: Memory leak directo - DEBE CORREGIRSE

✗ "indirectly lost: X bytes"
  → PROBLEMA: Memory leak indirecto - DEBE CORREGIRSE

⚠ "still reachable: X bytes"
  → NORMAL: Memoria aún accesible al salir (OK para readline/history)

⚠ "Open file descriptor: X"
  → PROBLEMA: File descriptor no cerrado - DEBE CORREGIRSE

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

🐛 DEBUGGING TIPS:

1. Si hay leaks en expansión:
   → Revisa ft_process_arg() y ft_expand_env_var()

2. Si hay leaks en parsing:
   → Revisa ft_parse_input() y liberación de t_cmd

3. Si hay FDs abiertos:
   → Busca open() sin close()
   → Verifica pipes cerrados en padre e hijo

4. Si hay leaks en export:
   → Revisa ft_setenv() y realloc de envp

5. Si hay leaks al interrumpir (Ctrl-C):
   → Revisa signal handler y cleanup de readline

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

📁 ARCHIVOS CRÍTICOS PARA LEAKS:

  Prioridad ALTA:
    • ft_handle_env_expansion.c
    • ft_parse_input.c
    • ft_execute_cmds.c
    • ft_heredoc.c
    • ft_cleanup.c

  Prioridad MEDIA:
    • ft_setenv.c / ft_dupenv.c
    • ft_process_arg.c
    • ft_split_input.c
    • ft_cd.c

  Verificar FDs:
    • ft_handle_infile.c
    • ft_handle_outfile.c
    • ft_heredoc.c
    • ft_setup_child_io.c

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

🎓 EJEMPLO DE SESIÓN COMPLETA:

$ make re
$ ./run_leak_tests.sh
[Test 1/10] Builtins básicos...
[Test 2/10] Echo con diferentes opciones...
...
✓ [01] 01_builtins: PERFECTO (no leaks)
✓ [02] 02_echo_variants: OK
...
✓✓✓ TODOS LOS TESTS PASARON SIN LEAKS ✓✓✓

$ ./minishell
minishell$ echo "Funcionando sin leaks!"
Funcionando sin leaks!
minishell$ exit

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

¡Buena suerte con tu minishell! 🚀

Para más detalles, consulta:
  • COMPREHENSIVE_TESTS.md  → Todos los comandos de prueba
  • EXECUTION_FLOWS.md      → Explicación de flujos internos

EOF
