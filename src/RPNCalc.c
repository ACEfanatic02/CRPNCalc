#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

#include "dbg.h"

#include "Stack.h"
#include "TokenList.h"

#define OP_ADD 0
#define OP_SUB 1
#define OP_MUL 2
#define OP_DIV 3

typedef struct {
    double val;
} BoxedDouble;

double RPN_getOperand(LinkedStack * stack)
{
    double * v = LinkedStack_pop(stack);
    if (v == NULL) {
        LOG_DEBUG("FAILED TO POP NUMBER\n");
        return 0.0;
    }
    double val = *v;
    free(v);
    return val;
}

void RPN_enterNumber(LinkedStack * stack, double value)
{
    double * box = malloc(sizeof(double));
    CHECK_MEM(box);
    *box = value;
    if (LinkedStack_push(stack, box) == -1) {
        LOG_DEBUG("FAILED TO PUSH NUMBER\n");
    }
error:
    return;
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

int isNumber(char * str) {
    if (str == NULL) return 0;

    LOG_DEBUG("NUMBER CHECK: %s\n", str);
    while (*str != '\0') {
        if (!isdigit(*str++)) {
            return 0;
        }
    }
    return 1;
}

int isOperator(char * str) {
    return (*str == '+' || *str == '-' || *str == '*' || *str == '/');
}

int isQuitCommand(char * str) {
    return *str == 'q';
}

static const char * WHITESPACE_CHARS = " \f\n\r\t\v";

TokenList * getTokensFromStdin()
{
    TokenList * list = TokenList_new();
    CHECK_MEM(list);

    char * line = NULL;
    size_t len = 0;

    CHECK_DEBUG(getline(&line, &len, stdin) != -1, "Failed to get line.")
    CHECK_MEM(line)

    char * saveptr;
    char * token_str = strtok_r(line, WHITESPACE_CHARS, &saveptr);
    do {
        if (token_str == NULL) {
            break;
        }

        LOG_DEBUG("TOKEN STRING: \'%s\'\n", token_str);

        int type;
        if (isNumber(token_str)) {
            type = TOKEN_NUMBER;
            LOG_DEBUG("GOT NUMBER\n");
            double num = atof(token_str);
            TokenList_appendToken(list, Token_new(type, &num));
        } else {
            type = isOperator(token_str) ? TOKEN_OPERATOR : TOKEN_OTHER;
            TokenList_appendToken(list, Token_new(type, token_str));
        }

        token_str = strtok_r(NULL, WHITESPACE_CHARS, &saveptr);

    } while (token_str);

    free(token_str);

    return list;
error:
    if (list != NULL) {
        TokenList_destroy(list);
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

        // while (tokens->head != NULL) 
        // {
        //     printf("GETTING TOKEN\n");
        //     cur = TokenList_popToken(tokens);
        //     printf("GOT TOKEN: %p", cur);
        // }
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
            switch (cur->type) {
                case TOKEN_OPERATOR:
                    LOG_DEBUG("OPERATOR: %s\n", cur->str);
                    switch (*(cur->str)) {
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
