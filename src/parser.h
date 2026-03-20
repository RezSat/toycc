#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "ast.h"

typedef struct {
    Lexer* lexer;
    Token current_token;
} Parser;

void parser_init(Parser* parser, Lexer* lexer);
ASTNode* parser_parse(Parser* parser);
void parser_free_node(ASTNode* node);

#endif
