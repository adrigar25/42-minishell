# 🗺️ MAPA DE FLUJOS DE EJECUCIÓN - MINISHELL MANDATORY

## 📊 RESUMEN EJECUTIVO

He analizado completamente tu minishell mandatory y he identificado **TODOS** los flujos de ejecución posibles. Este documento mapea cada caso y proporciona los comandos específicos para probarlos con valgrind.

---

## 🔍 FLUJOS PRINCIPALES IDENTIFICADOS

### 1. INICIALIZACIÓN Y SETUP
**Archivos involucrados:**
- `main.c` → `ft_minishell()`
- `ft_init_data()` → Crea estructura `t_data`
- `update_shlvl()` → Incrementa SHLVL

**Flujo:**
1. Inicializar datos (calloc de t_data)
2. Duplicar environment (ft_dupenv)
3. Actualizar SHLVL
4. Configurar señales (ft_init_signals)
5. Mostrar mensaje de bienvenida (si isatty)

**Posibles leaks:**
- Memoria de t_data
- Copia de envp
- Variables de entorno

**Test:**
```bash
valgrind ./minishell
exit
```

---

### 2. LECTURA DE INPUT
**Archivos involucrados:**
- `ft_read_input.c` → readline()
- `ft_generate_prompt()` → Genera prompt

**Flujo:**
1. Generar prompt dinámico con PWD
2. Leer línea con readline()
3. Agregar a history si no está vacío
4. Retornar input

**Posibles leaks:**
- String del prompt
- String de readline
- History de readline

**Test:**
```bash
valgrind ./minishell
echo "test"
exit
```

---

### 3. PROCESAMIENTO DE INPUT
**Archivos involucrados:**
- `ft_process_input.c`
- `ft_count_args.c`
- `ft_split_input.c`
- `ft_check_input_syntax.c`
- `ft_handle_env_expansion.c`
- `ft_parse_input.c`

**Flujo:**
1. Contar argumentos (ft_count_args)
2. Dividir input en tokens (ft_split_input)
3. Liberar input original
4. Verificar sintaxis (ft_check_input_syntax)
   - Detectar pipes al inicio/final
   - Detectar operadores consecutivos
   - Detectar redirecciones sin archivo
5. Expandir variables de entorno (ft_handle_env_expansion)
6. Liberar argv original si fue reemplazado
7. Parsear en lista de comandos (ft_parse_input)

**Posibles leaks:**
- Array de argv original
- Array de argv expandido
- Tokens individuales
- Lista de comandos (t_cmd)

**Test:**
```bash
valgrind ./minishell
echo $USER
exit
```

---

### 4. VERIFICACIÓN DE SINTAXIS
**Archivos involucrados:**
- `ft_check_input_syntax.c`

**Casos detectados:**
- Pipe al inicio: `| cat`
- Pipe/redir al final: `echo test |`
- Operadores dobles: `echo || cat`
- Operadores lógicos: `&&`, `||`, `&`
- Redirección sin archivo: `cat <`
- Redirecciones consecutivas: `< < file`

**Flujo:**
1. Verificar primer token no sea operador
2. Iterar tokens verificando pares consecutivos
3. Retornar error con código 2 si inválido

**Posibles leaks:**
- Mensajes de error

**Test:**
```bash
valgrind ./minishell
| cat
echo test |
cat <
exit
```

---

### 5. EXPANSIÓN DE VARIABLES
**Archivos involucrados:**
- `ft_handle_env_expansion.c`
- `ft_process_arg.c`
- `ft_expand_env_var.c`
- `ft_expand_exit_status.c`
- `ft_expansion_utils.c`

**Flujo:**
1. Iterar cada argumento
2. Para cada `$`:
   - Si `$?` → expandir exit status
   - Si `$VAR` → buscar en envp y expandir
   - Si `$NOEXISTE` → expandir a vacío
3. Copiar literales entre variables
4. Manejar comillas:
   - Dobles: expanden
   - Simples: no expanden

**Casos especiales:**
- `$VAR` en comillas dobles → expande
- `$VAR` en comillas simples → literal
- `$?` → último exit status
- `$$` → no soportado en mandatory
- Variables inexistentes → string vacío
- `$HOME`, `$USER`, `$PWD`, `$PATH` → del sistema

**Posibles leaks:**
- Strings expandidos intermedios
- Buffers de expansión

**Test:**
```bash
valgrind ./minishell
export VAR=test
echo $VAR
echo "$VAR"
echo '$VAR'
echo $?
echo $NOEXISTE
exit
```

---

### 6. PARSING EN COMANDOS
**Archivos involucrados:**
- `ft_parse_input.c`
- `parsing_utils/` (ft_create_cmd_node, ft_add_arg_to_cmd, ft_assign_fd, ft_redir, ft_process_pipe)

**Flujo:**
1. Crear primer nodo de comando
2. Iterar tokens:
   - Si `|` → crear nuevo nodo, enlazar
   - Si `<`, `>`, `>>`, `<<` → asignar fd
   - Otro → agregar como argumento
3. Para redirecciones:
   - `<` → ft_handle_infile()
   - `>` → ft_handle_outfile(0)
   - `>>` → ft_handle_outfile(1)
   - `<<` → ft_heredoc()

**Estructura t_cmd:**
```c
- argv: array de argumentos
- infd: file descriptor de entrada
- outfd: file descriptor de salida
- has_error: flag de error
- index: índice en pipeline
- data: puntero a t_data
- next: siguiente comando
```

**Posibles leaks:**
- Nodos t_cmd
- Arrays argv en cada nodo
- File descriptors no cerrados

**Test:**
```bash
valgrind ./minishell
echo test | cat
cat < file.txt
exit
```

---

### 7. EJECUCIÓN DE COMANDOS
**Archivos involucrados:**
- `ft_execute_cmds.c`
- `ft_exec_builtin.c`
- `ft_exec_bin.c`
- `ft_is_builtin.c`
- `ft_setup_child_io.c`
- `ft_finish_execution.c`

**Flujo principal:**
1. Alocar array de PIDs
2. Para cada comando:
   - **Caso especial:** Builtin sin pipe/redir → ejecutar directo
   - **Caso general:** Fork y ejecutar
3. En hijo (fork):
   - Configurar I/O (ft_setup_child_io)
   - Si builtin → ft_exec_builtin()
   - Si binario → ft_exec_bin()
   - Exit con status
4. En padre:
   - Guardar PID
   - Cerrar file descriptors
5. Esperar todos los hijos (ft_finish_execution)
6. Liberar array de PIDs

**Casos especiales:**
- **Builtin sin redirección ni pipe:** Se ejecuta directamente en el padre (no fork)
  - cd, export, unset, exit modifican el entorno del shell
- **Builtin con redirección o pipe:** Se ejecuta en hijo (fork)
- **Comando externo:** Siempre fork

**Posibles leaks:**
- Array de PIDs
- Procesos zombies
- File descriptors en padre
- File descriptors en hijo

**Test:**
```bash
valgrind ./minishell
echo test
cd /tmp
echo test | cat
/bin/ls
exit
```

---

### 8. BUILTINS

#### 8.1 ECHO
**Archivo:** `ft_echo.c`

**Flujo:**
1. Detectar flag `-n`
2. Iterar argumentos y imprimir
3. Imprimir newline si no hay `-n`

**Casos:**
- `echo hola`
- `echo -n hola`
- `echo -n -n -n hola`
- `echo "$VAR"`
- `echo '$VAR'`
- `echo "texto con 'comillas'"`

**Test:**
```bash
valgrind ./minishell
echo "test"
echo -n "sin newline"
exit
```

#### 8.2 CD
**Archivo:** `ft_cd.c`

**Flujo:**
1. Determinar directorio destino:
   - Sin args → HOME
   - `~` → HOME
   - `-` → OLDPWD
   - Otro → usar literal
2. Llamar `chdir()`
3. Si `-`, imprimir nuevo directorio
4. Actualizar PWD y OLDPWD

**Casos:**
- `cd` → HOME
- `cd ~` → HOME
- `cd -` → OLDPWD
- `cd /tmp` → ruta absoluta
- `cd ..` → ruta relativa
- `cd inexistente` → error

**Posibles leaks:**
- Strings de PWD/OLDPWD

**Test:**
```bash
valgrind ./minishell
cd /tmp
cd -
cd ~
exit
```

#### 8.3 PWD
**Archivo:** `ft_pwd.c`

**Flujo:**
1. Llamar `getcwd()`
2. Imprimir resultado

**Test:**
```bash
valgrind ./minishell
pwd
exit
```

#### 8.4 EXPORT
**Archivo:** `ft_export.c`

**Flujo:**
1. Si sin args → imprimir todas las variables
2. Para cada arg:
   - Validar identificador
   - Si contiene `=` → extraer nombre y valor
   - Llamar `ft_setenv()`

**Validación:**
- Debe empezar con letra o `_`
- Puede contener letras, números, `_`
- No puede contener `-`, espacios, empezar con número

**Casos:**
- `export` → mostrar todas
- `export VAR=valor`
- `export VAR="con espacios"`
- `export VAR` → exportar sin valor
- `export 123VAR=x` → error

**Posibles leaks:**
- Realloc de envp
- Strings de variables nuevas

**Test:**
```bash
valgrind ./minishell
export VAR=test
echo $VAR
export
exit
```

#### 8.5 UNSET
**Archivo:** `ft_unset.c`

**Flujo:**
1. Para cada arg:
   - Buscar variable en envp
   - Si existe, eliminar y reordenar array
   - Liberar string eliminado

**Test:**
```bash
valgrind ./minishell
export VAR=test
unset VAR
echo $VAR
exit
```

#### 8.6 ENV
**Archivo:** `ft_env.c`

**Flujo:**
1. Iterar envp
2. Imprimir cada variable

**Test:**
```bash
valgrind ./minishell
env
exit
```

#### 8.7 EXIT
**Archivo:** `ft_exit.c`

**Flujo:**
1. Si sin args → exit(0)
2. Si 1 arg:
   - Validar que sea numérico
   - Convertir a número (ft_atoll)
   - exit(número % 256)
3. Si >1 arg → error, exit(1)
4. Si no numérico → error, exit(2)

**Casos:**
- `exit` → 0
- `exit 42` → 42
- `exit 256` → 0 (256 % 256)
- `exit abc` → error, 2
- `exit 1 2` → error, 1

**Test:**
```bash
./minishell -c "exit 42"; echo $?
./minishell -c "exit 256"; echo $?
```

---

### 9. REDIRECCIONES

#### 9.1 INPUT (<)
**Archivo:** `ft_handle_infile.c`

**Flujo:**
1. Abrir archivo con `open()` en modo lectura
2. Si error → mensaje de error, retornar -1
3. Guardar fd en `cmd->infd`
4. En ejecución, `dup2(infd, STDIN_FILENO)`

**Casos:**
- `cat < archivo.txt` → leer de archivo
- `cat < noexiste` → error
- `cat < /tmp/noperm` → error permisos

**Posibles leaks:**
- File descriptor no cerrado

**Test:**
```bash
echo "test" > /tmp/test.txt
valgrind ./minishell
cat < /tmp/test.txt
exit
rm /tmp/test.txt
```

#### 9.2 OUTPUT (>)
**Archivo:** `ft_handle_outfile.c`

**Flujo:**
1. Abrir archivo con `open()` en modo `O_TRUNC`
2. Permisos 0644
3. Si error → mensaje, retornar -1
4. Guardar fd en `cmd->outfd`
5. En ejecución, `dup2(outfd, STDOUT_FILENO)`

**Casos:**
- `echo test > out.txt` → crear/truncar
- `echo test > /noperm/file` → error

**Posibles leaks:**
- File descriptor no cerrado

**Test:**
```bash
valgrind ./minishell
echo "test" > /tmp/out.txt
cat /tmp/out.txt
exit
rm /tmp/out.txt
```

#### 9.3 APPEND (>>)
**Archivo:** `ft_handle_outfile.c` con flag

**Flujo:**
1. Abrir con `O_APPEND`
2. Resto igual que OUTPUT

**Test:**
```bash
valgrind ./minishell
echo "línea1" >> /tmp/append.txt
echo "línea2" >> /tmp/append.txt
cat /tmp/append.txt
exit
rm /tmp/append.txt
```

#### 9.4 MÚLTIPLES REDIRECCIONES
**Flujo:**
1. Procesar de izquierda a derecha
2. Última redirección gana
3. Cerrar fds anteriores

**Ejemplo:**
```bash
cat < file1.txt < file2.txt  # Lee de file2.txt
echo test > out1.txt > out2.txt  # Escribe en out2.txt
```

**Test:**
```bash
valgrind ./minishell
echo "1" > /tmp/f1.txt
echo "2" > /tmp/f2.txt
cat < /tmp/f1.txt < /tmp/f2.txt
exit
rm /tmp/f1.txt /tmp/f2.txt
```

---

### 10. HEREDOCS (<<)
**Archivo:** `ft_heredoc.c`

**Flujo:**
1. Crear pipe()
2. Loop leyendo líneas con readline (get_next_line)
3. Para cada línea:
   - Si es delimiter → terminar
   - Expandir variables (ft_process_arg)
   - Escribir a pipe
4. Cerrar write end
5. Retornar read end como infd

**Expansión en heredoc:**
- Variables: `$VAR` → expandido
- Exit status: `$?` → expandido
- Si delimiter entre comillas: NO expandir

**Posibles leaks:**
- Strings de readline
- Strings expandidos
- File descriptors del pipe

**Test:**
```bash
valgrind ./minishell
cat << EOF
línea 1
línea 2
EOF
export VAR=test
cat << EOF
Variable: $VAR
EOF
exit
```

---

### 11. PIPES
**Archivos involucrados:**
- `ft_execute_cmds.c`
- `ft_setup_child_io.c`

**Flujo:**
1. Para pipeline `cmd1 | cmd2 | cmd3`:
2. Cada comando ya tiene infd/outfd configurado en parsing
3. En `ft_setup_child_io()`:
   - Si no es primer comando → `dup2(infd, STDIN)`
   - Si no es último comando → `dup2(outfd, STDOUT)`
4. Cerrar todos los fds excepto stdin/stdout/stderr
5. Ejecutar comando

**Configuración de pipes en parsing:**
- Al encontrar `|`:
  - Crear pipe()
  - cmd_actual->outfd = pipe[1]
  - cmd_siguiente->infd = pipe[0]

**Posibles leaks:**
- File descriptors de pipes no cerrados
- Procesos no esperados (waitpid)

**Test:**
```bash
valgrind ./minishell
echo "test" | cat
echo "hola mundo" | grep mundo
echo "multi" | cat | cat | cat
exit
```

---

### 12. SEÑALES
**Archivos involucrados:**
- `ft_init_signals.c`
- `sigint_handler()` (signal handler)

**Configuración:**
- `SIGINT` (Ctrl-C) → sigint_handler
- `SIGQUIT` (Ctrl-\) → SIG_IGN
- `SIGPIPE` → SIG_IGN

**Comportamiento:**
- **En prompt:** Ctrl-C → nueva línea, limpiar readline
- **Durante comando:** Ctrl-C → matar comando hijo
- **Ctrl-\:** Ignorado
- **Ctrl-D:** EOF, salir del shell

**Posibles leaks:**
- Memoria de readline no liberada al interrumpir
- Procesos hijos no terminados

**Test:**
```bash
valgrind ./minishell
# Presionar Ctrl-C varias veces
sleep 5
# Presionar Ctrl-C durante sleep
exit
```

---

### 13. LIMPIEZA (CLEANUP)
**Archivo:** `ft_cleanup.c`

**Flujo:**
1. Liberar envp (ft_free_matrix)
2. Liberar data
3. clear_history() (readline)
4. rl_clear_history() (readline)

**Llamado:**
- Al salir normalmente (exit)
- Al recibir EOF (Ctrl-D)
- Después de ejecutar todos los comandos del loop

**Posibles leaks:**
- Envp no liberado
- Data no liberado
- History de readline

**Test:**
```bash
valgrind ./minishell
echo "test"
exit
# O presionar Ctrl-D
```

---

### 14. MANEJO DE ERRORES
**Archivo:** `ft_handle_error.c`

**Códigos de error identificados:**
```
4:  Error al abrir archivo (infile)
5:  Syntax error: newline inesperado
6:  Syntax error: token inesperado
7:  Demasiados argumentos
8:  HOME no definido
11: Error de cd con detalles
13: Export: identificador inválido
14: Exit: argumento no numérico
```

**Flujo:**
1. Imprimir mensaje de error a stderr
2. Retornar código de error
3. Actualizar last_exit_status

**Test:**
```bash
valgrind ./minishell
cat < noexiste
cd /directorio/inexistente
export 123VAR=error
exit abc
```

---

## 🎯 CASOS EDGE CRÍTICOS PARA LEAKS

### 1. Readline interrumpido
```bash
valgrind ./minishell
# Escribir algo y presionar Ctrl-C antes de Enter
exit
```

### 2. Heredoc interrumpido
```bash
valgrind ./minishell
cat << EOF
línea 1
# Presionar Ctrl-C
exit
```

### 3. Múltiples comandos con error
```bash
valgrind ./minishell
comandoinexistente
comandoinexistente
comandoinexistente
exit
```

### 4. Pipes con errores
```bash
valgrind ./minishell
comandoinexistente | cat
echo test | comandoinexistente
comandoinexistente | comandoinexistente
exit
```

### 5. Redirecciones con errores
```bash
valgrind ./minishell
cat < noexiste
cat < noexiste < noexiste2
echo test > /noperm/file
exit
```

### 6. Variables circulares/complejas
```bash
valgrind ./minishell
export A=B
export B=C
export C=A
echo $A $B $C
unset A B C
exit
```

### 7. Expansión en heredoc
```bash
valgrind ./minishell
export VAR=test
cat << EOF
$VAR
$?
$NOEXISTE
EOF
unset VAR
exit
```

### 8. Salida con Ctrl-D
```bash
valgrind ./minishell
echo "test"
# Presionar Ctrl-D (debe salir limpiamente)
```

---

## 📋 CHECKLIST DE LEAKS POR MÓDULO

### ✓ Inicialización
- [ ] t_data alocada y liberada
- [ ] envp duplicado y liberado
- [ ] Variables de SHLVL liberadas

### ✓ Input
- [ ] Prompt generado y liberado
- [ ] String de readline liberado
- [ ] History limpiado

### ✓ Procesamiento
- [ ] argv original liberado
- [ ] argv expandido liberado
- [ ] Tokens intermedios liberados

### ✓ Expansión
- [ ] Buffers de expansión liberados
- [ ] Strings expandidos liberados
- [ ] Variables temporales liberadas

### ✓ Parsing
- [ ] Lista de comandos liberada
- [ ] Arrays argv de cada comando liberados
- [ ] Nodos t_cmd liberados

### ✓ Ejecución
- [ ] Array de PIDs liberado
- [ ] File descriptors cerrados (infd, outfd)
- [ ] Procesos hijos esperados (waitpid)

### ✓ Redirecciones
- [ ] File descriptors de archivos cerrados
- [ ] Nombres de archivos liberados

### ✓ Heredocs
- [ ] Pipe fds cerrados
- [ ] Líneas leídas liberadas
- [ ] Strings expandidos liberados

### ✓ Pipes
- [ ] Pipe fds cerrados en padre
- [ ] Pipe fds cerrados en hijo

### ✓ Builtins
- [ ] Strings temporales de cd liberados
- [ ] envp realocado correctamente en export
- [ ] Variables eliminadas liberadas en unset

### ✓ Señales
- [ ] Memoria de readline limpiada en SIGINT
- [ ] Procesos hijos matados correctamente

### ✓ Cleanup
- [ ] envp completo liberado
- [ ] data liberado
- [ ] History de readline limpiado

---

## 🚀 COMANDO PARA EJECUTAR TODOS LOS TESTS

```bash
# Opción 1: Script completo (10 tests)
./run_leak_tests.sh

# Opción 2: Test rápido
./quick_leak_test.sh

# Opción 3: Manual con valgrind completo
valgrind --leak-check=full \
         --show-leak-kinds=all \
         --track-origins=yes \
         --track-fds=yes \
         --verbose \
         --log-file=full_leak_test.log \
         ./minishell < COMPREHENSIVE_TESTS.md

# Ver resumen de leaks
grep -E "(definitely|indirectly|possibly) lost:" full_leak_test.log
```

---

## 📊 ESTADÍSTICAS DEL ANÁLISIS

**Archivos analizados:** ~60 archivos
**Funciones identificadas:** ~100+
**Flujos de ejecución:** 14 principales
**Casos de prueba:** 200+ comandos
**Builtins:** 7 (echo, cd, pwd, export, unset, env, exit)
**Tipos de redirección:** 4 (<, >, >>, <<)
**Señales manejadas:** 3 (SIGINT, SIGQUIT, SIGPIPE)

---

## ⚠️ ÁREAS CRÍTICAS PARA LEAKS

**Prioridad ALTA:**
1. **ft_handle_env_expansion** - Expansión de variables
2. **ft_parse_input** - Creación de lista de comandos
3. **ft_execute_cmds** - File descriptors de pipes
4. **ft_heredoc** - Pipe y strings leídos
5. **ft_cleanup** - Liberación final

**Prioridad MEDIA:**
6. **ft_setenv/ft_dupenv** - Manejo de envp
7. **ft_process_arg** - Buffers de expansión
8. **ft_split_input** - Tokenización
9. **ft_cd** - Strings de PWD/OLDPWD

**Prioridad BAJA:**
10. **Builtins simples** - echo, pwd, env

---

## 🎓 CONCLUSIONES

Tu minishell tiene los siguientes flujos principales:
1. **Inicialización** → Setup de datos y señales
2. **Loop principal** → Leer, procesar, ejecutar
3. **Procesamiento** → Sintaxis, expansión, parsing
4. **Ejecución** → Fork, exec, wait
5. **Cleanup** → Liberación de memoria

**Puntos críticos para memory leaks:**
- Expansión de variables (múltiples mallocs)
- Parsing de comandos (listas enlazadas)
- File descriptors de pipes y redirecciones
- Array de envp (reallocs en export)
- History de readline

**Usa los scripts proporcionados para probar TODO.**

¡Buena suerte! 🍀
