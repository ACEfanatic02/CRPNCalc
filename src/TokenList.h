/* TokenList.h */
#ifndef TOKENLIST_H
#define TOKENLIST_H

#define TOKEN_OPERATOR  0
#define TOKEN_NUMBER    1
#define TOKEN_OTHER     2

typedef struct {
    int type;
    union {
        char * str;
        double num;
    };
} Token;

typedef struct _TokenListNode {
    struct _TokenListNode * next;
    struct _TokenListNode * prev;
    Token * token;
} _TokenListNode;

typedef struct {
    _TokenListNode * head;
    _TokenListNode * tail;
    int length;
} TokenList;

_TokenListNode * TokenListNode_new(Token * token);

void TokenListNode_destroy(_TokenListNode * node);

TokenList * TokenList_new();

void TokenList_destroy(TokenList * list);

TokenList * TokenList_appendToken(TokenList * list, Token * token);

Token * TokenList_popToken(TokenList * list);

Token * Token_new(int type, void * val);

void Token_destroy(Token * token);

#endif
