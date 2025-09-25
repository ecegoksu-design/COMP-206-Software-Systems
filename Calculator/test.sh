#!/bin/bash

minicalc_test() {
	
	# No argument case
	if [ "$1" == "NO_ARGS" ]
       	then
        	operation=""
        	expected_exit_code=$2
        	expected_output=$3
        	output=$(./minicalc 2>&1)

	else	
		# Extract operation from first argument
		operation=$1
    		# Extracts expected exit code from second argument
    		expected_exit_code=$2
    		# Extracts expected output from third argument
		expected_output=$3
    		# Extract arguments from onwards
    		args="${@:4}"
	
    		# Run the executable file minicalc and get the actual output and exit code 
    		output=$(./minicalc "$operation" $args 2>&1)
	fi

    	exit_code=$?

    	# Compare the expected and real exit codes
    	if [ $exit_code -eq $expected_exit_code ]
    	then
        	echo "Success! Correct exit code."
    	else
        	echo "Incorrect exit code."
    	fi

    	# Echo real and expected output
    	echo "Expected output: $expected_output"	
    	echo "Real output: $output"
}

# Exit code 1 - no argument
minicalc_test "NO_ARGS" 1 "At least one command-line argument needed."

# Exit code 2 - invalid operation
minicalc_test "gad" 2 "Invalid operation. Valid operations: +, gcd, sqrt, anagram."

# Exit code 3 - addition 
minicalc_test "+" 3 "Incorrect number of arguments." 5

# Exit code 4 - addition
minicalc_test "+" 4 "Argument 1 is not convertible to an integer." "something" 2

# Working - addition
minicalc_test "+" 0 "5" 2 3

# Exit code 3 - gcd
minicalc_test "gcd" 3 "Incorrect number of arguments."

# Exit code 4 - gcd
minicalc_test "gcd" 4 "Argument 1 is not convertible to an integer." "something" 2

# Exit code 7 - gcd
minicalc_test "gcd" 7 "Positive numbers only." -1 5

# Working - gcd 
minicalc_test "gcd" 0 "3" 3 9

# Exit code 3 - sqrt
minicalc_test "sqrt" 3 "Incorrect number of arguments."

# Exit code 4 - sqrt
minicalc_test "sqrt" 4 "Argument not convertible to a double." "something"

# Exit code 5 - sqrt
minicalc_test "sqrt" 5 "Argument cannot be negative for square root." -4

# Working - sqrt
minicalc_test "sqrt" 0 "3.000000" 9

# Exit code 3 - anagram
minicalc_test "anagram" 3 "Incorrect number of arguments."

# Exit code 6 - anagram
minicalc_test "anagram" 6 "Arguments can only be in lower case." "Diary" "dAiry"

# Working - anagram
minicalc_test "anagram" 0 "true" "diary" "dairy"
