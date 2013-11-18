/**
 * Stack.h
 */

#ifndef STACK_H
#define STACK_H

typedef struct _StackNode {
    void * val;
    struct _StackNode * next;
} _StackNode;

typedef struct {
    _StackNode * head;
} LinkedStack;

/* Create a new LinkedStack */
LinkedStack * LinkedStack_new();

/* Push an element onto the stack. */
int LinkedStack_push(LinkedStack * stack, void * value);

/* Pop top element of stack. */
void * LinkedStack_pop(LinkedStack * stack);

/* Check whether the stack is empty. */
int LinkedStack_empty(LinkedStack * stack);

void LinkedStack_destroy(LinkedStack * stack);

#endif