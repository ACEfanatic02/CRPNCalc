#include <stdlib.h>

#include "Stack.h"

LinkedStack * LinkedStack_new()
{
    LinkedStack * stack = malloc(sizeof(LinkedStack));
    if (stack == NULL) {
        goto error;
    }
    stack->head = NULL;

    return stack;

error:
    return NULL;
}

int LinkedStack_push(LinkedStack * stack, void * value)
{
    if (stack == NULL) {
        goto error;
    }

    _StackNode * node = malloc(sizeof(_StackNode));
    if (node == NULL) {
        goto error;
    }
    node->val = value;
    node->next = stack->head;
    stack->head = node;

error:
    return -1;
}

void * LinkedStack_pop(LinkedStack * stack)
{
    if (stack == NULL) {
        goto error;
    }

    _StackNode * popped = stack->head;
    if (popped == NULL) {
        goto error;
    }

    stack->head = popped->next;
    void * rv = popped->val;
    free(popped);

    return rv;
error:
    return NULL;
}

int LinkedStack_empty(LinkedStack * stack)
{
    return stack == NULL || stack->head == NULL;
}

void LinkedStack_destroy(LinkedStack * stack)
{
    if (stack == NULL) {
        return;
    }

    while (!LinkedStack_empty(stack)) {
        _StackNode * n = stack->head;
        stack->head = n->next;
        free(n);
    }

    free(stack);
}