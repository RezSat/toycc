#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "lexer.h"

void lexer_init(Lexer* lexer, const char* source) {
    lexer->source = source;
    lexer->pos = 0;
}

static char lexer_peek(Lexer* lexer) {
    return lexer->source[lexer->pos];
}

static char lexer_advance(Lexer* lexer) {
    return lexer->source[lexer->pos++];
}

static void lexer_skip_whitespace(Lexer* lexer) {
    while (isspace(lexer_peek(lexer))) {
        lexer_advance(lexer);
    }
}

Token lexer_next_token(Lexer* lexer) {
    lexer_skip_whitespace(lexer);

    char current = lexer_peek(lexer);

    if (current == '\0') {
        Token token = {TOKEN_EOF, NULL, 0};
        return token;
    }

    if (isdigit(current)) {
        int start = lexer->pos;
        while (isdigit(lexer_peek(lexer))) {
            lexer_advance(lexer);
        }
        int length = lexer->pos - start;
        char* text = (char*)malloc(length + 1);
        strncpy(text, &lexer->source[start], length);
        text[length] = '\0';
        int value = atoi(text);
        Token token = {TOKEN_INT, text, value};
        return token;
    }

    if (isalpha(current)) {
        int start = lexer->pos;
        while (isalnum(lexer_peek(lexer))) {
            lexer_advance(lexer);
        }
        int length = lexer->pos - start;
        char* text = (char*)malloc(length + 1);
        strncpy(text, &lexer->source[start], length);
        text[length] = '\0';

        if (strcmp(text, "print") == 0) {
            Token token = {TOKEN_PRINT, text, 0};
            return token;
        }

        Token token = {TOKEN_UNKNOWN, text, 0};
        return token;
    }

    lexer_advance(lexer);
    Token token;
    token.text = (char*)malloc(2);
    token.text[0] = current;
    token.text[1] = '\0';
    token.value = 0;

    switch (current) {
        case '+': token.type = TOKEN_PLUS; break;
        case '-': token.type = TOKEN_MINUS; break;
        case '*': token.type = TOKEN_STAR; break;
        case '/': token.type = TOKEN_SLASH; break;
        case '(': token.type = TOKEN_LPAREN; break;
        case ')': token.type = TOKEN_RPAREN; break;
        case ';': token.type = TOKEN_SEMICOLON; break;
        default:  token.type = TOKEN_UNKNOWN; break;
    }

    return token;
}

void token_free(Token token) {
    if (token.text) {
        free(token.text);
    }
}
