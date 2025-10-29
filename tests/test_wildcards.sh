#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "$0")/.." && pwd)"
BIN="$ROOT/minishell_bonus"

if [ ! -x "$BIN" ]; then
  echo "minishell_bonus not found or not executable"
  exit 2
fi

# Test echo manual*
out=$($BIN debug <<'EOF'
echo manual*
EOF
)
if echo "$out" | grep -q "manual_test"; then
  echo "OK: echo manual* -> manual_test"
else
  echo "FAIL: echo manual* did not expand"
  echo "$out"
  exit 1
fi

# Test ambiguous redirect (capture output even if minishell exits non-zero)
if out=$(printf 'grep a < m*\n' | $BIN debug 2>&1); then
  :
else
  :
fi
if echo "$out" | grep -q "ambiguous redirect"; then
  echo "OK: grep a < m* -> ambiguous redirect"
else
  echo "FAIL: grep a < m* did not report ambiguous redirect"
  echo "$out"
  exit 1
fi

echo "All wildcard tests passed."
