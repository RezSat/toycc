#ifndef AST_H
#define AST_H

typedef enum {
    AST_PRINT,
    AST_BINARY_OP,
    AST_INT
} ASTNodeType;

typedef struct ASTNode {
    ASTNodeType type;
    union {
        struct {
            struct ASTNode* expr;
        } print_stmt;
        struct {
            char op;
            struct ASTNode* left;
            struct ASTNode* right;
        } binary_op;
        struct {
            int value;
        } int_literal;
    } data;
} ASTNode;

#endif
