#!/bin/bash
# test_pipex.sh
#
# This script runs a series of tests for ./pipex and compares the output and exit code
# with the expected behavior from Bash. For each test, it prints the test number, description,
# the output (STDOUT+STDERR) and the exit code for both pipex and Bash.
# The OK/KO message (colored) is determined by comparing only the exit codes.
# At the end, a summary is printed with the count of OK tests and details of any failed ones.
# Finally, all files created during setup are removed.

# ----------------- Setup: Create Required Files -----------------

# Create infile.txt if it doesn't exist.
if [ ! -f infile.txt ]; then
    echo "This is a sample content for infile.txt" > infile.txt
fi

# Create a simple script.sh (echoes stdin to stdout).
if [ ! -f script.sh ]; then
    echo "#!/bin/bash" > script.sh
    echo "cat" >> script.sh
    chmod +x script.sh
fi

# Create a script with a space in its name.
if [ ! -f "script space.sh" ]; then
    echo "#!/bin/bash" > "script space.sh"
    echo "cat" >> "script space.sh"
    chmod +x "script space.sh"
fi

# Create a script with an escaped quote in its name.
if [ ! -f 'script"quote.sh' ]; then
    echo "#!/bin/bash" > 'script"quote.sh'
    echo "cat" >> 'script"quote.sh'
    chmod +x 'script"quote.sh'
fi

# Create no_exec_cmd1.sh and no_exec_cmd2.sh.
if [ ! -f no_exec_cmd1.sh ]; then
    echo "#!/bin/bash" > no_exec_cmd1.sh
    echo "cat" >> no_exec_cmd1.sh
    chmod +x no_exec_cmd1.sh
fi
if [ ! -f no_exec_cmd2.sh ]; then
    echo "#!/bin/bash" > no_exec_cmd2.sh
    echo "cat" >> no_exec_cmd2.sh
    chmod +x no_exec_cmd2.sh
fi

# Create no_x_script.sh for tests 18 and 19.
if [ ! -f no_x_script.sh ]; then
    echo "#!/bin/bash" > no_x_script.sh
    echo "cat" >> no_x_script.sh
    chmod +x no_x_script.sh
fi

# Create middle_fail.sh that simulates failure in the middle.
if [ ! -f middle_fail.sh ]; then
    echo "#!/bin/bash" > middle_fail.sh
    echo "echo 'This script fails in the middle'" >> middle_fail.sh
    echo "exit 1" >> middle_fail.sh
    chmod +x middle_fail.sh
fi

# Create no_read.txt for input file with no read permissions.
if [ ! -f no_read.txt ]; then
    echo "Content for no_read.txt" > no_read.txt
fi

# Create no_write.txt for output file test.
if [ ! -f no_write.txt ]; then
    echo "Content for no_write.txt" > no_write.txt
fi

# Create a subdirectory "subdir" and a script inside it.
if [ ! -d subdir ]; then
    mkdir subdir
fi
if [ ! -f subdir/script.sh ]; then
    echo "#!/bin/bash" > subdir/script.sh
    echo "cat" >> subdir/script.sh
    chmod +x subdir/script.sh
fi

# ----------------- End Setup -----------------

# ----------------- Cleanup Function -----------------
cleanup() {
    echo -e "\n\e[1;35mCleaning up created files and directories...\e[0m"
    rm -f infile.txt script.sh "script space.sh" 'script"quote.sh' \
          no_exec_cmd1.sh no_exec_cmd2.sh no_x_script.sh middle_fail.sh \
          no_read.txt no_write.txt inexistent.txt no_w_perm outfile.txt
    rm -rf subdir
}
trap cleanup EXIT
# ------------------------------------------------------

# Global counters and arrays.
total_tests=0
ok_tests=0
failed_tests=()

# Function to print a fancy header.
print_header() {
    local title="$1"
    echo -e "\e[1;34m+=====================================+"
    printf "| %-35s |\n" "$title"
    echo -e "+=====================================+\e[0m"
}

# Helper function to run a non-here_doc test.
run_test() {
    test_num=$1
    test_desc="$2"
    shift 2

    ((total_tests++))

    # Build the pipex command (arguments remain as passed).
    pipex_cmd=(./pipex "$@")

    # For bash simulation, assume arguments are: infile, cmd1, cmd2, outfile.
    infile="$1"
    cmd1="$2"
    cmd2="$3"
    outfile="$4"

    # Remove any previous output files.
    rm -f "$outfile" no_file.txt

    print_header "Test $test_num: $test_desc"
    echo -e "\e[1;33mPipex command:\e[0m ${pipex_cmd[*]}"
    
    # Run pipex and capture its output (STDOUT+STDERR) and exit code.
    pipex_output=$( { "${pipex_cmd[@]}"; } 2>&1 )
    pipex_exit=$?
    
    # Now simulate the equivalent bash pipeline:
    #   < infile cmd1 | cmd2 > outfile
    bash_cmd="cat < '$infile' | $cmd1 | $cmd2 > '$outfile'"
    bash_output=$(bash -c "$bash_cmd" 2>&1)
    bash_exit=$?
    
    echo -e "\e[1;36mPipex output/error:\e[0m"
    echo "$pipex_output"
    echo -e "\e[1;36mPipex exit code:\e[0m $pipex_exit"
    echo ""
    echo -e "\e[1;36mBash simulation output/error:\e[0m"
    echo "$bash_output"
    echo -e "\e[1;36mBash simulation exit code:\e[0m $bash_exit"
    
    # Compare exit codes only.
    if [ "$pipex_exit" -eq "$bash_exit" ]; then
        echo -e "\e[1;32m[ OK ] Exit codes match\e[0m"
        ((ok_tests++))
    else
        echo -e "\e[1;31m[ KO ] Exit codes differ\e[0m"
        failed_tests+=("$test_num")
    fi

    echo -e "\e[1;34m+-------------------------------------+\e[0m"
    echo ""
}

# For tests that require a modified PATH, run them via env.
run_test_env() {
    test_num=$1
    test_desc="$2"
    env_setting="$3"  # e.g., PATH=""
    shift 3

    echo -e "\e[1;35mSetting environment: $env_setting\e[0m"
    env $env_setting run_test "$test_num" "$test_desc" "$@"
}

# ----------------- Tests -----------------
print_header "Mandatory Tests"

# Test 1: Normal parameters, simple commands.
run_test 1 "Normal parameters, simple commands, everything should go ok" infile.txt cat wc outfile.txt

# Test 2: Output file does not exist.
run_test 2 "Output file does not exist" infile.txt cat wc inexistent.txt

# Test 3: Commands with extra spaces in the argument.
run_test 3 "Commands with arguments, everything should go ok" infile.txt 'sed    "s/And/But/"' 'grep But' outfile.txt

# Test 4: Argument with spaces inside single quotes (awk argument).
run_test 4 "Argument with spaces in single quotes (awk argument)" infile.txt 'sed "s/And/But/"' "awk '{count++} END {print count}'" outfile.txt

# Test 5: Argument with spaces inside double quotes (awk argument).
run_test 5 "Argument with spaces in double quotes (awk argument)" infile.txt 'sed "s/And/But/"' 'awk "{count++} END {print count}"' outfile.txt

# Test 6: Argument with escaped double quotes and a space inside double quotes (awk argument).
run_test 6 'Argument with escaped double quotes and then a space (\" ,) inside double quotes (awk argument)' infile.txt 'sed "s/And/But/"' 'awk "{count++} END {printf \"count: %i\" , count}"' outfile.txt

# Test 7: Double quotes inside single quotes (awk argument).
run_test 7 'Double quotes inside single quotes (awk argument)' infile.txt 'sed "s/And/But/"' "awk '{count++} END {printf \"count: %i\", count}'" outfile.txt

# Test 8: Command that is in the same folder.
run_test 8 "Command that is in the same folder" infile.txt ./script.sh wc outfile.txt

# Test 9: Script with a space in its name.
run_test 9 "Script in the same folder with a name containing a space" infile.txt "./script space.sh" wc outfile.txt

# Test 10: Command with multiple arguments.
run_test 10 "Command with multiple arguments" infile.txt "cut -d, -f1" "wc -w" outfile.txt

# Test 11: Script with a space in its name (again, alternate quoting).
run_test 11 "Script with a name containing a space" infile.txt "./script space.sh" wc outfile.txt

# Test 12: Command that contains the complete path.
resolved_script=$(realpath script.sh)
run_test 12 "Command that contains the complete path" infile.txt "$resolved_script" wc outfile.txt

# Test 13: Input file does not exist.
run_test 13 "Input file does not exist" no_in cat wc outfile.txt

# Test 14: First command does not exist.
run_test 14 "First command does not exist" infile.txt non_existent_comm wc outfile.txt

# Test 15: Second command does not exist.
run_test 15 "Second command does not exist" infile.txt cat non_existent_comm outfile.txt

# Test 16: Input file does not have read permissions.
run_test 16 "Input file does not have read permissions" no_r_perm cat wc outfile.txt

# Test 17: Output file does not have write permissions.
run_test 17 "Output file does not have write permissions" infile.txt cat wc no_w_perm

# Test 18: First command does not have execution permission.
run_test 18 "The first command does not have execution permission" infile.txt ./no_x_script.sh wc outfile.txt

# Test 19: Second command does not have execution permission.
run_test 19 "The second command does not have execution permission" infile.txt cat ./no_x_script.sh outfile.txt

# Test 20: First command fails in the middle but produces some output.
run_test 20 "The first command fails in the middle of executing, but produces some output" infile.txt ./middle_fail.sh wc outfile.txt

# Test 21: Second command fails in the middle but produces some output.
run_test 21 "The second command fails in the middle of executing, but produces some output" infile.txt cat ./middle_fail.sh outfile.txt

# Test 22: PATH variable is empty, but the scripts are local.
run_test_env 22 "PATH variable is empty, but the scripts are local" "PATH=" infile.txt ./script.sh ./script.sh outfile.txt

# Test 23: PATH variable is empty (normal commands).
run_test_env 23 "PATH variable is empty" "PATH=" infile.txt cat wc outfile.txt

# Test 24: PATH variable does not exist with local scripts.
run_test_env 24 "PATH variable does not exist with local scripts" "PATH=" infile.txt ./script.sh ./script.sh outfile.txt

# Test 25: PATH variable does not exist with normal commands.
run_test_env 25 "PATH variable does not exist with normal commands" "PATH=" infile.txt cat wc outfile.txt

# Test 26: Should not match local script if it does not have a dot before the name.
run_test 26 "Should not match local script if it does not have a dot before the name" infile.txt cat script.sh outfile.txt

# Test 27: Search the command by directory order in PATH (tests_dir first).
# Replace /home/user/tests with your actual tests directory.
run_test_env 27 "Search by directory order in PATH (tests_dir first)" "PATH=/home/user/tests:/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin" infile.txt cat uname outfile.txt

# Test 28: Search the command by directory order in PATH (tests_dir last).
run_test_env 28 "Search by directory order in PATH (tests_dir last)" "PATH=/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin:/home/user/tests" infile.txt cat uname outfile.txt

# Test 29: PATH is shorter and does not have /usr/bin (thus wc is missing).
run_test_env 29 "PATH is shorter and does not have /usr/bin (thus wc missing)" "PATH=/bin" infile.txt cat wc outfile.txt

# Test 30: PATH has '/' at the end of each entry.
run_test_env 30 "PATH has '/' at the end of each entry" "PATH=/usr/local/bin/:/usr/bin/:/bin/:/usr/sbin/:/sbin/" infile.txt cat wc outfile.txt

# Test 31: Execute command in a subdirectory.
run_test 31 "Execute command in a subdirectory" infile.txt subdir/script.sh wc outfile.txt

# Test 32: Input file exists but does not have read permissions.
chmod 000 no_read.txt
run_test 32 "Input file exists but no read permission (should output error 126 or similar)" no_read.txt cat wc outfile.txt
chmod 644 no_read.txt  # Restore permissions after test

# Test 33: Output file exists but does not have write permissions.
touch no_write.txt
chmod 444 no_write.txt
run_test 33 "Output file exists but no write permission (should output error 126 or similar)" infile.txt cat wc no_write.txt
rm -f no_write.txt

# Test 34: First command exists but does not have execution permission.
(
    touch no_exec_cmd1.sh
    chmod 644 no_exec_cmd1.sh
    run_test 34 "First command exists but has no execution permission (should output error 126)" infile.txt ./no_exec_cmd1.sh wc outfile.txt
    rm -f no_exec_cmd1.sh
)

# Test 35: Second command exists but does not have execution permission.
(
    touch no_exec_cmd2.sh
    chmod 644 no_exec_cmd2.sh
    run_test 35 "Second command exists but has no execution permission (should output error 126)" infile.txt cat ./no_exec_cmd2.sh outfile.txt
    rm -f no_exec_cmd2.sh
)

# Test 36: Both commands exist but neither has execution permission.
(
    touch no_exec_cmd1.sh no_exec_cmd2.sh
    chmod 644 no_exec_cmd1.sh no_exec_cmd2.sh
    run_test 36 "Both commands exist but neither has execution permission (should output error 126)" infile.txt ./no_exec_cmd1.sh ./no_exec_cmd2.sh outfile.txt
    rm -f no_exec_cmd1.sh no_exec_cmd2.sh
)

# ----------------- End Tests -----------------

# Summary of test results.
echo -e "\e[1;35m+=====================================+"
echo -e "  Test Summary: $ok_tests out of $total_tests tests passed."
if [ "$ok_tests" -eq "$total_tests" ]; then
    echo -e "  Congratulations! All tests passed."
else
    echo -e "  Failed tests: ${failed_tests[*]}"
fi
echo -e "+=====================================+\e[0m"
