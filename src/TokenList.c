#include <stdlib.h>
#include <stdio.h>
#include "TokenList.h"

_TokenListNode * TokenListNode_new(Token * token)
{
    _TokenListNode * node = malloc(sizeof(_TokenListNode));
    if (node == NULL) {
        goto error;
    }

    node->token = token;
    node->next = NULL;
    node->prev = NULL;

    return node;
error:
    return NULL;
}

void TokenListNode_destroy(_TokenListNode * node)
{
    if (node == NULL) {
        return;
    }

    if (node->token != NULL) {
        Token_destroy(node->token);
    }

    free(node);
}

TokenList * TokenList_new()
{
    TokenList * list = malloc(sizeof(TokenList));
    if (list == NULL) {
        goto error;
    }

    list->head = NULL;
    list->tail = NULL;
    list->length = 0;

    return list;

error:
    return NULL;
}

void TokenList_destroy(TokenList * list)
{
    if (list == NULL) {
        return;
    }

    if (!(list->head == NULL) || !(list->tail == NULL)) {
        // Destroy contents of list
        _TokenListNode * node;
        while (list->head != NULL) {
            node = list->head;
            list->head = node->next;
            TokenListNode_destroy(node);
        }
    }

    free(list);
}

TokenList * TokenList_appendToken(TokenList * list, Token * token)
{
    if (list == NULL) {
        goto error;
    }

    _TokenListNode * node = TokenListNode_new(token);
    if (node == NULL) {
        printf("BUILT NULL NODE\n");
        goto error;
    }
    
    if (list->head == NULL || list->tail == NULL) {
        // Empty list
        list->head = node;
        list->tail = node;
    }
    else {
        list->tail->next = node;
        node->prev = list->tail;
        list->tail = node;
    }

    return list;
error:
    return NULL;
}

Token * TokenList_popToken(TokenList * list)
{
    printf("POPPING TOKEN\n");
    if (list == NULL) {
        goto error;
    }

    _TokenListNode * node = list->tail;
    if (list->tail == list->head) {
        // Single item
        list->tail = NULL;
        list->head = NULL;
    } else {
        list->tail = node->prev;
        list->tail->next = NULL;
    }
    Token * token = node->token;
    TokenListNode_destroy(node); 

    return token;
error:
    return NULL;
}

Token * Token_new(int type, void * val)
{
    Token * token = NULL;
    if (val == NULL) {
        goto error;
    }

    token = malloc(sizeof(Token));
    if (token == NULL) {
        goto error;
    }

    token->type = type;

    switch (type) {
        case TOKEN_OPERATOR:
        case TOKEN_OTHER:
            token->str = (char *)val; // TODO: copy string here
            break;
        case TOKEN_NUMBER:
            token->num = *(double *)(val);
            break;
        default:
            // LOG_ERROR("Unknown Token Type");
            goto error;
    }

    return token;

error:
    if (token != NULL) {
        free(token);
    }
    return NULL;
}


void Token_destroy(Token * token)
{
    if (token == NULL) {
        return;
    }

    free(token);
}

