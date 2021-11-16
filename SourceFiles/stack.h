/* Do Not Modify This File */
#ifndef STACK_H
#define STACK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "node.h"

typedef struct stack_head_struct {
  int count;
  Node *top;
} Stack_head;

/* Function Declaration Prototypes */
Stack_head *stack_initialize();
void stack_destroy(Stack_head *head);
int stack_push(Stack_head *stack, Token *tok);
Token *stack_pop(Stack_head *stack);
Token *stack_peek(Stack_head *stack);
int stack_is_empty(Stack_head *stack);
void stack_print(Stack_head *stack);

#endif
