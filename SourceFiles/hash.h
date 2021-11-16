/* Do NOT Modify this File */
#ifndef HASH_H
#define HASH_H

#include "symbol.h"

#define HASH_TABLE_INITIAL 5

Symtab *hash_initialize();
void hash_destroy(Symtab *symtab);
int hash_get_capacity(Symtab *symtab);
int hash_get_size(Symtab *symtab);
int hash_put(Symtab *symtab, char *var, int val);
Symbol *hash_get(Symtab *symtab, char *var);
void hash_rehash(Symtab *symtab, int new_capacity);
void hash_print_symtab(Symtab *symtab);
long hash_code(char *var);

#endif
