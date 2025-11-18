# 🧪 TESTS EXHAUSTIVOS PARA MINISHELL - DETECCIÓN DE LEAKS

## 📋 ÍNDICE DE FLUJOS Y CASOS DE PRUEBA

### 1. BUILTINS
### 2. REDIRECCIONES
### 3. PIPES
### 4. HEREDOCS
### 5. EXPANSIÓN DE VARIABLES
### 6. SINTAXIS Y ERRORES
### 7. SEÑALES
### 8. CASOS EDGE
### 9. MEMORIA Y LEAKS

---

## 🔧 1. BUILTINS

### 1.1 ECHO
```bash
# Test básico
echo "hola mundo"

# Flag -n
echo -n "sin newline"
echo -n -n -n "múltiples -n"

# Expansión de variables
echo $USER
echo "$USER"
echo '$USER'
echo $HOME
echo $PATH
echo $NONEXISTENT

# Exit status
echo $?

# Comillas mixtas
echo "hola 'mundo'"
echo 'hola "mundo"'
echo "hola $USER mundo"
echo 'hola $USER mundo'

# Strings vacíos
echo ""
echo ''
echo

# Múltiples argumentos
echo uno dos tres
echo "uno" "dos" "tres"

# Espacios
echo "   espacios   "
echo    múltiples    espacios

# Caracteres especiales
echo "hola\nmundo"
echo "tab\there"
```

**Comando para probar echo con valgrind:**
```bash
valgrind --leak-check=full --show-leak-kinds=all ./minishell
# Dentro de minishell, ejecutar todos los comandos echo arriba
exit
```

---

### 1.2 CD

```bash
# Test básico
pwd
cd /tmp
pwd
cd -
pwd

# Sin argumentos (ir a HOME)
cd
pwd

# Con ~
cd ~
pwd
cd ~/Desktop
pwd

# Rutas relativas
cd ..
pwd
cd ./test
pwd

# Rutas que no existen
cd /directorio/inexistente

# CD con demasiados argumentos
cd /tmp /usr

# Variables de entorno
cd $HOME
cd $PWD

# Casos edge
cd ""
cd ''
cd .
cd ..
cd ../..

# OLDPWD
cd /tmp
cd /usr
cd -
cd -

# Permisos denegados
mkdir -p /tmp/test_noperm
chmod 000 /tmp/test_noperm
cd /tmp/test_noperm
chmod 755 /tmp/test_noperm
rm -rf /tmp/test_noperm
```

**Comando para probar cd con valgrind:**
```bash
valgrind --leak-check=full --show-leak-kinds=all ./minishell
pwd
cd /tmp
cd -
cd ~
cd $HOME
exit
```

---

### 1.3 PWD

```bash
# Test básico
pwd

# Después de cd
cd /tmp
pwd
cd ~
pwd

# En subdirectorios
cd /usr/local/bin
pwd

# Después de cd -
cd /tmp
cd /usr
cd -
pwd
```

**Comando para probar pwd:**
```bash
valgrind --leak-check=full ./minishell
pwd
cd /tmp
pwd
exit
```

---

### 1.4 EXPORT

```bash
# Sin argumentos (mostrar todas las variables)
export

# Exportar nueva variable
export VAR1=valor1
echo $VAR1

# Exportar con comillas
export VAR2="valor con espacios"
echo $VAR2

# Exportar sin valor
export VAR3
echo $VAR3

# Múltiples exports
export A=1 B=2 C=3
echo $A $B $C

# Sobreescribir variable existente
export PATH=/nuevo/path
echo $PATH

# Variables con números
export VAR123=test
echo $VAR123

# Identificadores inválidos
export 123VAR=error
export VAR-NAME=error
export VAR NAME=error
export =valor

# Export vacío
export VAR=""
echo $VAR

# Variables especiales
export USER=nuevousuario
echo $USER
```

**Comando para probar export:**
```bash
valgrind --leak-check=full ./minishell
export
export TEST=valor
export TEST2="valor con espacios"
echo $TEST $TEST2
exit
```

---

### 1.5 UNSET

```bash
# Unset variable existente
export VAR=valor
echo $VAR
unset VAR
echo $VAR

# Unset variable inexistente
unset NOEXISTE

# Múltiples unsets
export A=1 B=2 C=3
unset A B C
echo $A $B $C

# Unset sin argumentos
unset

# Identificadores inválidos
unset 123VAR
unset VAR-NAME

# Unset variables del sistema
unset PATH
echo $PATH
export PATH=/usr/bin:/bin

# Unset USER
export MYUSER=$USER
unset USER
echo $USER
export USER=$MYUSER
```

**Comando para probar unset:**
```bash
valgrind --leak-check=full ./minishell
export TEST=valor
unset TEST
echo $TEST
exit
```

---

### 1.6 ENV

```bash
# Mostrar todas las variables
env

# Sin argumentos
env

# Después de export
export NUEVA_VAR=valor
env | grep NUEVA_VAR

# Después de unset
unset NUEVA_VAR
env | grep NUEVA_VAR
```

**Comando para probar env:**
```bash
valgrind --leak-check=full ./minishell
env
export TEST=valor
env | grep TEST
exit
```

---

### 1.7 EXIT

```bash
# Exit sin argumentos
exit

# Exit con código
exit 0
exit 1
exit 42
exit 255

# Exit con código negativo
exit -1

# Exit con código grande
exit 256
exit 300

# Exit con múltiples argumentos (error)
exit 1 2

# Exit con argumento no numérico
exit hola
exit "42"

# Exit con variable
export CODE=42
exit $CODE

# Exit con expresión
exit $?
```

**Comando para probar exit:**
```bash
# Test exit codes
./minishell -c "exit 0"; echo $?
./minishell -c "exit 42"; echo $?
./minishell -c "exit 256"; echo $?

# Test con valgrind
valgrind --leak-check=full ./minishell
exit
```

---

## 📂 2. REDIRECCIONES

### 2.1 INPUT (<)

```bash
# Redirigir desde archivo existente
echo "contenido" > /tmp/test_in.txt
cat < /tmp/test_in.txt
rm /tmp/test_in.txt

# Archivo inexistente
cat < archivo_inexistente.txt

# Múltiples redirecciones de input
echo "primero" > /tmp/file1.txt
echo "segundo" > /tmp/file2.txt
cat < /tmp/file1.txt < /tmp/file2.txt
rm /tmp/file1.txt /tmp/file2.txt

# Input con pipes
echo "test" > /tmp/test.txt
cat < /tmp/test.txt | grep test
rm /tmp/test.txt

# Input con variables
export FILE=/tmp/test.txt
echo "contenido" > $FILE
cat < $FILE
rm $FILE

# Sin permisos de lectura
echo "test" > /tmp/noperm.txt
chmod 000 /tmp/noperm.txt
cat < /tmp/noperm.txt
chmod 644 /tmp/noperm.txt
rm /tmp/noperm.txt
```

**Comando para probar input:**
```bash
echo "test" > /tmp/test.txt
valgrind --leak-check=full ./minishell
cat < /tmp/test.txt
exit
rm /tmp/test.txt
```

---

### 2.2 OUTPUT (>)

```bash
# Redirigir a nuevo archivo
echo "contenido" > /tmp/output.txt
cat /tmp/output.txt
rm /tmp/output.txt

# Sobreescribir archivo existente
echo "primero" > /tmp/output.txt
echo "segundo" > /tmp/output.txt
cat /tmp/output.txt
rm /tmp/output.txt

# Múltiples redirecciones
echo "test" > /tmp/file1.txt > /tmp/file2.txt
cat /tmp/file1.txt
cat /tmp/file2.txt
rm /tmp/file1.txt /tmp/file2.txt

# Output con pipes
echo "test" | cat > /tmp/output.txt
cat /tmp/output.txt
rm /tmp/output.txt

# Output a archivo con espacios en el nombre
echo "test" > "/tmp/archivo con espacios.txt"
cat "/tmp/archivo con espacios.txt"
rm "/tmp/archivo con espacios.txt"

# Sin permisos de escritura
touch /tmp/noperm.txt
chmod 000 /tmp/noperm.txt
echo "test" > /tmp/noperm.txt
chmod 644 /tmp/noperm.txt
rm /tmp/noperm.txt

# Directorio inexistente
echo "test" > /directorio/inexistente/file.txt
```

**Comando para probar output:**
```bash
valgrind --leak-check=full ./minishell
echo "test" > /tmp/output.txt
cat /tmp/output.txt
exit
rm /tmp/output.txt
```

---

### 2.3 APPEND (>>)

```bash
# Append a nuevo archivo
echo "línea1" >> /tmp/append.txt
echo "línea2" >> /tmp/append.txt
cat /tmp/append.txt
rm /tmp/append.txt

# Append vs truncate
echo "primero" > /tmp/test.txt
echo "segundo" >> /tmp/test.txt
cat /tmp/test.txt
rm /tmp/test.txt

# Múltiples appends
echo "1" >> /tmp/test.txt
echo "2" >> /tmp/test.txt
echo "3" >> /tmp/test.txt
cat /tmp/test.txt
rm /tmp/test.txt

# Append con pipes
echo "línea1" | cat >> /tmp/test.txt
echo "línea2" | cat >> /tmp/test.txt
cat /tmp/test.txt
rm /tmp/test.txt
```

**Comando para probar append:**
```bash
valgrind --leak-check=full ./minishell
echo "línea1" >> /tmp/append.txt
echo "línea2" >> /tmp/append.txt
cat /tmp/append.txt
exit
rm /tmp/append.txt
```

---

### 2.4 REDIRECCIONES COMBINADAS

```bash
# Input y output
echo "contenido" > /tmp/in.txt
cat < /tmp/in.txt > /tmp/out.txt
cat /tmp/out.txt
rm /tmp/in.txt /tmp/out.txt

# Múltiples inputs y outputs
echo "test" > /tmp/file1.txt
cat < /tmp/file1.txt > /tmp/file2.txt < /tmp/file1.txt
cat /tmp/file2.txt
rm /tmp/file1.txt /tmp/file2.txt

# Input, output y append
echo "a" > /tmp/test.txt
cat < /tmp/test.txt >> /tmp/test.txt
cat /tmp/test.txt
rm /tmp/test.txt

# Con pipes y redirecciones
echo "test" | cat > /tmp/out.txt | cat < /tmp/out.txt
rm /tmp/out.txt
```

**Comando para probar combinadas:**
```bash
valgrind --leak-check=full ./minishell
echo "test" > /tmp/in.txt
cat < /tmp/in.txt > /tmp/out.txt
exit
rm /tmp/in.txt /tmp/out.txt
```

---

## 🔄 3. PIPES

### 3.1 PIPES BÁSICOS

```bash
# Pipe simple
echo "hola" | cat

# Pipe con grep
echo "hola mundo" | grep mundo

# Pipe con wc
echo "una línea" | wc -l

# Múltiples pipes
echo "hola mundo" | cat | cat | cat

# Pipes con comandos complejos
ls -la | grep minishell | wc -l

# Pipe con variables
export VAR="test"
echo $VAR | cat
```

**Comando para probar pipes:**
```bash
valgrind --leak-check=full ./minishell
echo "hola" | cat
echo "hola mundo" | grep mundo
exit
```

---

### 3.2 PIPES MÚLTIPLES

```bash
# Tres comandos
echo "línea1\nlínea2\nlínea3" | cat | wc -l

# Cinco comandos
echo "test" | cat | cat | cat | cat

# Pipes con builtins
export VAR=valor | echo $VAR

# Pipes complejos
ls -la | grep ".c" | wc -l | cat

# Pipes con redirecciones
echo "test" | cat > /tmp/pipe.txt
cat /tmp/pipe.txt
rm /tmp/pipe.txt

# Pipes con heredoc
cat << EOF | grep test
línea1
test
línea3
EOF
```

**Comando para probar pipes múltiples:**
```bash
valgrind --leak-check=full ./minishell
echo "test" | cat | cat | cat
exit
```

---

### 3.3 PIPES CON ERRORES

```bash
# Comando inexistente en pipe
echo "test" | comandoinexistente

# Error en primer comando
comandoinexistente | cat

# Error en comando intermedio
echo "test" | comandoinexistente | cat

# Pipe vacío (error de sintaxis)
echo "test" |

# Múltiples pipes consecutivos
echo "test" || cat

# Pipe al inicio
| cat
```

**Comando para probar pipes con errores:**
```bash
valgrind --leak-check=full ./minishell
echo "test" | comandoinexistente
comandoinexistente | cat
exit
```

---

## 📝 4. HEREDOCS

### 4.1 HEREDOCS BÁSICOS

```bash
# Heredoc simple
cat << EOF
línea 1
línea 2
línea 3
EOF

# Heredoc con delimitador personalizado
cat << END
contenido
END

# Heredoc con variables
export VAR=valor
cat << EOF
El valor es: $VAR
EOF

# Heredoc con comillas simples (sin expansión)
cat << 'EOF'
$VAR no se expande
EOF

# Heredoc vacío
cat << EOF
EOF

# Múltiples líneas
cat << EOF
línea 1
línea 2
línea 3
línea 4
EOF
```

**Comando para probar heredocs:**
```bash
valgrind --leak-check=full ./minishell
cat << EOF
línea 1
línea 2
EOF
exit
```

---

### 4.2 HEREDOCS MÚLTIPLES

```bash
# Dos heredocs
cat << EOF1 << EOF2
primero
EOF1
segundo
EOF2

# Heredoc con pipe
cat << EOF | grep test
línea sin test
línea con test
otra línea
EOF

# Heredoc con redirección
cat << EOF > /tmp/heredoc.txt
contenido del heredoc
EOF
cat /tmp/heredoc.txt
rm /tmp/heredoc.txt
```

**Comando para probar heredocs múltiples:**
```bash
valgrind --leak-check=full ./minishell
cat << EOF | grep test
línea con test
otra línea
EOF
exit
```

---

### 4.3 HEREDOCS CON EXPANSIÓN

```bash
# Expansión de variables
export VAR=test
cat << EOF
$VAR
EOF

# Expansión de exit status
false
cat << EOF
Exit status: $?
EOF

# Sin expansión (delimitador entre comillas)
cat << 'EOF'
$VAR
$?
EOF

# Expansión en medio de texto
export USER=test
cat << EOF
Usuario: $USER en el sistema
EOF
```

**Comando para probar heredocs con expansión:**
```bash
valgrind --leak-check=full ./minishell
export VAR=test
cat << EOF
Valor: $VAR
EOF
exit
```

---

## 🔤 5. EXPANSIÓN DE VARIABLES

### 5.1 EXPANSIÓN BÁSICA

```bash
# Variable simple
export VAR=valor
echo $VAR

# Variable con texto
export NAME=Juan
echo "Hola $NAME"

# Variables inexistentes
echo $NOEXISTE

# Variables del sistema
echo $USER
echo $HOME
echo $PATH
echo $PWD

# Variable vacía
export EMPTY=
echo $EMPTY

# Múltiples variables
export A=1 B=2 C=3
echo $A $B $C
```

**Comando para probar expansión:**
```bash
valgrind --leak-check=full ./minishell
export VAR=test
echo $VAR
echo "$VAR"
echo '$VAR'
exit
```

---

### 5.2 EXPANSIÓN CON COMILLAS

```bash
# Dobles comillas (expande)
export VAR=valor
echo "$VAR"

# Comillas simples (no expande)
echo '$VAR'

# Comillas mixtas
echo "$VAR en 'comillas simples'"
echo '$VAR en "dobles comillas"'

# Variable entre comillas con espacios
export VAR="valor con espacios"
echo "$VAR"

# Sin comillas vs con comillas
export VAR="uno dos tres"
echo $VAR
echo "$VAR"
```

**Comando para probar expansión con comillas:**
```bash
valgrind --leak-check=full ./minishell
export VAR="valor con espacios"
echo $VAR
echo "$VAR"
echo '$VAR'
exit
```

---

### 5.3 EXPANSIÓN DE EXIT STATUS ($?)

```bash
# Después de comando exitoso
echo "test"
echo $?

# Después de comando fallido
comandoinexistente
echo $?

# Después de cada comando
ls > /dev/null
echo $?
cd /directorio/inexistente
echo $?

# En heredoc
true
cat << EOF
Exit: $?
EOF

false
cat << EOF
Exit: $?
EOF

# Con pipes
echo "test" | grep test
echo $?
echo "test" | grep nada
echo $?
```

**Comando para probar exit status:**
```bash
valgrind --leak-check=full ./minishell
echo "test"
echo $?
comandoinexistente
echo $?
exit
```

---

### 5.4 CASOS COMPLEJOS

```bash
# Variable con números
export VAR123=valor
echo $VAR123

# Variables concatenadas
export A=uno B=dos
echo $A$B

# Variable en medio de texto
export VAR=test
echo "pre${VAR}post"
echo pre${VAR}post

# Variable con caracteres especiales
export VAR="valor*con?especiales"
echo $VAR

# Expansión múltiple
export A=B
export B=C
echo $A $B

# Variable con $
export VAR='$HOME'
echo $VAR
```

**Comando para probar casos complejos:**
```bash
valgrind --leak-check=full ./minishell
export A=uno B=dos
echo $A$B
echo "$A $B"
exit
```

---

## ⚠️ 6. SINTAXIS Y ERRORES

### 6.1 ERRORES DE SINTAXIS

```bash
# Pipe al inicio
| cat

# Pipe al final
echo "test" |

# Pipes dobles
echo "test" || cat

# Redirección sin archivo
cat <

# Redirección sin comando
< archivo.txt

# Output sin archivo
cat >

# Múltiples operadores
echo "test" | | cat
echo "test" > > file.txt
cat < < file.txt

# Operadores no soportados
echo "test" && echo "test2"
echo "test" || echo "test2"

# Paréntesis (no soportados)
(echo "test")

# Wildcards (no soportados en mandatory)
echo *.c

# Comillas sin cerrar
echo "hola
echo 'hola
```

**Comando para probar errores de sintaxis:**
```bash
valgrind --leak-check=full ./minishell
| cat
echo "test" |
cat <
exit
```

---

### 6.2 COMANDOS INEXISTENTES

```bash
# Comando inexistente
comandoinexistente

# Comando con ruta incorrecta
/usr/bin/comandoinexistente

# Binario sin permisos
touch /tmp/noperm
chmod 000 /tmp/noperm
/tmp/noperm
chmod 755 /tmp/noperm
rm /tmp/noperm

# PATH vacío
export OLDPATH=$PATH
export PATH=
ls
export PATH=$OLDPATH
```

**Comando para probar comandos inexistentes:**
```bash
valgrind --leak-check=full ./minishell
comandoinexistente
/usr/bin/noexiste
exit
```

---

### 6.3 ARGUMENTOS INVÁLIDOS

```bash
# cd con argumentos inválidos
cd arg1 arg2

# export con identificadores inválidos
export 123VAR=valor
export VAR-NAME=valor
export =valor

# unset con identificadores inválidos
unset 123VAR

# exit con argumentos inválidos
exit abc
exit 1 2
```

**Comando para probar argumentos inválidos:**
```bash
valgrind --leak-check=full ./minishell
cd /tmp /usr
export 123VAR=valor
exit abc
```

---

## 🎯 7. SEÑALES

### 7.1 CTRL-C (SIGINT)

```bash
# En prompt
# Presionar Ctrl-C

# Durante ejecución de comando
sleep 5
# Presionar Ctrl-C mientras duerme

# Durante heredoc
cat << EOF
# Presionar Ctrl-C

# En pipe largo
sleep 10 | cat
# Presionar Ctrl-C
```

**Comando para probar SIGINT:**
```bash
valgrind --leak-check=full ./minishell
# Presionar Ctrl-C varias veces
sleep 5
# Presionar Ctrl-C durante sleep
exit
```

---

### 7.2 CTRL-\ (SIGQUIT)

```bash
# En prompt
# Presionar Ctrl-\

# Durante ejecución
sleep 5
# Presionar Ctrl-\ (debe ser ignorado)
```

---

### 7.3 CTRL-D (EOF)

```bash
# En prompt vacío (debe salir)
# Presionar Ctrl-D

# Después de texto
echo "test"
# Presionar Ctrl-D
```

**Comando para probar EOF:**
```bash
valgrind --leak-check=full ./minishell
# Presionar Ctrl-D (debe salir limpiamente)
```

---

## 🎪 8. CASOS EDGE

### 8.1 STRINGS VACÍOS

```bash
# Comando vacío
# Presionar Enter

# Echo vacío
echo

# Variable vacía
export VAR=
echo $VAR

# Argument vacío
echo "" "" ""

# Input vacío
cat << EOF
EOF
```

**Comando para probar strings vacíos:**
```bash
valgrind --leak-check=full ./minishell
echo
export VAR=
echo $VAR
exit
```

---

### 8.2 ESPACIOS Y TABS

```bash
# Múltiples espacios
echo    test    test

# Tabs
echo	test	test

# Espacios al inicio
   echo "test"

# Espacios al final
echo "test"   

# Solo espacios
echo "     "

# Comandos con espacios extraños
   ls    -la   |   grep   minishell
```

**Comando para probar espacios:**
```bash
valgrind --leak-check=full ./minishell
echo    test    test
   echo "test"
exit
```

---

### 8.3 RUTAS Y ARCHIVOS

```bash
# Ruta absoluta
/bin/echo "test"

# Ruta relativa
./minishell

# PATH con múltiples directorios
export PATH=/usr/bin:/bin:/usr/local/bin
ls

# Archivo actual
cat ./Makefile

# Archivo en subdirectorio
cat ./src/main.c

# Archivo con espacios
touch "/tmp/archivo con espacios.txt"
cat "/tmp/archivo con espacios.txt"
rm "/tmp/archivo con espacios.txt"

# Archivo con caracteres especiales
touch /tmp/archivo\$especial.txt
cat /tmp/archivo\$especial.txt
rm /tmp/archivo\$especial.txt
```

**Comando para probar rutas:**
```bash
valgrind --leak-check=full ./minishell
/bin/echo "test"
cat ./Makefile
exit
```

---

### 8.4 VARIABLES ESPECIALES

```bash
# HOME
echo $HOME
cd $HOME
pwd

# USER
echo $USER

# PATH
echo $PATH

# PWD y OLDPWD
pwd
echo $PWD
cd /tmp
echo $OLDPWD

# SHLVL
echo $SHLVL

# _ (último argumento)
echo $_ 

# Variables inexistentes
echo $NOEXISTE

# Variables con nombres similares
export VAR=1
export VARA=2
echo $VAR $VARA
```

**Comando para probar variables especiales:**
```bash
valgrind --leak-check=full ./minishell
echo $HOME $USER $PWD
export TEST=valor
echo $TEST
exit
```

---

### 8.5 MEMORIA Y LÍMITES

```bash
# Comando muy largo
echo "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"

# Muchos argumentos
echo 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20

# Muchos pipes
echo "test" | cat | cat | cat | cat | cat | cat | cat | cat | cat | cat

# Variable muy larga
export LONGVAR="aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
echo $LONGVAR

# Muchas variables
export A=1 B=2 C=3 D=4 E=5 F=6 G=7 H=8 I=9 J=10
echo $A $B $C $D $E $F $G $H $I $J

# Heredoc muy largo
cat << EOF
línea 1
línea 2
línea 3
...
línea 100
EOF
```

---

## 🔍 9. DETECCIÓN DE LEAKS - COMANDOS VALGRIND

### 9.1 Test Completo de Builtins

```bash
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --track-fds=yes ./minishell << 'TESTEOF'
echo "test builtins"
pwd
cd /tmp
pwd
cd -
export VAR=valor
echo $VAR
unset VAR
env | grep VAR
exit
TESTEOF
```

---

### 9.2 Test Completo de Redirecciones

```bash
# Preparar archivos
echo "test" > /tmp/test_in.txt

valgrind --leak-check=full --show-leak-kinds=all --track-fds=yes ./minishell << 'TESTEOF'
cat < /tmp/test_in.txt
echo "output" > /tmp/test_out.txt
cat /tmp/test_out.txt
echo "append" >> /tmp/test_out.txt
cat /tmp/test_out.txt
exit
TESTEOF

# Limpiar
rm /tmp/test_in.txt /tmp/test_out.txt
```

---

### 9.3 Test Completo de Pipes

```bash
valgrind --leak-check=full --show-leak-kinds=all ./minishell << 'TESTEOF'
echo "test" | cat
echo "hola mundo" | grep mundo
ls | grep minishell
echo "test" | cat | cat | cat
exit
TESTEOF
```

---

### 9.4 Test Completo de Heredocs

```bash
valgrind --leak-check=full --show-leak-kinds=all ./minishell << 'TESTEOF'
cat << EOF
línea 1
línea 2
EOF
export VAR=test
cat << EOF
Variable: $VAR
EOF
exit
TESTEOF
```

---

### 9.5 Test Completo de Expansión

```bash
valgrind --leak-check=full --show-leak-kinds=all ./minishell << 'TESTEOF'
export VAR=valor
echo $VAR
echo "$VAR"
echo '$VAR'
echo "test"
echo $?
comandoinexistente
echo $?
exit
TESTEOF
```

---

### 9.6 Test de Señales con Timeout

```bash
timeout 5 valgrind --leak-check=full ./minishell << 'TESTEOF'
echo "antes de sleep"
sleep 3
echo "después de sleep"
exit
TESTEOF
```

---

### 9.7 Test de Casos Edge

```bash
valgrind --leak-check=full --show-leak-kinds=all ./minishell << 'TESTEOF'
echo

export VAR=
echo $VAR
echo    múltiples    espacios
echo "     "
exit
TESTEOF
```

---

### 9.8 Test Combinado Exhaustivo

```bash
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --track-fds=yes --verbose ./minishell << 'TESTEOF'
# Builtins básicos
echo "Iniciando tests"
pwd
cd /tmp
pwd
cd -

# Export y variables
export VAR1=valor1
export VAR2="valor con espacios"
echo $VAR1 $VAR2

# Redirecciones
echo "test redirect" > /tmp/test.txt
cat < /tmp/test.txt
echo "append" >> /tmp/test.txt
cat /tmp/test.txt

# Pipes
echo "test pipe" | cat
echo "hola mundo" | grep mundo | cat

# Heredoc
cat << EOF
Heredoc test
$VAR1
EOF

# Expansión de exit status
ls > /dev/null
echo $?
comandoinexistente 2>/dev/null
echo $?

# Unset
unset VAR1 VAR2
echo $VAR1

# Env
env | grep USER

# Cleanup
rm /tmp/test.txt

# Exit
exit 0
TESTEOF

echo "Exit code: $?"
```

---

## 📊 10. SCRIPT DE AUTOMATIZACIÓN DE TESTS

Guarda este script como `run_leak_tests.sh`:

```bash
#!/bin/bash

# Colores
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

echo -e "${YELLOW}========================================${NC}"
echo -e "${YELLOW}  MINISHELL LEAK DETECTION TEST SUITE  ${NC}"
echo -e "${YELLOW}========================================${NC}"

# Compilar
echo -e "\n${YELLOW}Compilando minishell...${NC}"
make re
if [ $? -ne 0 ]; then
    echo -e "${RED}Error de compilación${NC}"
    exit 1
fi

# Directorio de logs
mkdir -p leak_test_logs
LOGDIR="leak_test_logs"
TIMESTAMP=$(date +%Y%m%d_%H%M%S)

# Test 1: Builtins
echo -e "\n${YELLOW}Test 1: Builtins${NC}"
valgrind --leak-check=full --show-leak-kinds=all --log-file="${LOGDIR}/test1_builtins_${TIMESTAMP}.log" ./minishell << 'EOF'
echo "test"
pwd
cd /tmp
cd -
export VAR=valor
echo $VAR
unset VAR
exit
EOF

# Test 2: Redirecciones
echo -e "\n${YELLOW}Test 2: Redirecciones${NC}"
echo "test" > /tmp/test_in.txt
valgrind --leak-check=full --show-leak-kinds=all --log-file="${LOGDIR}/test2_redirects_${TIMESTAMP}.log" ./minishell << 'EOF'
cat < /tmp/test_in.txt
echo "output" > /tmp/test_out.txt
cat /tmp/test_out.txt
exit
EOF
rm /tmp/test_in.txt /tmp/test_out.txt 2>/dev/null

# Test 3: Pipes
echo -e "\n${YELLOW}Test 3: Pipes${NC}"
valgrind --leak-check=full --show-leak-kinds=all --log-file="${LOGDIR}/test3_pipes_${TIMESTAMP}.log" ./minishell << 'EOF'
echo "test" | cat
echo "hola mundo" | grep mundo
exit
EOF

# Test 4: Heredocs
echo -e "\n${YELLOW}Test 4: Heredocs${NC}"
valgrind --leak-check=full --show-leak-kinds=all --log-file="${LOGDIR}/test4_heredocs_${TIMESTAMP}.log" ./minishell << 'EOF'
cat << END
línea 1
línea 2
END
exit
EOF

# Test 5: Expansión
echo -e "\n${YELLOW}Test 5: Expansión de Variables${NC}"
valgrind --leak-check=full --show-leak-kinds=all --log-file="${LOGDIR}/test5_expansion_${TIMESTAMP}.log" ./minishell << 'EOF'
export VAR=test
echo $VAR
echo "$VAR"
echo '$VAR'
echo $?
exit
EOF

# Analizar resultados
echo -e "\n${YELLOW}========================================${NC}"
echo -e "${YELLOW}  ANÁLISIS DE RESULTADOS  ${NC}"
echo -e "${YELLOW}========================================${NC}"

LEAK_COUNT=0
for logfile in ${LOGDIR}/*_${TIMESTAMP}.log; do
    TEST_NAME=$(basename $logfile)
    if grep -q "no leaks are possible" "$logfile"; then
        echo -e "${GREEN}✓ $TEST_NAME: SIN LEAKS${NC}"
    elif grep -q "definitely lost: 0 bytes" "$logfile" && grep -q "indirectly lost: 0 bytes" "$logfile"; then
        echo -e "${GREEN}✓ $TEST_NAME: OK (leaks reachables solamente)${NC}"
    else
        echo -e "${RED}✗ $TEST_NAME: LEAKS DETECTADOS${NC}"
        LEAK_COUNT=$((LEAK_COUNT + 1))
        
        # Mostrar resumen de leaks
        echo "  Resumen:"
        grep "definitely lost:" "$logfile" | head -1
        grep "indirectly lost:" "$logfile" | head -1
    fi
done

# Resumen final
echo -e "\n${YELLOW}========================================${NC}"
if [ $LEAK_COUNT -eq 0 ]; then
    echo -e "${GREEN}✓ TODOS LOS TESTS PASARON SIN LEAKS${NC}"
else
    echo -e "${RED}✗ $LEAK_COUNT TEST(S) CON LEAKS DETECTADOS${NC}"
    echo -e "${YELLOW}Revisa los logs en: ${LOGDIR}/${NC}"
fi
echo -e "${YELLOW}========================================${NC}"
```

**Uso:**
```bash
chmod +x run_leak_tests.sh
./run_leak_tests.sh
```

---

## 🎯 11. CHECKLIST DE PRUEBAS MANUALES

### ✓ Antes de entregar, probar TODOS estos casos:

**Builtins:**
- [ ] echo con y sin -n
- [ ] cd con HOME, ~, -, rutas relativas/absolutas
- [ ] pwd
- [ ] export sin args, con args, identificadores inválidos
- [ ] unset variables existentes e inexistentes
- [ ] env
- [ ] exit con y sin argumentos, códigos numéricos y no numéricos

**Redirecciones:**
- [ ] Input < desde archivo existente e inexistente
- [ ] Output > a nuevo archivo y sobrescribir
- [ ] Append >> a archivo nuevo y existente
- [ ] Múltiples redirecciones
- [ ] Redirecciones combinadas con pipes

**Pipes:**
- [ ] Pipe simple
- [ ] Múltiples pipes (3+)
- [ ] Pipes con builtins
- [ ] Pipes con comandos externos
- [ ] Pipes con errores

**Heredocs:**
- [ ] Heredoc simple
- [ ] Heredoc con expansión de variables
- [ ] Heredoc con $?
- [ ] Heredoc con pipes
- [ ] Heredoc con delimitadores entre comillas

**Expansión:**
- [ ] Variables simples ($VAR)
- [ ] Variables con comillas dobles ("$VAR")
- [ ] Variables con comillas simples ('$VAR')
- [ ] Exit status ($?)
- [ ] Variables inexistentes
- [ ] Variables del sistema ($HOME, $USER, $PATH)

**Sintaxis:**
- [ ] Errores de sintaxis (|, <, >, etc. mal colocados)
- [ ] Comandos inexistentes
- [ ] Argumentos inválidos

**Señales:**
- [ ] Ctrl-C en prompt
- [ ] Ctrl-C durante ejecución
- [ ] Ctrl-\ (debe ser ignorado)
- [ ] Ctrl-D (debe salir)

**Casos Edge:**
- [ ] Comandos vacíos
- [ ] Variables vacías
- [ ] Strings con espacios
- [ ] Rutas con espacios
- [ ] Múltiples espacios/tabs

**Memoria:**
- [ ] Valgrind en todos los flujos principales
- [ ] Sin leaks en salida normal
- [ ] Sin leaks con Ctrl-C
- [ ] Sin leaks con Ctrl-D
- [ ] File descriptors cerrados correctamente

---

## 🚀 12. COMANDO ÚNICO PARA PROBAR TODO

```bash
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --track-fds=yes --verbose --log-file=full_test.log ./minishell << 'ENDOFTEST'
echo "=== TEST COMPLETO MINISHELL ==="
echo "Test 1: Builtins básicos"
pwd
cd /tmp
pwd
cd -
echo "Test 2: Variables"
export TEST=valor
echo $TEST
echo "$TEST"
echo '$TEST'
unset TEST
echo "Test 3: Exit status"
ls > /dev/null
echo $?
comandoinexistente 2>/dev/null
echo $?
echo "Test 4: Redirecciones"
echo "redirect test" > /tmp/minitest.txt
cat < /tmp/minitest.txt
echo "append" >> /tmp/minitest.txt
cat /tmp/minitest.txt
echo "Test 5: Pipes"
echo "pipe test" | cat
echo "grep test" | grep test
echo "Test 6: Heredoc"
cat << EOF
heredoc line 1
heredoc line 2
EOF
echo "Test 7: Expansión en heredoc"
export VAR=test
cat << EOF
Variable: $VAR
Exit: $?
EOF
rm /tmp/minitest.txt
echo "=== FIN DE TESTS ==="
exit 0
ENDOFTEST
echo "Revisa full_test.log para resultados de valgrind"
```

---

## 📝 NOTAS FINALES

1. **Ejecuta todos los tests con valgrind** para detectar leaks
2. **Revisa los file descriptors** con `--track-fds=yes`
3. **Prueba con diferentes shells** para comparar comportamiento
4. **Documenta los leaks encontrados** y su ubicación en el código
5. **Usa el script de automatización** para tests regulares durante desarrollo

**Comando rápido para comparar con bash:**
```bash
# Ejecuta el mismo comando en bash y minishell
bash -c 'echo $USER'
./minishell -c 'echo $USER'
```

¡Buena suerte con las pruebas! 🍀
