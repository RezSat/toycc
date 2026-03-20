#include <stdio.h>
#include <stdlib.h>
#include "parser.h"

static void parser_advance(Parser* parser) {
    token_free(parser->current_token);
    parser->current_token = lexer_next_token(parser->lexer);
}

static ASTNode* create_node(ASTNodeType type) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    node->type = type;
    return node;
}

void parser_init(Parser* parser, Lexer* lexer) {
    parser->lexer = lexer;
    parser->current_token = lexer_next_token(lexer);
}

// Forward declarations for recursive descent
static ASTNode* parse_expression(Parser* parser);

// primary = INT | "(" expression ")"
static ASTNode* parse_primary(Parser* parser) {
    if (parser->current_token.type == TOKEN_INT) {
        ASTNode* node = create_node(AST_INT);
        node->data.int_literal.value = parser->current_token.value;
        parser_advance(parser);
        return node;
    } else if (parser->current_token.type == TOKEN_LPAREN) {
        parser_advance(parser);
        ASTNode* node = parse_expression(parser);
        if (parser->current_token.type == TOKEN_RPAREN) {
            parser_advance(parser);
        } else {
            fprintf(stderr, "Expected ')'\n");
        }
        return node;
    }
    fprintf(stderr, "Unexpected token in primary\n");
    return NULL;
}

// multiplicative = primary ( ( "*" | "/" ) primary )*
static ASTNode* parse_multiplicative(Parser* parser) {
    ASTNode* node = parse_primary(parser);
    while (parser->current_token.type == TOKEN_STAR || parser->current_token.type == TOKEN_SLASH) {
        char op = parser->current_token.text[0];
        parser_advance(parser);
        ASTNode* new_node = create_node(AST_BINARY_OP);
        new_node->data.binary_op.op = op;
        new_node->data.binary_op.left = node;
        new_node->data.binary_op.right = parse_primary(parser);
        node = new_node;
    }
    return node;
}

// additive = multiplicative ( ( "+" | "-" ) multiplicative )*
static ASTNode* parse_additive(Parser* parser) {
    ASTNode* node = parse_multiplicative(parser);
    while (parser->current_token.type == TOKEN_PLUS || parser->current_token.type == TOKEN_MINUS) {
        char op = parser->current_token.text[0];
        parser_advance(parser);
        ASTNode* new_node = create_node(AST_BINARY_OP);
        new_node->data.binary_op.op = op;
        new_node->data.binary_op.left = node;
        new_node->data.binary_op.right = parse_multiplicative(parser);
        node = new_node;
    }
    return node;
}

static ASTNode* parse_expression(Parser* parser) {
    return parse_additive(parser);
}

// statement = "print" expression ";"
ASTNode* parser_parse(Parser* parser) {
    if (parser->current_token.type == TOKEN_PRINT) {
        parser_advance(parser);
        ASTNode* expr = parse_expression(parser);
        if (parser->current_token.type == TOKEN_SEMICOLON) {
            parser_advance(parser);
            ASTNode* node = create_node(AST_PRINT);
            node->data.print_stmt.expr = expr;
            return node;
        } else {
            fprintf(stderr, "Expected ';'\n");
        }
    }
    return NULL;
}

void parser_free_node(ASTNode* node) {
    if (!node) return;
    if (node->type == AST_PRINT) {
        parser_free_node(node->data.print_stmt.expr);
    } else if (node->type == AST_BINARY_OP) {
        parser_free_node(node->data.binary_op.left);
        parser_free_node(node->data.binary_op.right);
    }
    free(node);
}
