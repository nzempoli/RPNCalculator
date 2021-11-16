/* Do NOT Modify This File */
#ifndef TOKEN_H
#define TOKEN_H

#define TYPE_ASSIGNMENT 0
#define TYPE_OPERATOR   1
#define TYPE_VARIABLE   2
#define TYPE_VALUE      3
#define TYPE_PRINT      4

#define OPERATOR_PLUS   0
#define OPERATOR_MINUS  1
#define OPERATOR_MULT   2
#define OPERATOR_DIV    3

#define MAX_VARIABLE_LEN 20

/* Struct definition for Tokens */
typedef struct token_struct {
  int type;
  int oper;
  char variable [MAX_VARIABLE_LEN];
  int value;
} Token;

/* Token Related Prototypes */
int token_read_line(char *string, int size);
int token_has_next();
Token *token_create_value(int val);
Token *token_get_next();
void token_print_remaining();
void token_print(Token *token);
void token_free(Token *token);

#endif
