#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "token.h"

/* These are globals that are restricted to this one file only.
 */
/* Main Buffer (will be eaten by strtok) */
static char *buffer = NULL;
/* Copy Buffer */
static char *cbuf = NULL;
/* Pointers to each Buffer (will follow same jumps) */
static char *p_buf = NULL;
static char *p_cbuf = NULL;

/* Clean any buffer values in use */
static void clean_buffer() {
  if(buffer != NULL) {
    free(buffer);
    buffer = NULL;
    p_buf = NULL;
  }
  if(cbuf != NULL) {
    free(cbuf);
    cbuf = NULL;
    p_cbuf = NULL;
  }
}

/* Initializes the buffers with a new string to parse */
static int assign_buffer(char *string, int size) {
  /* Clear the buffers responsibly first */
  clean_buffer();

  /* Allocate new buffers */
  buffer = malloc(size + 1);
  cbuf = malloc(size + 1);

  if(buffer == NULL || cbuf == NULL) {
    return -1;
  }

  /* Copy the string into the two buffers and ensure proper termination */
  strncpy(buffer, string, size);
  strncpy(cbuf, string, size);
  buffer[size] = '\0';
  cbuf[size] = '\0';

  /* Prime the main buffer for tokenization */
  p_buf = strtok(buffer, " ");
  p_cbuf = cbuf;
  return 0;
}

/* Main interface with other programs.  Passes in a line from the file
 * Returns -1 if string is NULL or is empty.
 * Otherwise, returns 0
 */
int token_read_line(char *string, int size) {
  if(string == NULL || string[0] == '\0') {
    return -1;
  }
  else {
    return assign_buffer(string, size);
  }
}

/* If strtok hasn't returned NULL into p_buf, we have at least one symbol left.
 */
int token_has_next() {
  return (p_buf != NULL);
}

Token *token_create_value(int val) {
  Token *tok = malloc(sizeof(Token));
  if(tok == NULL) {
    return NULL;
  }

  tok->type = TYPE_VALUE;
  tok->value = val;
  return tok;
}

/* If the next symbol represents an operator, assign the codes here. */
static int get_operator(char *p_buf) {
  if(strncmp(p_buf, "+", 1) == 0) {
    return OPERATOR_PLUS;
  }
  else if (strncmp(p_buf, "-", 1) == 0) {
    return OPERATOR_MINUS;
  }
  else if (strncmp(p_buf, "*", 1) == 0) {
    return OPERATOR_MULT;
  }
  else if (strncmp(p_buf, "/", 1) == 0) {
    return OPERATOR_DIV;
  }
  else {
    return -1;
  }
}

/* Returns 1 if the next symbol is print */
static int is_print(char *p_buf) {
  return (strncmp(p_buf, "print", 5) == 0);
}

/* Returns 1 if the next symbol is an operator */
static int is_operator(char *p_buf) {
  return (get_operator(p_buf) != -1);
}

/* Returns 1 if the next symbol is '=' for assignment */
static int is_assignment(char *p_buf) {
  return (p_buf[0] == '=');
}

/* Returns 1 if the next symbol is a value */
static int is_value(char *p_buf) {
  if((p_buf[0] >= '0' && p_buf[0] <= '9') || p_buf[0] == '-') {
    return 1;
  }
  else {
    return 0;
  }
}

/* Internal function to create a new token and perform the assignments */
static Token *create_token() {
  Token *tok = malloc(sizeof(Token));
  if(tok == NULL) {
    return NULL;
  }
  if(is_operator(p_buf)) {
    tok->type = TYPE_OPERATOR;
    tok->oper = get_operator(p_buf);
  }
  else if(is_assignment(p_buf)) {
    tok->type = TYPE_ASSIGNMENT;
  }
  else if(is_print(p_buf)) {
    tok->type = TYPE_PRINT;
  }
  else if(is_value(p_buf)) {
    tok->type = TYPE_VALUE;
    tok->value = strtol(p_buf, NULL, 10);
  }
  else {
    tok->type = TYPE_VARIABLE;
    strncpy(tok->variable, p_buf, MAX_VARIABLE_LEN);
  }
  return tok;
}

/* Creates a token from the next symbol and returns it.
 * Returns NULL if no more symbols
 */
Token *token_get_next() {
  if(token_has_next() == 0) {
    return NULL;
  }

  Token *tok = create_token();
  /* Advance the two buffers */
  p_buf = strtok(NULL, " ");
  p_cbuf = (p_buf-buffer)+cbuf;

  if(p_buf == NULL) {
    free(buffer);
    free(cbuf);
  }

  return tok;
}

/* Prints out the remaining symbols */
void token_print_remaining() {
  printf("|-----Program Remaining\n");
  if(token_has_next()) {
    printf("| %s", p_cbuf);
  }
}

/* Frees a token */
void token_free(Token *tok) {
  free(tok);
  tok = NULL;
}

/* Prints a token */
void token_print(Token *tok) {
  if(tok == NULL) {
    return;
  }
  else if(tok->type == TYPE_OPERATOR) {
    switch(tok->oper) {
      case OPERATOR_PLUS: printf("+ "); break;
      case OPERATOR_MINUS: printf("- "); break;
      case OPERATOR_MULT: printf("* "); break;
      case OPERATOR_DIV: printf("/ "); break;
      default: printf(" "); break;
    }
  }
  else if(tok->type == TYPE_PRINT) {
    printf("print ");
  }
  else if(tok->type == TYPE_ASSIGNMENT) {
    printf("= ");
  }
  else if(tok->type == TYPE_VALUE) {
    printf("%d ", tok->value);
  }
  else {
    printf("%s ", tok->variable);
  }
}
