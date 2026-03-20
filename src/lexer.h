#ifndef LEXER_H
#define LEXER_H

typedef enum {
    TOKEN_PRINT,
    TOKEN_INT,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_STAR,
    TOKEN_SLASH,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_SEMICOLON,
    TOKEN_EOF,
    TOKEN_UNKNOWN
} TokenType;

typedef struct {
    TokenType type;
    char* text;
    int value; // for integer literals
} Token;

typedef struct {
    const char* source;
    int pos;
} Lexer;

void lexer_init(Lexer* lexer, const char* source);
Token lexer_next_token(Lexer* lexer);
void token_free(Token token);

#endif
