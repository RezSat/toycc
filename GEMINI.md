# Project goal

Build a very small compiler in C on Windows 10 using LLVM.

## Language v1
Only support:
- print <expr>;
- integer literals
- +, -, *, /
- parentheses

Examples:
print 1 + 2;
print (3 + 4) * 5;

## Architecture
- lexer
- parser
- AST
- LLVM IR codegen

## Rules
- Keep code simple and beginner-friendly
- Use C, not C++
- No advanced optimizations
- One step at a time
- Always explain what file to create or edit
- Do not rewrite everything unless asked
- Prefer small diffs


# Suggested Strcuture (You are free to change the structure as necessary)
toycc/
  src/ (not created)
    main.c
    lexer.c
    lexer.h
    parser.c
    parser.h
    ast.h
    codegen.c
    codegen.h
  CMakeLists.txt (created)
  GEMINI.md (created-this file)
  README.md (created-emtpty)
  examples/ (not created)
    test1.toy