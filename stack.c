/*
 * stack.c
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "stack.h"

/* 
 * Creates a new stack element and pushes it on the front of the stack.
 * Returns a pointer to the newly created element (now head of the stack).
 */
stack_node *push(stack_node *head, int num, int denom)
{
  stack_node *elt;

  // Allocate a new list element with malloc
  elt = malloc(sizeof(*elt));

  // If malloc fails end the program
  if (elt == NULL) {
    printf("Couldn't allocate a new stack node\n");
    exit(1);
  }

  elt->num = num;
  elt->denom = denom;
  elt->next = head;

  // return the pointer to the new head of the stack
  return elt;
}

/*
 * Pops an element from the stack.
 * Returns a pointer to the (new) head of the stack.
 */
stack_node *pop(stack_node *head)
{
  stack_node *new_head;

  if (head == NULL) return head;

  new_head = head->next;

  // Free the space we allocated for the head stack node
  free(head);

  // Return the new head
  return new_head;
}

/*
 * Returns the number of nodes in the stack.
 */
int size(stack_node *head) {
  int counter = 0;
  while (head) {
    counter++;
    head = head->next;
  }
  return counter;
}
