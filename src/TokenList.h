/* TokenList.h */
#ifndef TOKENLIST_H
#define TOKENLIST_H

// Token types.
#define TOKEN_OPERATOR  0
#define TOKEN_NUMBER    1
#define TOKEN_OTHER     2

typedef struct {
    int type;
    union {
        // Number tokens contain num, all others contain str.
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

/* Create a new TokenListNode. */
_TokenListNode * TokenListNode_new(Token * token);

/* Destroy a TokenListNode. */
void TokenListNode_destroy(_TokenListNode * node);

/* Create a new TokenList. */
TokenList * TokenList_new();

/* Destroy a TokenList. */
void TokenList_destroy(TokenList * list);

/* Create and append a new token to given TokenList. */
TokenList * TokenList_appendToken(TokenList * list, Token * token);

/* Pop and return last token on given TokenList. */
Token * TokenList_popToken(TokenList * list);

/* Create a new Token. */
Token * Token_new(int type, void * val);

/* Destroy a Token. */
void Token_destroy(Token * token);

#endif
