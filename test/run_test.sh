#!/bin/bash

# Custom shell path
CUSTOM_SHELL="./minishell"  # Your custom shell executable

# Directory containing test cases
TEST_DIR="test/tests"
echo "🍓 🍓 Test 🍓 🍓"

# Loop over each test case
for test_file in "$TEST_DIR"/*.commands; do
    test_name=$(basename "$test_file" .commands)
    # Temporary files to store output
    custom_output=$(mktemp)
    bash_output=$(mktemp)
    echo "🍓 Running test: $test_name"
    
    # Run the commands in the custom shell and capture the output
    $CUSTOM_SHELL < "$test_file" > "$custom_output" 2>&1 &
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