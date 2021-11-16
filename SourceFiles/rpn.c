/* Fill in your Name and GNumber in the following two comment fields
 * Name: Nicholas Zempolich
 * GNumber:G00702946
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stack.h"
#include "token.h"
#include "hash.h"

/* Local Function Declarations (that you need to finish implementing) */
static int read_file(char *filename, char *line);
static int parse_token(Symtab *symtab, Stack_head *stack, Token *tok);
int operation(Symtab *symtab,Stack_head *stack,Token *tok);


/* Pre-Written Local Function Declarations */
static void print_header(char *filename, int step);
static void print_step_header(int step);
static void print_step_footer(Symtab *symtab, Stack_head *stack);
static void print_step_output(int val);

/* Defines the largest line that can be read from a file */
#define MAX_LINE_LEN 255

/* This has been written for you.
 * Main function to run your program (written for you).
 * 1) Opens the file using the passed in filename. (your function)
 * -- If the file is not found (ie. fopen returns NULL), then exit(-1);
 * 2) Reads one line from the file.
 * -- The programs will all only be one line in size.
 * -- That line may be up to MAX_LINE_LEN long.
 * 3) Closes the file.
 * 4) Calls token_read_line(line, strlen(line))
 * -- This parses the line and prepares the tokens to be ready to get.
 * 5) While there are tokens remaining to parse: token_has_next() != 0
 * -- a) Get the next token: token_get_next()
 * 6) Parse the token (your function)
 * 7) Print out all of the relevant information
 */
int rpn(Stack_head *stack, Symtab *symtab, char *filename) {
  int step = 0; /* Used to track the program steps */
  int ret = 0;
  char line[MAX_LINE_LEN];
  Token *tok = NULL;

  /* Complete the read_file function that is defined later in this file. */
  ret = read_file(filename, line);
  if(ret != 0) {
    printf("Error: Cannot Read File %s.  Exiting\n", filename);
    exit(-1);
  }

  /* Pass the line into the tokenizer to initialize that system */
  token_read_line(line, strlen(line));

  /* Prints out the nice program output header */
  print_header(filename, step);

  /* Iterate through all tokens */
  while(token_has_next()) {
    /* Begin the next step of execution and print out the step header */
    step++; /* Begin the next step of execution */
    print_step_header(step);

    /* Get the next token */
    tok = token_get_next();
    /* Complete the implementation of this function later in this file. */
    ret = parse_token(symtab, stack, tok);
    if(ret != 0) {
      printf("Critical Error in Parsing.  Exiting Program!\n");
      exit(-1);
    }

    /* Prints out the end of step information */
    print_step_footer(symtab, stack);
  }

  return 0;
}

/* (IMPLEMENT THIS FUNCTION)
 * Local function to open a file or exit.
 * Follow the Directions in the Project Documentation for this Function
 * Open filename, read its contents (up to MAX_LINE_LEN) into line, then
 *   close the file and return 0.
 * On any file error, return -1.
 */
static int read_file(char *filename, char *line) {
  FILE *fp= fopen(filename,"r");//Opens file in read mode
  if (fp==NULL) { //if there were issues opening the file return -1;
    return -1;
  }
  if (fgets(line,MAX_LINE_LEN,fp)==NULL){//if we couldn't read a line quit
     fclose(fp);
    return -1;
  }
  fclose(fp);//successfully read, return 0
  return 0;
}

/* (IMPLEMENT THIS FUNCTION)
 * Parses the Token to implement the rpn calculator features
 * Follow the Directions in the Project Documentation for this Function
 * You may implement this how you like, but many small functions would be good!
 * If the token you are passed in is NULL, return -1.
 * If there are any memory errors, return -1.
 */
static int parse_token(Symtab *symtab, Stack_head *stack, Token *tok) {
  int result=0;
  int output=0;
  Token *tok1=NULL;
  Token *tok2=NULL;
  Symbol *sym=NULL;
  if ((tok->type==3)||(tok->type==2)) {//If the token type is variable or value add to stack
    if (stack_push(stack,tok)==-1){ //If the push failed return -1;
      return -1;
    }
  }
  if (tok->type==1) {//If type is operation call operation function
    result=operation(symtab,stack,tok);
    if (result==-1) { //If the operation failed return -1
      return -1;
    }
  }else if (tok->type==0){//If the token type is equals
    tok2=stack_pop(stack);//Get the value
    tok1=stack_pop(stack);//Get the variable
    hash_put(symtab,tok1->variable,tok2->value);//Add the variable to the hashtable
    free(tok1); 
    free(tok2);
    free(tok);
  }else if (tok->type==4){//If the token type is print
    tok1=stack_pop(stack);//Get the token we need to print
    if (tok1->type==2) {//If the token is a variable go through actions to get value
      sym=hash_get(symtab,tok1->variable);
      output=sym->val;
      symbol_free(sym);
    }else{
      output=tok1->value;//If the token is a value just get value
    }
    print_step_output(output);//Print out the value of the token
    free(tok1);
    free(tok);
  }
  return 0;
}

/**
 * Name: operation
 * Description: This function will take take a math operation
 *              specified in tok, and applies it to the top two
 *              values in stack and adds the result back to the stack.
 *              Returns 1 on success, -1 on failure.
 * Parameters:
 *   Symtab *symtab - Symbol Table that contains existing symbols
 *   Stack_head *stack - The current stack for the program
 *   Token *tok - Token that contains the operation.
 *   
 */
int operation(Symtab *symtab,Stack_head *stack,Token *tok) {
  int result=0;
  if (stack->count<2) {//If the stack is too small return an error
   return -1;
  }
  Token *tok2=stack_pop(stack);
  Token *tok1=stack_pop(stack);
  Symbol *s1=NULL;
  Symbol *s2=NULL;
  int first=tok1->value; //Sets first to the base token->value (will only stay if token is a value)
  int second=tok2->value;//Sets second to the base token->value
  if (tok1->type==2) {//If the token is a variable, get it's value from symbol table
    s1=hash_get(symtab,tok1->variable);
    if (s1==NULL) {
      return -1;
    }
    first=s1->val;
    symbol_free(s1);
  }
  if (tok2->type==2){//If the token is a variable get it's value from the symbol table
   s2=hash_get(symtab,tok2->variable);
   if (s2==NULL) {
     return -1;
   }
   second=s2->val;
   symbol_free(s2);  
  }
  if (tok->oper==0){//Add operation
    result=first+second;   
  }else if (tok->oper==1) {//Minus operation
    result=first-second;
  }else if (tok->oper==2){// Multiply operation
    result=first*second;
  }else if (tok->oper==3) {//divide operation
    result=first/second;
  }
  free(tok1);
  free(tok2);
  free(tok);
  Token *new=token_create_value(result);
  if (new==NULL) { //If we couldn't create a new token return error
    return -1;
  }
  stack_push(stack,new);
  return 0;
}





/* This has been written for you.
 * Prints out the main output header
 */
static void print_header(char *filename, int step) {
  printf("######### Beginning Program (%s) ###########\n", filename);
  printf("\n.-------------------\n");
  printf("| Program Step = %2d\n", step);
  token_print_remaining();
  printf("o-------------------\n");
}

/* This has been written for you.
 * Prints out the information at the top of each step
 */
static void print_step_header(int step) {
  printf("\n.-------------------\n");
  printf("| Program Step = %2d\n", step++);
}

/* This has been written for you.
 * Prints out the output value (print token) nicely
 */
static void print_step_output(int val) {
  printf("|-----Program Output\n");
  printf("| %d\n", val);
}

/* This has been written for you.
 * Prints out the information at the bottom of each step
 */
static void print_step_footer(Symtab *symtab, Stack_head *stack) {
  hash_print_symtab(symtab);
  stack_print(stack);
  token_print_remaining();
  printf("o-------------------\n");
}
