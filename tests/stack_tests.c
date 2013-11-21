#include "../src/Stack.h"

#include "minunit.h"

char * test_stack_push_pop()
{
    LinkedStack * stack = LinkedStack_new();

    char * first = "first";
    char * second = "second";

    mu_assert(LinkedStack_push(stack, first) == 0, "Failed to push first item");
    mu_assert(LinkedStack_push(stack, second) == 0, "Failed to push second item");

    mu_assert(LinkedStack_pop(stack) == second, "Failed to pop second item.");
    mu_assert(LinkedStack_pop(stack) == first, "Failed to pop first item.");
    mu_assert(LinkedStack_pop(stack) == NULL, "Failed to pop NULL from empty stack.");
    return NULL;
}

char * all_tests() {
    mu_suite_start();

    mu_run_test(test_stack_push_pop);

    return NULL;
}

RUN_TESTS(all_tests);