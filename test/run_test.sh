#!/bin/bash

#!/bin/bash

# Determine script location and set paths relative to it
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# Custom shell path (project root /minishell)
CUSTOM_SHELL="$SCRIPT_DIR/../minishell_bonus"

# Directory containing test cases (project_root/tests)
TEST_DIR="$SCRIPT_DIR/tests"

echo "$TEST_DIR"
echo "Using custom shell: $CUSTOM_SHELL"

echo "🍓 🍓 Test 🍓 🍓"

# check test dir
if [ ! -d "$TEST_DIR" ]; then
    echo "Test directory not found: $TEST_DIR"
    exit 1
fi

shopt -s nullglob
tests=("$TEST_DIR"/*.commands)
if [ ${#tests[@]} -eq 0 ]; then
    echo "No .commands test files in $TEST_DIR"
    exit 0
fi

# Loop over each test case
for test_file in "${tests[@]}"; do
    test_name=$(basename "$test_file" .commands)
    # Temporary files to store output
    custom_output=$(mktemp)
    bash_output=$(mktemp)
    echo "🍓 Running test: $test_name"
    
    # Run the commands in the custom shell and capture the output
    "$CUSTOM_SHELL" < "$test_file" > "$custom_output" 2>&1 &
    # Run the commands in bash and capture the output
    bash < "$test_file" > "$bash_output" 2>&1
    
    # Wait for the custom shell to finish
    #wait
    sleep 1

    # Compare outputs
    if diff "$custom_output" "$bash_output" > /dev/null; then
        echo "$test_name: PASS 💚"
    else
        echo "$test_name: FAIL 🔴"
        echo "------------- Bash Output:"
        cat "$bash_output"
        echo "------------- Custom Shell Output:"
        cat "$custom_output"
    fi
    # Clean up temporary files
    rm -f "$custom_output" "$bash_output"
    echo "test done: $test_name"
done