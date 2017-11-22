/*
 * stack.h : Declares functions used to implement a simple stack
 */

typedef struct stack_node {
  int num;
  int denom;  
  struct stack_node *next; // Pointer to the next element in the stack
} stack_node;

stack_node *push(stack_node *head, int num, int denom);
stack_node *pop(stack_node *head);
int size(stack_node *head);