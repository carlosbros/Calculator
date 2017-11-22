/*
 * main.c
 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stack.h"

#define MAX_LINE_LENGTH 100

// Prints the number at the top of the stack in the format "a/b"
void print_head(stack_node *head) {
	printf("%d/%d\n", head->num, head->denom); 
}

// Removes all leading and trailing whitespace of the given C string
char *trim_whitespace(char *input)
{
  char *end;

  // Remove spaces from the beginning
  while(isspace((unsigned char)*input)) input++;

  if (*input == 0) {
  	return input;
  }

  // Remove spaces from the end
  end = input + strlen(input) - 1;
  while(end > input && isspace((unsigned char)*end)) end--;

  *(end + 1) = 0;

  return input;
}

// Computes the greatest common divisor of two ints
int gcd(int a, int b) {
	if (b == 0) {
		return a;
	}
	return gcd(b, a % b);
}

// Computes the least common multiple of two ints
int lcm(int a, int b) {
	return (a * b) / gcd(a, b);
}

// Gets the next number from the stack (also simplifies it)
stack_node *next_numbers(stack_node *head, int *num1, int *denom1,
				         int *num2, int *denom2) {
	// Store head num and denom and then pop
    *num1 = head->num;
    *denom1 = head->denom;
    head = pop(head);

    // Store second node (new head) num and denom, then pop
    *num2 = head->num;
    *denom2 = head->denom;
    head = pop(head);

    // Simplify terms
    int gcd1 = gcd(*num1, *denom1);
    int gcd2 = gcd(*num2, *denom2);
    *num1 /= gcd1;
    *denom1 /= gcd1;
    *num2 /= gcd2;
    *denom2 /= gcd2;

    // Check for negative denominators (convention)
    if (*denom1 < 0) {
    	*num1 *= -1;
    	*denom1 *= -1;
    }
    if (*denom2 < 0) {
    	*num2 *= -1;
    	*denom2 *= -1;
    }

    return head;
}

// Simplify the fraction res_num / res_denom
void simplify_result(int *res_num, int *res_denom) {
    if (*res_num == 0) {
    	*res_denom = 1;
    } else {
    	int res_gcd = gcd(*res_num, *res_denom);
    	*res_num /= res_gcd;
    	*res_denom /= res_gcd;
    }
    if (*res_denom < 0) {
    	*res_num *= -1;
    	*res_denom *= -1;
    }
}

int main ()
{
  char input[MAX_LINE_LENGTH];
  char *in;
  int number;
  stack_node *head = NULL;

  while (1) {
  	int i = 0;

    // Read input, trim extra whitespace and lowercase all characters
    fgets(input, MAX_LINE_LENGTH, stdin);
    while (input[i]) {
    	input[i] = tolower(input[i]);
    	i++;
    }
    in = trim_whitespace(input);

    // Check for an integer
    if (sscanf(in, "%d", &number) == 1) {
    	// Check for extra invalid chars after the integer
    	int num_digits = 0;
    	int tmp = number;
    	while (tmp != 0) {
    		num_digits++;
    		tmp /= 10;
    	}
    	if (number <= 0) {
    		num_digits++;
    	}
    	if (strlen(in) != num_digits) {
    		printf("INVALID COMMAND\n");
    		continue;
    	}

    	// Push the integer onto the stack, format "number / 1"
    	head = push(head, number, 1);
    	print_head(head);
    }

    // Check for dup
    else if (strcmp(in, "dup") == 0) {
    	if (size(head) < 1) {
    		printf("STACK ERROR\n");
    		continue;
   		}

   		// Push head value to duplicate head of the stack
    	head = push(head, head->num, head->denom);
   		print_head(head);
   	}

    // Check for swap
   	else if (strcmp(in, "swap") == 0) {
    	if (size(head) < 2) {
    		printf("STACK ERROR\n");
    		continue;
   		}
   		int num1, denom1, num2, denom2;
    		
    	// Get the next numbers from the stack
    	head = next_numbers(head, &num1, &denom1, &num2, &denom2);

    	// Push in reverse order to swap
    	head = push(head, num1, denom1);
    	head = push(head, num2, denom2);
    	print_head(head);
    }

   	// Check for quit
    else if (strcmp(in, "quit") == 0) {
   		while (head) {
   			head = pop(head);
      	}
      	exit(0);
   	}

    // Check for +
   	else if (strcmp(in, "+") == 0) {
   		int num1, denom1, num2, denom2;

    	if (size(head) < 2) {
    		printf("STACK ERROR\n");
    		continue;
   		}

   		// Get the next numbers from the stack
    	head = next_numbers(head, &num1, &denom1, &num2, &denom2);

    	// Combine fractions and add (+)
    	int res_denom = lcm(denom1, denom2);
    	int res_num = (num1 * (res_denom / denom1)) +
    	              (num2 * (res_denom / denom2));
    	
    	// Simplify result
    	simplify_result(&res_num, &res_denom);

    	// Push result and print new head
    	head = push(head, res_num, res_denom);
    	print_head(head);
    }

    // Check for -
    else if (strcmp(in, "-") == 0) {
    	int num1, denom1, num2, denom2;

    	if (size(head) < 2) {
    		printf("STACK ERROR\n");
    		continue;
    	}

      	// Get the next numbers from the stack
    	head = next_numbers(head, &num1, &denom1, &num2, &denom2);

    	// Combine fractions and subtract (-)
    	int res_denom = lcm(denom1, denom2);
    	int res_num = (num2 * (res_denom / denom2)) -
    	              (num1 * (res_denom / denom1));
    	
    	// Simplify result
    	simplify_result(&res_num, &res_denom);

    	// Push result and print new head
    	head = push(head, res_num, res_denom);
    	print_head(head);
    }

    // Check for *
    else if (strcmp(in, "*") == 0) {
    	int num1, denom1, num2, denom2;

    	if (size(head) < 2) {
    		printf("STACK ERROR\n");
    		continue;
    	}

      	// Get the next numbers from the stack
    	head = next_numbers(head, &num1, &denom1, &num2, &denom2);

    	// Multiply (*)
    	int res_num = num1 * num2;
    	int res_denom = denom1 * denom2;
    	
    	// Simplify result
    	simplify_result(&res_num, &res_denom);

    	// Push result and print new head
    	head = push(head, res_num, res_denom);
    	print_head(head);
    }
    
    // Check for /
    else if (strcmp(in, "/") == 0) {
    	int num1, denom1, num2, denom2;

    	if (size(head) < 2) {
    		printf("STACK ERROR\n");
    		continue;
    	}

      	// Get the next numbers from the stack
    	head = next_numbers(head, &num1, &denom1, &num2, &denom2);

    	// Divide (/)
    	int res_num = num1 * denom2;
    	int res_denom = denom1 * num2;
    	
    	// End program if division by zero occurs
    	if (res_denom == 0) {
    		printf("DIVIDE BY 0 ERROR\n");
    		while (head) {
    			head = pop(head);
    		}
    		exit(0);
    	}

    	// Simplify result
    	simplify_result(&res_num, &res_denom);

    	// Push result and print new head
    	head = push(head, res_num, res_denom);
    	print_head(head);
	}

    // Anything else is invalid
    else {
    	printf("INVALID COMMAND\n");
    }
  }
  
  return 0;
}
