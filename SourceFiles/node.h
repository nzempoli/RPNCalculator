/* Do NOT Modify this File */
#ifndef NODE_H
#define NODE_H

#include "token.h"

/* This is a general-pupose node for linked lists.
 * The data element here is a Token struct pointer.
 * Both your Stack and Hash Table will use Tokens for data.
 */
typedef struct node_struct {
  Token *tok;
  struct node_struct *next;
} Node;

/* Node Function Prototypes */
Node *node_create(Token *tok);
void node_free(Node *node);

#endif
