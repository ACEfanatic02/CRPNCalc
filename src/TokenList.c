#include <stdlib.h>
#include <stdio.h>

#include "dbg.h"

#include "TokenList.h"

_TokenListNode * TokenListNode_new(Token * token)
{
    _TokenListNode * node = malloc(sizeof(_TokenListNode));
    CHECK_MEM(node);

    node->token = token;
    node->next = NULL;
    node->prev = NULL;

    return node;
error:
    return NULL;
}

void TokenListNode_destroy(_TokenListNode * node)
{
    CHECK_DEBUG(node != NULL, "Destroying NULL node");

    if (node->token != NULL) {
        Token_destroy(node->token);
    }

    free(node);
error:
    return;
}

TokenList * TokenList_new()
{
    TokenList * list = malloc(sizeof(TokenList));
    CHECK_MEM(list)

    list->head = NULL;
    list->tail = NULL;
    list->length = 0;

    return list;

error:
    return NULL;
}

void TokenList_destroy(TokenList * list)
{
    CHECK_DEBUG(list != NULL, "Destroying NULL list.");

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
error:
    return;
}

TokenList * TokenList_appendToken(TokenList * list, Token * token)
{
    CHECK(list != NULL, "Append to null list.");

    _TokenListNode * node = TokenListNode_new(token);
    CHECK(node != NULL, "Built NULL node.");
    
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
    CHECK(list != NULL, "Pop from NULL list.");

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
    
    CHECK(val != NULL, "Token_new with NULL value.");

    token = malloc(sizeof(Token));
    CHECK_MEM(token);

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
            SENTINEL("Unknown Token Type");
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
    CHECK_DEBUG(token != NULL, "Destroying NULL token.");

    if (token->type != TOKEN_NUMBER && token->str != NULL) {
        // free contained string;
        free(token->str);
    }

    free(token);
error:
    return;
}

