Minishell tests
================

Este directorio contiene un runner de pruebas automáticas básicas para el
proyecto `minishell`.

Archivos
-------

- `run_tests.sh`: script que ejecuta una batería de pruebas no interactivas
  para el binario obligatorio `minishell` y, si existe, para `minishell_bonus`.

Este directorio contiene un runner que ejecuta la binary `minishell` dentro de
un PTY (para emular una terminal interactiva) y compara la salida con
expectativas definidas en `test_cases.txt`.

Contenido principal

- `test_cases.txt`: casos de prueba (formato `---test---`, con `name`, `input`, `output`).
- `run_tests.sh`: runner que ejecuta cada caso en la shell bajo prueba.
- `.minitest/`: directorio temporal usado por el runner (aquí se escriben archivos
  de prueba y el `pty_driver.py`).
- `logs/`: logs generados por el runner (`run_<timestamp>.log`).

Cómo usarlo

1. Compila tu proyecto:

```bash
make
```

2. Ejecuta las pruebas (timeout recomendado para shells lentas):

```bash
TEST_PROMPT_TIMEOUT=4.0 ./tests/run_tests.sh --verbose
```

Opciones y variables útiles

- `--verbose`: muestra RAW OUTPUT por test para depuración.
- `TEST_PROMPT_TIMEOUT`: segundos (float) a esperar por prompt entre comandos.
- `TEST_PROMPT_PATTERNS`: patrones separados por `||` usados para detectar el prompt.

Consejos

- Prefiere `printf` en `input` cuando necesites control exacto de saltos de línea
  y escapes (evita `echo -n` y `echo -e`).
- Todos los artefactos de pruebas se crean en `tests/.minitest`.

Si quieres documentación más extensa sobre el runner, ver `../README.md`.
