/* Fill in your Name and GNumber in the following two comment fields
 * Name:Nicholas Zempolich
 * GNumber: G00702946
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "node.h"
#include "hash.h"

int insertSymbol(Symtab *symtab, char *var, int val, int placement);
Symbol *findValue(Symtab *symtab,char *var, int placement, int *found);

/* (IMPLEMENT THIS FUNCTION)
 * In this function, you will create a new Symtab struct.
 * Follow the Directions in the Project Documentation for this Function
 * Return the pointer to the new symtab.
 * On any memory errors, return NULL
 */
Symtab *hash_initialize() {
  Symtab *new=malloc(sizeof(Symtab));
  if (new==NULL) {//If Symtab couldn't be initialized return NULL
    return NULL;
  }
  new->size=0;
  new->capacity=HASH_TABLE_INITIAL;
  new->table=malloc(new->capacity * sizeof(Symbol*));
  if (new->table==NULL) {//If table couldn't be initalized quit
    return NULL;
  }
  for (int i=0;i<(new->capacity);i++) {//Set each index of the array to NULL
    new->table[i]=NULL;
  }
  return new;
}

/* (IMPLEMENT THIS FUNCTION)
 * Destroy your Symbol Table.
 * Follow the Directions in the Project Documentation for this Function
 * Return on any memory errors.
 */
void hash_destroy(Symtab *symtab) {
  if (symtab==NULL) {//If Symtab is NULL quit
    return;
  }
  Symbol *walkSym=NULL;
  Symbol *holder=NULL;
  if (symtab->table==NULL) {
    free(symtab);
    return;
  }
  for (int i=0;i<hash_get_capacity(symtab);i++) {//Walk through each index of the table
    walkSym=symtab->table[i]; //Set up a walker node so we can walk through each node in a list
    while (walkSym!=NULL) {//While the walker is not null, move to next walker, and release the previous one. 
      holder=walkSym;
      walkSym=walkSym->next;
      holder->next=NULL;
      symbol_free(holder);
    }
  }
  free(symtab->table);
  free(symtab);
}

/* (IMPLEMENT THIS FUNCTION)
 * Return the capacity of the table inside of symtab.
 * If symtab is NULL, return -1;
 */
int hash_get_capacity(Symtab *symtab) {
  if (symtab==NULL) {
    return -1;
  }
  return symtab->capacity;
}

/* (IMPLEMENT THIS FUNCTION)
 * Return the number of used indexes in the table (size) inside of symtab.
 * If symtab is NULL, return -1;
 */
int hash_get_size(Symtab *symtab) {
  if (symtab==NULL) {
    return -1;
  }
  return symtab->size;
}


/*
 * Returns the load of the symbol table as a double.
 */
double get_load(Symtab *symtab) {
  double size=hash_get_size(symtab);
  double capacity=hash_get_capacity(symtab);
  return size/capacity;
}

/* (IMPLEMENT THIS FUNCTION)
 * Adds a new Symbol to the symtab via Hashing.
 * Follow the Directions in the Project Documentation for this Function
 * If symtab is NULL, there are any malloc errors, or if any rehash fails, return -1;
 * Otherwise, return 0;
 */
int hash_put(Symtab *symtab, char *var, int val) {
  int result=0;
  if (symtab==NULL) {//If symtab is null return
    return -1;
  }
  int placement=0;
  if ((get_load(symtab))>=2.0){//If the load is greater than 2(get load returns double) rehash
    hash_rehash(symtab,symtab->capacity*2);
  }
  if (symtab->table==NULL) {//If rehash fails quit
    return -1;
  }
  placement=(hash_code(var))%(hash_get_capacity(symtab));
  result = insertSymbol(symtab,var,val,placement);//place the variable in the table
  if (result==-1) {
    return -1;
  }
    return 0;
}


/**
 *Name: insertSymbol
 *Description:  Function that will either update a symbol with the same name
                as var with the value of val. Or generate a new symbol
 *              and add it to the table. Returns -1 if it failed, 0 if success.
 *Paramaters:
 *      symtab - Symbol table the new symbol will be put into
 *      var - The variable name
 *      val - The value for the symbol
 *      placement - index where the symbol will be placed.
 */
int insertSymbol(Symtab *symtab,char *var,int val,int placement) {
  int found=0;
  Symbol *holder=findValue(symtab,var,placement,&found); //Checks if the variable is already in the table
  if (found==1) {//If it is in the table update the existing symbol
    holder->val=val;
  }else{//Else add a new symbol to the table.
    Symbol *new=symbol_create(var,val);
    if (new==NULL) {
      return -1;
    }
    if ((symtab->table)[placement]==NULL) {//If the index in the table is null
      (symtab->table)[placement]=new; //Set the head of that list to the symbol
    }else{    
      holder->next=new; //Else add the symbol to the end of list
    }
    symtab->size=symtab->size+1;
  }
  return 0;
}

/**
 *Name: findValue
 *Description: Takes in a variable and checks to see if it's already in the
 *             table. If it is in the table found will be set to 1, and a
 *             pointer to the symbol will be returned. If it is not already 
 *             in the table found will be set to 0. The symbol pointer returned
 *             will either be the pointer to the index (if no symbols are 
 *             already specified) or the node at the end of the linked list
 *             at that index.
 *Parameters:
 *             symtab - Symbol table
 *             var - name of the variable
 *             placement - index where the symbol should be placed
 *             found     - pointer to found that will be updated  
 */
//Checks to see if the table has a specific value already stored.
Symbol *findValue(Symtab *symtab,char *var, int placement,int *found) {
  Symbol *walker=(symtab->table)[placement]; //Sets walker to beginning of list,If the list is NULL this will be returned. 
  if ((walker!=NULL)&&(strncmp(var,walker->variable,12)==0)) { 
    *found=1; //Checks the first Symbol at index placement, and if it matches var returns 1.
  }
  if (walker!=NULL) { //If walker's not null check each Symbol and if any match set found to 1, and return the found symbol.
    while (((walker->next)!=NULL)&&(*found!=1)) {
      walker=walker->next;
      if ((strncmp(var,walker->variable,12)==0)) {
        *found=1;
      }
    }
  }
  return walker;
}




/* (IMPLEMENT THIS FUNCTION)
 * Gets the Symbol for a variable in the Hash Table.
 * Follow the Directions in the Project Documentation for this Function
 * On any NULL symtab or memory errors, return NULL
 */
Symbol *hash_get(Symtab *symtab, char *var) {
  int placement=hash_code(var)%symtab->capacity;
  int found=0;
  if (symtab==NULL) { //If symtab is null quit
    return NULL;
  }
  Symbol *symb=findValue(symtab,var,placement,&found);//checks if the symbol is already in the table
  if (found==0) { //If it was not quit
    return NULL;
  }
  Symbol *copy=symbol_copy(symb); //If it was found generate a copy
  if (copy==NULL) { //If copy failed return.
    return NULL;
  }
  return copy;
}

/* (IMPLEMENT THIS FUNCTION)
 * Doubles the size of the Array in symtab and rehashes.
 * Follow the Directions in the Project Documentation for this Function
 * If there were any memory errors, set symtab->array to NULL
 * If symtab is NULL, return immediately.
 */
void hash_rehash(Symtab *symtab, int new_capacity) {
  int oldCapacity=0;
  if (symtab==NULL) {//If symtab is null return
    return;
  }
  
  Symbol **newtable=malloc(sizeof(Symbol *) * new_capacity);
  if (newtable==NULL) {//If a newtable couldn't be initialized quit
    return;
  } 
  Symbol **oldtable=symtab->table;
  symtab->table=newtable;
  for (int j=0;j<new_capacity;j++) {//Set each index of new table to null
    newtable[j]=NULL;
  }
  int hash=0;
  Symbol *walker=NULL;
  Symbol *oldSymbol=NULL;
  symtab->size=0; //Sets symtab size to 0 for the rebuild
  oldCapacity=symtab->capacity; //Stores old capacity
  symtab->capacity=new_capacity;//Updates symtab to use new capaicty
  for (int i=0;i<oldCapacity;i++){//Walk through each index in the old table
    walker=oldtable[i];
    while (walker!=NULL) {//While we're walking through each node in a list at index i
      hash=hash_code(walker->variable)%new_capacity;
      if(insertSymbol(symtab,walker->variable,walker->val,hash)==-1) {//Attempt to insert symbol from the old table into the new one.
        free(symtab->table);
        symtab->table=NULL; //Return Null if failed.
        return;
      }  
      oldSymbol=walker;
      walker=walker->next;
      symbol_free(oldSymbol);//free the copied symbol
    }
  }
  free(oldtable);//Free the original table.
  return;
}

/* Implemented for you.
 * Provided function to print the symbol table */
void hash_print_symtab(Symtab *symtab) {
  if(symtab == NULL) {
    return;
  }
  printf("|-----Symbol Table [%d size/%d cap]\n", symtab->size, symtab->capacity);

  int i = 0;
  Symbol *walker = NULL;

  /* Iterate every index, looking for symbols to print */
  for(i = 0; i < symtab->capacity; i++) {
    walker = symtab->table[i];
    /* For each found linked list, print every symbol therein */
    while(walker != NULL) {
      printf("| %10s: %d \n", walker->variable, walker->val);
      walker = walker->next;
    }
  }
  return;
}

/* This function is written for you.
 * This computes the hash function for a String
 */
long hash_code(char *var) {
  long code = 0;
  int i;
  int size = strlen(var);

  for(i = 0; i < size; i++) {
    code = (code + var[i]);
    if(size == 1 || i < (size - 1)) {
      code *= 128;
    }
  }

  return code;
}
