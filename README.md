# Minishell

Minishell is a small Unix shell developed in C as part of the 42 curriculum.  
The goal of this project is to recreate a simplified version of bash, handling commands, pipes, redirections, and signals, while deepening the understanding of processes and file descriptors in Unix systems.

---

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
