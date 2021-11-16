/* Do NOT Modify This File */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "symbol.h"

/* Creates a new symbol.
 * Will initialize val and variable (if not NULL).
 * Returns NULL on any memory errors.
 */
Symbol *symbol_create(char *variable, int value) {
  Symbol *sym = malloc(sizeof(Symbol));
  if(sym == NULL) {
    return NULL;
  }

  sym->next = NULL;
  sym->val = value;
  if(variable != NULL) {
    strncpy(sym->variable, variable, MAX_VAR_LEN);
  }
  return sym;
}

/* Performs a deep copy and returns a copy of the symbol.
 * If symbol is NULL, will return NULL
 */
Symbol *symbol_copy(Symbol *sym) {
  if(sym == NULL) {
    return NULL;
  }

  Symbol *copy = malloc(sizeof(Symbol));
  copy->next = NULL;
  copy->val = sym->val;
  memcpy(copy->variable, sym->variable, MAX_VAR_LEN);
  return copy;
}

/* Frees the symbol.
 */
void symbol_free(Symbol *sym) {
  free(sym);
  sym = NULL;
  return;
}
