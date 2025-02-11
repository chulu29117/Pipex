#!/bin/bash
# **************************************************************************** #
#                                                                              #
#    test_pipex.sh                                                             #
#                                                                              #
#    A script to run test cases on your bonus version of pipex, comparing      #
#    its output to the equivalent Bash pipeline.                             #
#                                                                              #
#    Usage: ./test_pipex.sh                                                    #
#                                                                              #
# **************************************************************************** #

# Path to your pipex binary (built with "make bonus")
PIPEX=./pipex

# Temporary directory for test files and outputs
TMP_DIR=tmp_tests
mkdir -p "$TMP_DIR"

# run_test runs a test where:
#   $1 is the test name (for identification)
#   $2 is the Bash command (the expected behavior), which must be written as an
#      equivalent pipeline. Use double quotes if needed.
#   The remaining arguments are the arguments passed to pipex.
# It compares the output file produced by the Bash command and by pipex.
run_test() {
    TEST_NAME=$1
    BASH_CMD=$2
    shift 2
    PIPEX_ARGS=("$@")
    OUTFILE="${PIPEX_ARGS[-1]}"
    
    # Remove previous outfile if exists
    rm -f "$OUTFILE"

    echo "Running test: $TEST_NAME"
    BASH_OUT="$TMP_DIR/${TEST_NAME}_bash.out"
    PIPEX_OUT="$TMP_DIR/${TEST_NAME}_pipex.out"

    # Run the Bash equivalent command.
    # Example: < infile cat | wc -l > outfile
    # We replace 'outfile' with our temporary file BASH_OUT.
    eval "$BASH_CMD > \"$BASH_OUT\""

    # Run your pipex command with the given arguments.
    $PIPEX "${PIPEX_ARGS[@]}"

    # Copy the output produced by pipex for comparison.
    cp "$OUTFILE" "$PIPEX_OUT"

    # Compare outputs.
    if diff -q "$BASH_OUT" "$PIPEX_OUT" >/dev/null; then
        echo "[OK] $TEST_NAME passed."
    else
        echo "[FAIL] $TEST_NAME failed."
        echo "Bash output:"
        cat "$BASH_OUT"
        echo "Pipex output:"
        cat "$PIPEX_OUT"
    fi
    echo "-------------------------"
}

echo "=== Testing mandatory functionality ==="

# Test 1: Simple pipeline (count lines)
# Create an input file with 3 lines.
echo -e "Line1\nLine2\nLine3" > "$TMP_DIR/infile.txt"
# The equivalent Bash pipeline is:
#    < infile.txt cat | wc -l > outfile
run_test "SimpleCount" "< \"$TMP_DIR/infile.txt\" cat | wc -l" \
         "$TMP_DIR/infile.txt" "cat" "wc -l" "$TMP_DIR/outfile.txt"

# Test 2: Pipeline with grep and sort.
echo -e "apple\nbanana\napple\ncherry" > "$TMP_DIR/fruits.txt"
# Bash equivalent: < fruits.txt grep apple | sort > outfile
run_test "GrepSort" "< \"$TMP_DIR/fruits.txt\" grep apple | sort" \
         "$TMP_DIR/fruits.txt" "grep apple" "sort" "$TMP_DIR/outfile.txt"

echo "=== Testing error and edge cases ==="

# Test 3: Non-existent input file. (Expect an error message.)
echo "Test: Non-existent infile (should produce an error message)"
$PIPEX "nonexistent.txt" "cat" "wc -l" "$TMP_DIR/outfile.txt"

# Test 4: Invalid command. (Expect an error message.)
echo "Test: Invalid command (should produce an error message)"
$PIPEX "$TMP_DIR/infile.txt" "invalidcmd" "wc -l" "$TMP_DIR/outfile.txt"

echo "=== Testing bonus here_doc functionality ==="

# Test 5: here_doc mode.
# Prepare input that simulates interactive input:
# The file here_input.txt contains lines and ends with the limiter "END".
echo -e "Line1\nLine2\nLine3\nEND" > "$TMP_DIR/here_input.txt"
echo "Running here_doc test (pipex should read from here_doc input)..."
# Pipe the content of here_input.txt to pipex.
# This should mimic: cmd1 << LIMITER | cmd2 > outfile
# For this example, we use "cat" and "wc -l" as our commands.
cat "$TMP_DIR/here_input.txt" | $PIPEX here_doc END "cat" "wc -l" "$TMP_DIR/outfile.txt"
echo "here_doc output:"
cat "$TMP_DIR/outfile.txt"

echo "=== All tests completed ==="
