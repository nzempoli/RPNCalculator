/* Fill in your Name and GNumber in the following two comment fields
 * Name: Nicholas Zempolich
 * GNumber: G00702946
 */
#include <stdio.h>
#include <stdlib.h>

#include "node.h"
#include "stack.h"

/* (IMPLEMENT THIS FUNCTION)
 * Create a new Stack_head struct on the Heap and return a pointer to it.
 * Follow the Directions in the Project Documentation for this Function
 * On any malloc errors, return NULL
 */
Stack_head *stack_initialize() {
  Stack_head *entry=malloc(sizeof(Stack_head));  
  if (entry==NULL) { //If head couldn't be initialized set it to null
    return NULL;
  }
  entry->count=0;
  entry->top=NULL;
  return entry;
}

/* (IMPLEMENT THIS FUNCTION)
 * Destroy a stack.
 * Follow the Directions in the Project Documentation for this Function
 * If head is NULL, you can just return.
 */
void stack_destroy(Stack_head *head) {
  if (head==NULL){ //If head is null return
    return;
  }
  Node *walker=head->top;
  Node *current=head->top;
  free(head);
  while (walker!=NULL) { //Walks through each node in the stack, and destroyes it
    current=walker;
    token_free(walker->tok);
    walker=walker->next;
    node_free(current);
  }
}

/* (IMPLEMENT THIS FUNCTION)
 * Push a new Token on to the Stack.
 * Follow the Directions in the Project Documentation for this Function
 * On any malloc errors, return -1.
 * If there are no errors, return 0.
 */
int stack_push(Stack_head *stack, Token *tok) {
  if ((stack==NULL)||(tok==NULL)) { //If given an empty token or stack quit
    return -1;
  }
  Node *new=node_create(tok);
  if (new==NULL) {
    return -1;
  }
  new->next=stack->top;//Add node to the top and increase count
  stack->top=new;
  stack->count=stack->count+1;
  return 0;
}

/* (IMPLEMENT THIS FUNCTION)
 * Pop a Token off of the Stack.
 * Follow the Directions in the Project Documentation for this Function
 * If the stack was empty, return NULL.
 */
Token *stack_pop(Stack_head *stack) {
  if ((stack==NULL)||(stack->count==0)) {//If stack is empty return NULL
    return NULL;
  }
  Node *toRemove=stack->top;
  Token *tok=stack->top->tok;
  if (stack->count==1) { //Sets top to empty if we removed the only entry
    stack->top=NULL;
  }else{
    stack->top=stack->top->next;//Otherwise we set the top to what used to be tops next
  }
  stack->count=stack->count-1;
  node_free(toRemove);  
  return tok;
}

/* (IMPLEMENT THIS FUNCTION)
 * Return the token in the stack node on the top of the stack
 * Follow the Directions in the Project Documentation for this Function
 * If the stack is NULL, return NULL.
 * If the stack is empty, return NULL.
 */
Token *stack_peek(Stack_head *stack) {
  if (stack_is_empty(stack)!=0){//If stack is empty it will return NULL 
    return NULL;
  }
  return stack->top->tok;
}

/* (IMPLEMENT THIS FUNCTION)
 * Return the number of nodes in the stack.
 * Follow the Directions in the Project Documentation for this Function
 * If stack is NULL, return -1.
 * Return 1 if the stack is empty or 0 otherwise.
 */
int stack_is_empty(Stack_head *stack) {//
  if (stack==NULL) {//If null return -1
    return -1;
  }else if (stack->count==0){//if stack is empty return 1
    return 1;
  }
  return 0;
}

/* These two functions are written for you.
 * It recurses the stack and prints out the tokens in reverse order
 * eg. top->2->4->1->8 will print at Stack: 8 1 4 2
 * eg. stack_push(5) will then print Stack: 8 1 4 2 5
 */

/* This is implemented for you.
 * Recursive print. (Local function)
 * Base Case: node == NULL, return
 * Recursive Case: call print_node(node->next, print_data), then print node.
 */
static void print_node(Node *node) {
  if(node == NULL) {
    return;
  }
  token_print(node->tok);
  print_node(node->next);
  return;
}

/* This is implemented for you.
 * Setup function for the recursive calls.  Starts printing with stack->top
 */
void stack_print(Stack_head *stack) {
  if(stack == NULL) {
    return;
  }
  printf("|-----Program Stack\n");
  printf("| ");
  print_node(stack->top);
  printf("\n");
  return;
}
