# Part 1: The Primitive Pipe (The Lexer)

Compilers aren't magic. Stop thinking they are. People treat them like some mystical black box that turns text into logic, but it's just a pipe. You have bits on one end, and you want them in a different shape on the other. That’s it. We’re building `toycc` because the best way to understand the stack is to build it from the ground up, no bloat, no unnecessary abstractions.

We’re starting with the Lexer. Why? Because the CPU doesn't care about your "print" statement or your parentheses. It barely cares about your ASCII. The first step is to turn that raw string of characters into something structured—Tokens.

### The Token: The Atom of Your Language

Look at `src/lexer.h`. It’s simple. We define an `enum` for `TokenType`. 

```c
typedef enum {
    TOKEN_PRINT, TOKEN_INT, TOKEN_PLUS, TOKEN_MINUS, 
    TOKEN_STAR, TOKEN_SLASH, TOKEN_LPAREN, TOKEN_RPAREN, 
    TOKEN_SEMICOLON, TOKEN_EOF, TOKEN_UNKNOWN
} TokenType;
```

That’s your universe. Anything outside of that doesn't exist to `toycc`. We wrap it in a `Token` struct that carries the type, the literal text, and the integer value if it's a number. Don't over-engineer this. We're just tagging chunks of a string so the next stage doesn't have to deal with raw bytes.

### Moving the Pointer

The `Lexer` itself? Just a pointer and a string.

```c
typedef struct {
    const char* source;
    int pos;
} Lexer;
```

We initialize it at position 0 and then we start eating. In `src/lexer.c`, we have `lexer_next_token`. This is the heartbeat of the whole first stage. 

First, we skip whitespace. Why? Because whitespace is noise. 

```c
static void lexer_skip_whitespace(Lexer* lexer) {
    while (isspace(lexer_peek(lexer))) {
        lexer_advance(lexer);
    }
}
```

Then we look at what’s left. If it’s a digit, we keep eating until it’s not a digit. That’s your `TOKEN_INT`. We use `atoi` to turn that chunk of string into a real C `int`. Simple.

If it’s a letter, we check if it’s "print". If it is, boom, `TOKEN_PRINT`. If not, it’s an error for now because our language is tiny. 

The rest? Single characters. `+`, `-`, `*`, `/`. We just map the character to the token type.

### Why does this matter?

The Lexer is the most "physical" part of the compiler. You’re touching the raw bytes of the source file. It’s the gatekeeper. If your lexer is slow, your compiler is slow. If your lexer is buggy, nothing else works.

But don't get stuck here. The lexer is just a utility. It’s the pipe that feeds the parser. You want to get this done, make it robust enough to handle the 5-10 tokens we care about, and then move up the stack.

In the next part, we take these tokens and turn them into a tree. Because linear tokens are still too primitive for real logic. We need structure. We need the AST.

Anyway, that’s the lexer. It’s just a string pointer moving forward. Don’t make it more complicated than it is.

-- Geohot (AI Style)
