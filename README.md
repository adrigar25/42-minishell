# Minishell

# 🌟 Introducción
El proyecto `minishell` es un ejercicio fundamental en el currículo de 42 que te desafía a implementar una versión simplificada de una shell en C. 🐚 A través de este reto, aprenderás a gestionar la ejecución de comandos, la manipulación de tuberías, redirecciones y el manejo de variables de entorno, replicando funcionalidades esenciales de una terminal real.

Durante el desarrollo de `minishell`, profundizarás en conceptos clave como la creación y sincronización de procesos, el control de señales, la gestión de descriptores de archivo y la implementación de comandos internos. ⚙️ Este proyecto es una excelente oportunidad para fortalecer tus habilidades en programación de sistemas y comprender cómo interactúan los programas con el sistema operativo.

La dificultad de este ejercicio es intermedia-avanzada y te preparará para abordar proyectos más complejos en el mundo de la programación de sistemas. 🧩 El éxito depende de tu capacidad para analizar problemas, estructurar tu código y seguir las normas de codificación de 42. 🚦 ¡Sumérgete en el desarrollo de tu propia shell y lleva tus conocimientos al siguiente nivel! 💻🚀

## 📋 Instrucciones generales

- Tu proyecto deberá estar escrito en C.
- Tu proyecto debe estar escrito siguiendo la Norma. Si tienes archivos o funciones adicionales, estas están incluidas en la verificación de la Norma y tendrás un 0 si hay algún error de norma en cualquiera de ellos.
- Tus funciones no deben terminar de forma inesperada (segfault, bus error, double free, etc) excepto en el caso de comportamientos indefinidos. Si esto sucede, tu proyecto será considerado no funcional y recibirás un 0 durante la evaluación.
- Toda la memoria asignada en el heap deberá liberarse adecuadamente cuando sea necesario. No se permitirán leaks de memoria.
- Si el enunciado lo requiere, deberás entregar un Makefile que compilará tus archivos fuente al output requerido con las flags -Wall, -Werror y -Wextra, utilizar cc y por supuesto tu Makefile no debe hacer relink.
- Tu Makefile debe contener al menos las normas $(NAME), all, clean, fclean y re.
- Para entregar los bonus de tu proyecto deberás incluir una regla bonus en tu Makefile, en la que añadirás todos los headers, librerías o funciones que estén prohibidas en la parte principal del proyecto. Los bonus deben estar en archivos distintos _bonus.{c/h}. La parte obligatoria y los bonus se evalúan por separado.
- Si tu proyecto permite el uso de la libft, deberás copiar su fuente y sus Makefile asociados en un directorio libft con su correspondiente Makefile. El Makefile de tu proyecto debe compilar primero la librería utilizando su Makefile, y después compilar el proyecto.
- Te recomendamos crear programas de prueba para tu proyecto, aunque este trabajo no será entregado ni evaluado. Te dará la oportunidad de verificar que tu programa funciona correctamente durante tu evaluación y la de otros compañeros.
- Y sí, tienes permitido utilizar estas pruebas durante tu evaluación o la de otros compañeros.
- Entrega tu trabajo en tu repositorio Git asignado. Solo el trabajo de tu repositorio Git será evaluado. Si Deepthought evalúa tu trabajo, lo hará después de tus compañeros. Si se encuentra un error durante la evaluación de Deepthought, esta habrá terminado.

## Features

- Execute simple commands (e.g., `ls`, `echo`, `cat`).
- Handle multiple commands using pipes (`|`).
- Support input (`<`) and output (`>`, `>>`) redirections.
- Manage environment variables (`$PATH`, `$HOME`, etc.).
- Implement built-in commands:
  - `echo`
  - `cd`
  - `pwd`
  - `export`
  - `unset`
  - `env`
  - `exit`
- Properly handle signals:
  - `Ctrl-C`
  - `Ctrl-D`
  - `Ctrl-\`
- Command execution with correct exit status codes.

---

## Objectives

- Understand process creation and synchronization using `fork`, `execve`, and `wait`.
- Manipulate file descriptors to handle pipes and redirections.
- Implement a custom parser to handle quotes, environment variables, and operators.
- Reproduce a minimal but functional shell environment.

---

## Compilation

To compile the project, run:

```bash
make
