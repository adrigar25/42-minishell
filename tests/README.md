Minishell tests
================

Este directorio contiene un runner de pruebas automáticas básicas para el
proyecto `minishell`.

Archivos
-------
- `run_tests.sh`: script que ejecuta una batería de pruebas no interactivas
  para el binario obligatorio `minishell` y, si existe, para `minishell_bonus`.

Cómo ejecutar
-------------
1) Compila tu proyecto desde la raíz del repo:

```bash
make
```

o si quieres probar el bonus:

```bash
make bonus
```

2) Ejecuta el runner de pruebas:

```bash
./tests/run_tests.sh
```

Notas
-----
- El runner es intencionadamente simple y no cubre todos los casos edge del
  enunciado (por ejemplo, interacción con señales y comportamiento del prompt).
- Muchos tests (señales, edición de línea, control del prompt) deben ejecutarse
  manualmente en una terminal interactiva y están listados en `manual_test`.
- Puedes ampliar `run_tests.sh` añadiendo más entradas en la función `run_all_for`.
