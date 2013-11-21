#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "dbg.h"

#include "Stack.h"
#include "TokenList.h"

#define OP_ADD 0
#define OP_SUB 1
#define OP_MUL 2
#define OP_DIV 3

double RPN_getOperand(LinkedStack * stack)
{
    double * v = LinkedStack_pop(stack);
    CHECK_DEBUG(v != NULL, "Failed to pop number.");

    double val = *v;
    free(v);
    return val;
error:
    return 0.0;
}

void RPN_enterNumber(LinkedStack * stack, double value)
{
    double * box = malloc(sizeof(double));
    CHECK_MEM(box);
    *box = value;

    CHECK(LinkedStack_push(stack, box) != -1, "Failed to push number to stack.");
    return;

error:
    if (box != NULL) {
        free(box);
    }
}

double RPN_binaryOp(LinkedStack * stack, int op_type)
{
    double a = RPN_getOperand(stack);
    double b = RPN_getOperand(stack);

    double result = 0.0;
    switch (op_type) 
    {
        case OP_ADD:
            result = a + b;
            break;
        case OP_SUB:
            result = b - a;
            break;
        case OP_MUL:
            result = a * b;
            break;
        case OP_DIV:
            result = b / a;
            break;
        default:
            SENTINEL_DIE("UNKNOWN OPERATOR");
    }
    RPN_enterNumber(stack, result);
    return result;
}

int tryParseDouble(char * str, double * rv)
{
    CHECK_DEBUG(str != NULL && rv != NULL, "NULL given to tryParseDouble.");

    char * end = NULL;
    double d = strtod(str, &end);

    if (end == str) {
        // No characters consumed.
        LOG_DEBUG("Tried parsing non-double: %s", str);
        return 0;
    }
    else {
        *rv = d;
        return 1;
    }

error:
    return 0;   
}

int isOperator(char * str) {
    return (*str == '+' || *str == '-' || *str == '*' || *str == '/');
}

int isQuitCommand(char * str) {
    return *str == 'q' || *str == 'Q';
}

static const char * WHITESPACE_CHARS = " \f\n\r\t\v";

char * copyString(char * str)
{
    char * s = NULL;
    CHECK(str != NULL, "copyString given NULL");
    size_t len = strlen(str);

    s = malloc(len + 1);

    strncpy(s, str, len);

    return s;
error:
    if (s != NULL) {
        free(s);
    }
    return NULL;
}

TokenList * getTokensFromStdin()
{
    char * line = NULL;
    size_t len = 0;
    char * saveptr = NULL;
    char * token_str = NULL;

    TokenList * list = TokenList_new();
    CHECK_MEM(list);

    CHECK_DEBUG(getline(&line, &len, stdin) != -1, "Failed to get line.")
    CHECK_MEM(line)

    token_str = strtok_r(line, WHITESPACE_CHARS, &saveptr);
    do {
        if (token_str == NULL) {
            break;
        }

        LOG_DEBUG("TOKEN STRING: \'%s\'\n", token_str);

        int type;
        double num;
        if (tryParseDouble(token_str, &num)) {
            type = TOKEN_NUMBER;
            TokenList_appendToken(list, Token_new(type, &num));
        }
        else {
            type = isOperator(token_str) ? TOKEN_OPERATOR : TOKEN_OTHER;
            TokenList_appendToken(list, Token_new(type, copyString(token_str)));
        }

        token_str = strtok_r(NULL, WHITESPACE_CHARS, &saveptr);

    } while (token_str);

    free(line);

    return list;
error:
    if (list != NULL) {
        TokenList_destroy(list);
    }
    if (line != NULL) {
        free(line);
    }
    return NULL;
}

int main(int argc, char * argv[])
{
    printf("Reverse Polish Notation Calculator\n");
    LinkedStack * stack = LinkedStack_new();
    for (;;) {
        TokenList * tokens = getTokensFromStdin();

        LOG_DEBUG("GOT TOKEN LIST\n");

        if (tokens == NULL) {
            LOG_DEBUG("NULL LIST\n");
            continue;
        }

        LOG_DEBUG("TOKEN LIST NOT NULL\n");
        if (tokens->head == NULL) {
            LOG_DEBUG("TOKEN LIST HEAD NULL\n");
        }

        Token * cur = NULL;
        _TokenListNode * node;
        for (node = tokens->head;
             node != NULL;
             node = node->next)
        {
            cur = node->token;

            LOG_DEBUG("CUR: %p : %d\n", cur, cur->type);

            if (cur == NULL) {
                LOG_DEBUG("NULL TOKEN\n");
                continue;
            }

            double result = 0.0;
            switch (cur->type) 
            {
                case TOKEN_OPERATOR:
                    LOG_DEBUG("OPERATOR: %s\n", cur->str);
                    switch (*(cur->str)) 
                    {
                        case '+':
                            result = RPN_binaryOp(stack, OP_ADD);
                            break;
                        case '-':
                            result = RPN_binaryOp(stack, OP_SUB);
                            break;
                        case '*':
                            result = RPN_binaryOp(stack, OP_MUL);
                            break;
                        case '/':
                            result = RPN_binaryOp(stack, OP_DIV);
                            break;
                    }
                    printf("%f\n", result);
                    break;
                case TOKEN_NUMBER:
                    LOG_DEBUG("NUMBER: %f\n", cur->num);
                    RPN_enterNumber(stack, cur->num);
                    break;
                case TOKEN_OTHER:
                    if (isQuitCommand(cur->str)) {
                        LinkedStack_destroy(stack);
                        TokenList_destroy(tokens);
                        return EXIT_SUCCESS;
                    }
                    break;
                default:
                    SENTINEL_DIE("Unknown token type.");
            }
        }
        TokenList_destroy(tokens);
    }

    return EXIT_SUCCESS;
}
