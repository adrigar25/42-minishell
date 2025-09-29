#!/usr/bin/env bash
set -euo pipefail

THIS_DIR=$(cd "$(dirname "$0")" && pwd)
ROOT_DIR=$(cd "$THIS_DIR/.." && pwd)
MINI="$ROOT_DIR/minishell"
BONUS="$ROOT_DIR/minishell_bonus"
# Use tests/.minitest so all temp files live under the tests folder
TMPDIR="$THIS_DIR/.minitest"
mkdir -p "$TMPDIR"

# Flags
VERBOSE=0
if [ "${1:-}" = "--verbose" ]; then
  VERBOSE=1
fi

# Logging: create logs dir and tee output to timestamped logfile
# Keep colors on the terminal but write a cleaned version (no ANSI/CSI)
# to the logfile to make it readable.
LOGDIR="$THIS_DIR/logs"
mkdir -p "$LOGDIR"
TS=$(date +"%Y%m%d_%H%M%S")
# Single clean logfile (colours stripped and structured summaries written here).
LOGFILE="$LOGDIR/run_$TS.log"
# Use tee to duplicate output: terminal gets original (with colors),
# the logfile gets the output piped through perl to strip ANSI/CSI sequences.
exec > >(tee >(perl -0777 -pe '
  # strip CSI (ANSI) sequences
  s/\x1b\[[0-9;?;:]*[ -\/]*[@-~]//g;
  # strip OSC sequences
  s/\x1b\].*?(?:\x07|\x1b\\)//gs;
' >> "$LOGFILE")) 2>&1
printf 'Logfile: %s\n' "$LOGFILE"
# initialize/overwrite logfile
printf 'Clean test run log - %s\n\n' "$TS" > "$LOGFILE"

norm() {
  # Trim trailing whitespace/newlines for stable comparison
  # Trim leading and trailing whitespace/newlines for stable comparison
  printf '%s' "$1" | sed -e 's/^[[:space:]]*//' -e 's/[[:space:]]*$//'
}

# Colors (use $'...' so variables contain the actual ESC byte)
RED=$'\033[31m'
GRN=$'\033[32m'
YEL=$'\033[33m'
CYA=$'\033[96m'
RST=$'\033[0m'

run_test() {
  local bin="$1" name="$2" input="$3" expected="$4"
  printf "  %-20s " "$name"
  # replace .minitest placeholders in input and expected so files are created under tests/
  input_exec="${input//.minitest/$TMPDIR}"
  expected_exec="${expected//.minitest/$TMPDIR}"
  # PTY tuning: allow overriding via env vars TEST_PROMPT_TIMEOUT and TEST_PROMPT_PATTERNS
  # TEST_PROMPT_TIMEOUT: float seconds to wait for prompt between commands (default 2.0)
  # TEST_PROMPT_PATTERNS: '||' separated byte patterns to treat as prompt markers (default: minishell>||> )
  PROMPT_TIMEOUT=${TEST_PROMPT_TIMEOUT:-2.0}
  PROMPT_PATTERNS=${TEST_PROMPT_PATTERNS:-$'minishell>||> '}
  # Prefer running under a PTY so readline-based shells behave like an interactive terminal
  if command -v python3 >/dev/null 2>&1; then
    # write a small python PTY driver to TMPDIR so we avoid here-doc parsing issues
    DRIVER="$TMPDIR/pty_driver.py"
  cat > "$DRIVER" <<'PY'
import sys, os, pty, select, time

binpath = sys.argv[1]
timeout = float(sys.argv[2]) if len(sys.argv) > 2 else 1.0
patterns = sys.argv[3].split('||') if len(sys.argv) > 3 else ['minishell>', '> ']

data = sys.stdin.read().splitlines(True)
master, slave = pty.openpty()
# disable echo on the slave side so the child process won't echo our input
import termios
try:
  attrs = termios.tcgetattr(slave)
  attrs[3] = attrs[3] & ~termios.ECHO
  termios.tcsetattr(slave, termios.TCSANOW, attrs)
except Exception:
  # if termios fails, continue without disabling echo
  pass
pid = os.fork()
if pid == 0:
  os.setsid()
  os.dup2(slave, 0)
  os.dup2(slave, 1)
  os.dup2(slave, 2)
  os.execv(binpath, [binpath])
else:
  os.close(slave)
  buf = bytearray()

  def read_available(timeout=0.1):
    out = bytearray()
    end = time.time() + timeout
    while time.time() < end:
      r, _, _ = select.select([master], [], [], 0.05)
      if master in r:
        try:
          chunk = os.read(master, 1024)
        except OSError:
          break
        if not chunk:
          break
        out.extend(chunk)
      else:
        break
    return out

  def wait_for_prompt(timeout=timeout):
    total = bytearray()
    end = time.time() + timeout
    while time.time() < end:
      chunk = read_available(0.1)
      if chunk:
        total.extend(chunk)
        for p in patterns:
          if p.encode() in total:
            return total
      else:
        time.sleep(0.01)
    return total

  # consume initial banner/prompt and discard so the ASCII-art/banner doesn't pollute results
  try:
    _ = wait_for_prompt()
  except Exception:
    pass

  for line in data:
    try:
      os.write(master, line.encode())
    except Exception:
      pass
    # collect until prompt after this command
    chunk = wait_for_prompt()
    if chunk:
      buf.extend(chunk)

  try:
    os.write(master, b'exit\n')
  except Exception:
    pass

  while True:
    chunk = read_available(0.2)
    if chunk:
      buf.extend(chunk)
    else:
      break

  sys.stdout.buffer.write(buf)
PY
    chmod +x "$DRIVER" || true
    out=$(printf '%s\n' "$input_exec" 'exit' | python3 "$DRIVER" "$bin" "$PROMPT_TIMEOUT" "$PROMPT_PATTERNS" 2>/dev/null || true)
  else
  out=$(printf '%s\n' "$input_exec" 'exit' | "$bin" 2>&1 || true)
  fi
  # Debug: when verbose, show raw output captured from the driver
  if [ "$VERBOSE" -eq 1 ]; then
    printf '    %b--- RAW OUTPUT --- %b\n' "$YEL" "$RST"
    # Show non-printing bytes visibly by piping through sed to escape control chars
    printf '%s' "$out" | sed -n '1,200p' | sed 's/^/    /'
    printf '    %b--- END RAW OUTPUT --- %b\n' "$YEL" "$RST"
  fi

  # Cleaning pipeline:
  # 1) strip ANSI/CSI escape sequences
  # 2) remove ASCII-art/comment lines starting with '#'
  # 3) remove 'Welcome' banner lines
  # 4) strip prompt prefixes like '...minishell...> '
  out_no_esc=$(printf '%s' "$out" | perl -pe 's/\e\[[0-9;?;]*[ -~]//g')
  out_no_art=$(printf '%s' "$out_no_esc" | sed -E '/^# /d; /Welcome to Minishell!/d')
  # remove prompt-like fragments anywhere in the output (covers cases where
  # the shell printed no trailing newline, e.g. `echo -n hello` so the prompt
  # appears on the same line). Use Perl for a robust, global replacement.
  out_no_prompt=$(printf '%s' "$out_no_art" | perl -0777 -pe 's/[^\n>]{0,200}> ?//g')

  out_clean="$out_no_prompt"
  if [ "$VERBOSE" -eq 0 ]; then
    # remove lines that contain the input lines (shell may echo them with prompt prefix)
    if [ -n "$input" ]; then
      while IFS= read -r il; do
        il_trim=$(printf '%s' "$il" | sed -e 's/^[[:space:]]*//' -e 's/[[:space:]]*$//')
        if [ -n "$il_trim" ]; then
          # escape sed special chars
    il_esc=$(printf '%s' "$il_trim" | sed -e 's/[\\\/&]/\\&/g')
    out_clean=$(printf '%s\n' "$out_clean" | sed "/${il_esc}/d" )
        fi
      done <<< "$input"
    fi
  fi
  # remove leftover lines that are only control sequences or empty
  out_clean=$(printf '%s' "$out_clean" | sed -E '/^[[:space:]]*$/d' | sed -E '/^exit$/d')
  # normalize numeric-only lines (wc output may be padded with spaces)
  out_clean=$(printf '%s' "$out_clean" | sed -E 's/^[[:space:]]+([0-9]+)[[:space:]]*$/\1/')
  out_norm=$(norm "$out_clean")
  exp_norm=$(norm "$expected_exec")
  if [ "$out_norm" = "$exp_norm" ]; then
    status='(OK)'
  else
    status='(FAIL)'
  fi

  # Build the block in the example.log format
  printf '[NAME_TEST]\n'
  printf 'test: %s\n\n' "$name"
  printf 'minishell_output:\n%s\n\n' "$out_clean"
  printf 'expected:\n%s\n\n' "$expected_exec"
  printf '%s\n\n' "$status"
  printf '[END_TEST]\n\n'
  # Print a short status line for quick terminal scan
  if [ "$status" = '(OK)' ]; then
    printf "%b[OK]%b %s\n" "$GRN" "$RST" "$name"
    return 0
  else
    printf "%b[FAIL]%b %s\n" "$RED" "$RST" "$name"
    return 1
  fi
}

run_all_for() {
  local bin="$1"
  printf '\n%s=== Running tests for %s ===%s\n' "$CYA" "$bin" "$RST"
  if [ "$VERBOSE" -eq 1 ]; then
    printf '%b[VERBOSE]%b Showing raw output (no input removal)\n' "$YEL" "$RST"
  fi
  local total=0
  local passed=0
  local failed=0

  # Parse test_cases.txt
  local name=''
  local input=''
  local output=''
  local mode=none
  while IFS= read -r line || [ -n "$line" ]; do
    if [ "$line" = '---test---' ]; then
      # run previous test if any
      if [ -n "$name" ]; then
        run_test "$bin" "$name" "$input" "$output"
        rc=$?
        total=$((total+1))
        if [ $rc -eq 0 ]; then passed=$((passed+1)); else failed=$((failed+1)); fi
      fi
      # reset for next
      name=''
      input=''
      output=''
      mode=none
      continue
    fi

    case "$line" in
      name:*)
        name="${line#name: }"
        ;;
      input:)
        mode=reading_input
        ;;
      output:)
        mode=reading_output
        ;;
      *)
        if [ "$mode" = "reading_input" ]; then
          if [ -z "$input" ]; then
            input="$line"
          else
            input="$input"$'\n'$line
          fi
        elif [ "$mode" = "reading_output" ]; then
          if [ -z "$output" ]; then
            output="$line"
          else
            output="$output"$'\n'$line
          fi
        fi
        ;;
    esac
  done < "$THIS_DIR/test_cases.txt"

  # run last test if any
  if [ -n "$name" ]; then
    run_test "$bin" "$name" "$input" "$output"
    rc=$?
    total=$((total+1))
    if [ $rc -eq 0 ]; then passed=$((passed+1)); else failed=$((failed+1)); fi
  fi

  # Summary for this binary
  if [ "$passed" -eq "$total" ]; then
    printf '%bAll tests passed for %s (%d/%d)%b\n' "$CYA" "$bin" "$passed" "$total" "$RST"
  elif [ "$passed" -gt 0 ]; then
    printf '%bSome tests passed for %s (%d/%d)%b\n' "$YEL" "$bin" "$passed" "$total" "$RST"
  else
    printf '%bAll tests FAILED for %s (%d/%d)%b\n' "$RED" "$bin" "$passed" "$total" "$RST"
  fi
    # Return non-zero if any test failed so caller can reflect overall status
    if [ "$failed" -ne 0 ]; then
      return 1
    fi
    return 0
}

main() {
  failed=0
  bins=( )
  if [ -x "$MINI" ]; then
    bins+=("$MINI")
  else
    echo "Warning: mandatory binary $MINI not found or not executable. Build it first." >&2
  fi
  if [ -x "$BONUS" ]; then
    bins+=("$BONUS")
  fi

  if [ ${#bins[@]} -eq 0 ]; then
    echo "No binaries to test. Exiting." >&2
    exit 2
  fi

  for b in "${bins[@]}"; do
    if ! run_all_for "$b"; then
      failed=1
    fi
  done

  if [ "$failed" -ne 0 ]; then
    echo "Some tests failed." >&2
    exit 3
  fi

  echo "All tests passed."
}

main "$@"
