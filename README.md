# ToyCC, A Tiny LLVM-Based Compiler in C

A minimal toy compiler written in C that compiles a simple expression language directly into native executables using LLVM.

This was vibe-coded with Gemini CLI.

---

## Features

* Simple custom language
* Integer arithmetic support
* Operator precedence and parentheses
* `print` statements
* Generates executables directly (`.exe`)
* Uses LLVM under the hood

---

## Example

### Input (toy language)

```
print 1 + 2;
print (3 + 4) * 5;
```

### Output

```
3
35
```

---

## Language Specification (v1)

Supported syntax:

* Integer literals
* Binary operators: `+`, `-`, `*`, `/`
* Parentheses
* Print statements:

```
print <expression>;
```

---

## Project Structure

```
toycc/
├── src/
│   ├── main.c
│   ├── lexer.c / lexer.h
│   ├── parser.c / parser.h
│   ├── ast.h
│   ├── codegen.c / codegen.h
├── examples/
│   └── test1.toy
├── build/
├── CMakeLists.txt
└── README.md
```

---

## Build Instructions (Windows 10)

### Requirements

* LLVM (Clang)
* CMake
* Visual Studio Build Tools (C/C++)

### Build

```
cmake -S . -B build
cmake --build build
```

---

## Usage

### Compile a toy program into an executable

```
.\build\toycc.exe examples\test1.toy
```

This directly generates:

```
test1.exe
```

### Run

```
.\test1.exe
```

---

## How It Works

Pipeline:

```
Source Code
   ↓
Lexer
   ↓
Parser
   ↓
AST
   ↓
LLVM IR Generation
   ↓
LLVM Backend → Native Executable
```

> Note: LLVM IR is generated internally but does not require manual compilation.

---

## Development Approach

This project was built using a structured workflow with Gemini CLI:

* Small, focused prompts
* Incremental development (lexer → parser → AST → codegen)
* Manual compilation and debugging at each step
* Minimal, controlled changes instead of large rewrites


---

## Why This Project

The goal was to **see how it will integrate LLVM with C**, not just generate code. Because most of instructions I found was with C++

This compiler is intentionally minimal to focus on:

* Core compiler architecture
* LLVM-based code generation
* Systems programming in C