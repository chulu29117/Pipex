#!/bin/bash

PIPEX="./pipex"  # Path to your Pipex executable
BASH="bash"  # Bash shell
TEST_DIR="pipex_test_results"  # Directory to store outputs

# Create test directory
mkdir -p "$TEST_DIR"

# Define test cases: "input_file 'cmd1' 'cmd2' output_file"
TEST_CASES=(
    "input.txt 'ls' 'wc -l' output1.txt"
    "input.txt 'grep Hello' 'cat' output2.txt"
    "input.txt 'invalidCommand' 'wc -l' output3.txt"
    "input.txt '/bin/ls' '/usr/bin/wc -l' output4.txt"
    "input.txt 'echo Test' 'rev' output5.txt"
    "input.txt 'head -n 3' 'tail -n 1' output6.txt"
    "input.txt 'cat nonexistent.txt' 'wc -l' output7.txt"
    "/dev/null 'ls -l' 'wc -l' output8.txt"
)

# Run tests
for test in "${TEST_CASES[@]}"; do
    INPUT=$(echo "$test" | cut -d ' ' -f1)
    CMD1=$(echo "$test" | cut -d ' ' -f2)
    CMD2=$(echo "$test" | cut -d ' ' -f3)
    OUTPUT=$(echo "$test" | cut -d ' ' -f4)

    # Run Bash version
    eval "< $INPUT $CMD1 | $CMD2 > $TEST_DIR/bash_$OUTPUT" 2> "$TEST_DIR/bash_$OUTPUT.stderr"
    BASH_EXIT=$?

    # Run Pipex version
    $PIPEX $INPUT "$CMD1" "$CMD2" "$TEST_DIR/pipex_$OUTPUT" 2> "$TEST_DIR/pipex_$OUTPUT.stderr"
    PIPEX_EXIT=$?

    # Compare outputs
    diff -u "$TEST_DIR/bash_$OUTPUT" "$TEST_DIR/pipex_$OUTPUT" > "$TEST_DIR/diff_$OUTPUT" 2>&1
    DIFF_STDOUT=$?

    diff -u "$TEST_DIR/bash_$OUTPUT.stderr" "$TEST_DIR/pipex_$OUTPUT.stderr" > "$TEST_DIR/diff_$OUTPUT.stderr" 2>&1
    DIFF_STDERR=$?

    # Display results
    echo "🔹 Test: < $INPUT $CMD1 | $CMD2 > $OUTPUT"
    echo "  ✅ Exit Code - Bash: $BASH_EXIT | Pipex: $PIPEX_EXIT"
    if [ "$DIFF_STDOUT" -eq 0 ] && [ "$DIFF_STDERR" -eq 0 ] && [ "$BASH_EXIT" -eq "$PIPEX_EXIT" ]; then
        echo "  ✅ Output Matches ✅"
    else
        echo "  ❌ Differences Found ❌"
        echo "  🔹 Diff stdout: cat $TEST_DIR/diff_$OUTPUT"
        echo "  🔹 Diff stderr: cat $TEST_DIR/diff_$OUTPUT.stderr"
    fi
    echo "------------------------------------"
done
