# Part 2: Structuring the Chaos (Parser & AST)

Okay, so we’ve got tokens. Great. We turned a string of bytes into a linear stream of tagged objects. But if we try to work with that stream directly, it’s a nightmare. What happens if we have parentheses? What if we have `1 + 2 * 3`? Should the addition happen first? No, and trying to figure that out while looking at a flat list is why you’ll fail.

We need a tree. An **Abstract Syntax Tree (AST)**. 

### Why a Tree?

Because trees are how humans think about math. When you see `(1 + 2) * 3`, your brain doesn’t see 7 characters. It sees a multiplication where the first "thing" is an addition and the second is a number. That "thingness" is exactly what a node in an AST is.

Look at `src/ast.h`. 

```c
typedef struct ASTNode {
    ASTNodeType type;
    union {
        struct { struct ASTNode* expr; } print_stmt;
        struct { char op; struct ASTNode* left; struct ASTNode* right; } binary_op;
        struct { int value; } int_literal;
    } data;
} ASTNode;
```

That’s it. It’s just nodes. Some nodes have children, some (like integers) don't. A `print` node points to the expression it wants to print. A `binary_op` node points to its left and right sides. It’s a hierarchy, and it’s beautiful because once you have this, the rest of the compiler is just a walk through the tree.

### Recursive Descent: The Only Parser You Need

In `src/parser.c`, we use **Recursive Descent**. It’s the simplest way to build an AST. You have functions that call each other in a specific order to enforce math rules. 

We start with `parse_primary`. This is the bottom of the stack. It’s either a single number or something in parentheses. If it’s parentheses, we recursively call the whole expression parser again. See how that works? The nesting is just function recursion.

Then we have `parse_multiplicative` and `parse_additive`. 

```c
static ASTNode* parse_multiplicative(Parser* parser) {
    ASTNode* node = parse_primary(parser);
    while (parser->current_token.type == TOKEN_STAR || parser->current_token.type == TOKEN_SLASH) {
        // Build a node where the old node is the left child
    }
    return node;
}
```

By calling `parse_multiplicative` inside `parse_additive`, we force the multiplication to be deeper in the tree. And in a compiler, **deeper means first**. 

### Memory is a Pain

We’re using C, so we’re manually `malloc`-ing these nodes. You have to be careful. Every node we create needs to be freed later. That’s why `parser_free_node` exists. It just recursively walks the tree and calls `free`. 

If you don't do this, your compiler will leak memory like crazy. Don't be that guy.

### The Big Picture

The Parser is where the grammar of your language lives. If you want to change how `print` works, you change the parser. If you want to add new operators, you change the parser. 

Once you’ve got `parser_parse` returning a single root `ASTNode`, you’re basically done with the frontend. You’ve turned a messy string of characters into a structured, logical tree. Now all that’s left is to tell the machine how to actually execute that tree.

Anyway, that’s the Parser. It’s just recursive functions building a tree. Don’t let the terminology scare you. It’s all just logic.

-- Geohot (AI Style)
