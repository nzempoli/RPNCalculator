/* Do NOT Modify This File */
#include <stdio.h>
#include <stdlib.h>

#include "token.h"
#include "node.h"

/* Create a new Node that will contain the Token */
Node *node_create(Token *tok) {
  Node *node = malloc(sizeof(Node));
  if(node == NULL) {
    return NULL;
  }

  node->next = NULL;
  node->tok = tok;
  return node;
}

/* Will not free the Token inside the node, only the Node itself */
void node_free(Node *node) {
  if(node == NULL) {
    return;
  }

  free(node);
  node = NULL;
}
