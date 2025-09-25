#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

// Euclidian Algorithm function to calculate gcd
int gcd(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

// Anagram function
int isAnagram(char a[], char b[]) {

	// Return unsuccessful if the lengths of the arrays are not the same
	int len_a = strlen(a);
	int len_b = strlen(b);
	if (len_a != len_b) {
		return 0;
	}
	
	// Store character counts	
	int count_a[26] = {0};
	int count_b[26] = {0};

	// Iterate strings
	for (int i = 0; i < len_a; i++) {
        	count_a[a[i] - 'a']++;
        	count_b[b[i] - 'a']++;
	}

	// Return not an anagram since all counts aren't zero
	for (int i = 0; i < 26; i++) {
		if (count_a[i] != count_b[i]) {
			return 0;
		}
	}

	// Return an anagram
	return 1;
}

// Main Calculator
int main (int argc, char *argv[]) {

	// Exit with error code 1 if minicalc is executed with no arguments
	if (argc < 2) {
		fprintf(stderr, "At least one command-line argument needed.\n");
		exit(1);
	}
	
	// Determine the operation
	char *operation = argv[1];

	// Addition Operation
	if (strcmp(operation, "+") == 0) {

		// Exit with error code 3 if 2 subsequent command-line arguments aren't provided
		if (argc != 4) {
			fprintf(stderr, "Incorrect number of arguments.\n");
			exit(3);
		}
		
		// Convert arguments into integers
		long integer1 = strtol(argv[2], NULL, 10);
		long integer2 = strtol(argv[3], NULL, 10);

		// Exit with error code 4 if argument 1 is not convertible to an integer
		if (integer1 == 0 && strcmp(argv[2], "0") != 0) {
			fprintf(stderr, "Argument 1 is not convertible to an integer.\n");
			exit(4);
		}

		// Exit with error code 4 if argument 2 is not convertible to an integer
		if (integer2 == 0 && strcmp(argv[3], "0") != 0) {
			fprintf(stderr, "Argument 2 is not convertible to an integer.\n");
			exit(4);
		}
		// Print addition result
		printf("%ld\n", integer1 + integer2);
	}

	// Greatest Common Divisor Operation
	else if (strcmp(operation, "gcd") == 0) {

    		// Exit with error code 3 if at least 2 subsequent command-line args not provided
    		if (argc < 4) {
        	fprintf(stderr, "Incorrect number of arguments.\n");
        	exit(3);
    	}

		// Initialize gcd and pointer
    		long calculated_gcd = 0;
    		char *p; 

    		// Check for every argument provided
    		for (int i = 2; i < argc; i++) {
			// Convert arguments to integers
        		long number = strtol(argv[i], &p, 10);

        		// Exit with error code 4 if argument at index i not convertible to integer
        		if (*p != '\0' || argv[i] == p) {
            		fprintf(stderr, "Argument %d is not convertible to an integer.\n", i-1);
            		exit(4);
        		}

        		// Exit with error code 7 if any of the arguments is not positive number
        		if (number <= 0) {
            		fprintf(stderr, "Positive numbers only.\n");
           		exit(7);
        		}

        		// Initialize gcd with the first number
        		if (i == 2) {
            			calculated_gcd = number;
        		} else {
            			// Call to gcd function to calculate
           			 calculated_gcd = gcd(calculated_gcd, number);
        		}
    		}

    		// Print greatest common divisor
    		printf("%ld\n", calculated_gcd);
	}

	// Square Root Operation
	else if (strcmp(operation, "sqrt") == 0) {
		
		// Exit with error code 3 if 1 subsequent command-line argument isn't provided
		if (argc != 3) {
			fprintf(stderr, "Incorrect number of arguments.\n");
			exit(3);
		}

		// Convert argument to a double
		double number = strtod(argv[2], NULL);

		// Exit with error code 4 if argument not convertible to a double
		if (number == 0 && strcmp(argv[2], "0") != 0) {
			fprintf(stderr, "Argument not convertible to a double.\n");
			exit(4);
		}
		// Exit with code 5 if the argument provided is a negative number
		if (number < 0) {
			fprintf(stderr, "Argument cannot be negative for square root.\n");
			exit(5);
		}

                // Print square root
		printf("%.6lf\n", sqrt(number));
        }

	// Anagram Operation
	else if (strcmp(operation, "anagram") == 0) {

		// Exit with error code 3 if 2 subsequent command-line arguments aren't provided
		if (argc != 4) {
			fprintf(stderr, "Incorrect number of arguments.\n");
			exit(3);
		}
		
		// Look through the letters of arguments $3 and $4
		for (int i = 2; i <= 3; i++) {
			char *p = argv[i];
			while (*p) {
				// Exit with error code 6 if there are non-lowercase chars in args
				if (*p < 'a' || *p > 'z') {
					fprintf(stderr, "Arguments can only be in lower case.\n");
					exit(6);
				}
				p++;
			}
		}

                // Print if the arguments are anagrams or not
		if (isAnagram(argv[2], argv[3]))
			printf("true\n");
		else
			printf("false\n");
        }

	else {
		// Exit with error code 2 if the operation isn't +, gcd, sqrt, or anagram
		fprintf(stderr, "Invalid operation. Valid operations: +, gcd, sqrt, anagram.\n");
		exit(2);
	}
}
