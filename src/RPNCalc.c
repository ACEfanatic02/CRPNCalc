#include "Stack.h"

#define OP_ADD 0
#define OP_SUB 1
#define OP_MUL 2
#define OP_DIV 3

typedef struct {
    double val;
} BoxedDouble;

double RPN_getOperand(LinkedStack * stack)
{
    BoxedDouble * v = LinkedStack_pop(stack);
    if (v == NULL) {
        return 0.0;
    }
    double val = v->val;
    free(v);
    return val;
}

void RPN_enterNumber(LinkedStack * stack, double value)
{
    BoxedDouble * box = malloc(sizeof(BoxedDouble));
    if (box == NULL) {
        return;
    }
    box->val = value;
    LinkedStack_push(stack, box);
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
            assert(0, "UNKNOWN OPERATOR");
    }
    RPN_enterNumber(stack, result);
    return result;
}

