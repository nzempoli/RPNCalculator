/* Do NOT Modify this File */
#ifndef SYMBOL_H
#define SYMBOL_H

#define MAX_VAR_LEN 20

/* Symbol Structure
 * This is the entry that is used in the symbol table.
 * variable is the name of the variable
 * val is its current value.
 * next is a pointer to the next Symbol in the Separate Chaining Linked List
 */
typedef struct symbol_struct {
  char variable[MAX_VAR_LEN];
  int val;
  struct symbol_struct *next;
} Symbol;

/* Symbol Table Structure
 * size is the number of current indices that have data in them.
 * capacity is the total number of indices in the table (array)
 * table is an array of Symbol *s, it's an array of node pointers.
 * -- Since it's a pointer to an array of Symbol* types, it's a Symbol**
 */
typedef struct symtab_struct {
  int size;
  int capacity;
  Symbol **table;
} Symtab;

/* Function Prototypes */
Symbol *symbol_create(char *variable, int value);
Symbol *symbol_copy(Symbol *sym);
void symbol_free(Symbol *sym);

#endif
