#include <stdlib.h>
#include <stdio.h>

#include "dbg.h"

#include "Stack.h"

LinkedStack * LinkedStack_new()
{
    LinkedStack * stack = malloc(sizeof(LinkedStack));
    CHECK_MEM(stack);
    stack->head = NULL;

    return stack;

error:
    return NULL;
}

int LinkedStack_push(LinkedStack * stack, void * value)
{
    CHECK(stack != NULL, "Push given NULL stack.");

    _StackNode * node = malloc(sizeof(_StackNode));

    CHECK_MEM(node);

    node->val = value;
    node->next = stack->head;
    stack->head = node;

    return 0;
    
error:
    return -1;
}

void * LinkedStack_pop(LinkedStack * stack)
{
    CHECK(stack != NULL, "Pop given NULL stack.");

    _StackNode * popped = stack->head;
    CHECK_DEBUG(popped != NULL, "Popped NULL node.")

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
    CHECK_DEBUG(stack != NULL, "Destroying NULL stack.");

    while (!LinkedStack_empty(stack)) {
        _StackNode * n = stack->head;
        stack->head = n->next;
        free(n->val);
        free(n);
    }

    free(stack);
error:
    return;
}