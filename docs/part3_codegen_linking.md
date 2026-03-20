# Part 3: From Trees to Iron (Codegen & Linking)

We’ve got a tree. Now what? You can’t just tell a CPU "hey, traverse this tree and do the math." The CPU doesn’t understand trees. It understands **instructions**. Specifically, it understands registers, memory offsets, and jump addresses. 

But writing an x86 backend from scratch is a massive waste of time for a project like this. We’re using **LLVM**.

### LLVM is the Bridge

LLVM is the absolute best thing to happen to compilers. It’s an intermediate layer. We give it **Intermediate Representation (IR)**, which looks a bit like assembly but is much cleaner, and LLVM handles all the nasty machine-specific optimizations and machine code generation. 

Look at `src/codegen.c`. We walk the tree we built in the last stage. 

If we hit an `AST_INT` node, we tell LLVM: `LLVMConstInt`. Done. 
If we hit a `AST_BINARY_OP` node, we recursively generate the left and right sides, and then tell LLVM: `LLVMBuildAdd` or `LLVMBuildMul`.

LLVM will take those commands and produce something like:
```llvm
%addtmp = add i32 %1, %2
```

It’s that simple. We’re basically just translating our tree nodes into a linear list of LLVM commands. 

### The "Print" Statement

Our language has a `print` statement. How does that translate to the machine? We have to link against a library that knows how to talk to the operating system's console. 

We declare an external function, `printf`. 

```c
LLVMTypeRef printf_args[] = { LLVMPointerType(LLVMInt8Type(), 0) };
LLVMTypeRef printf_type = LLVMFunctionType(LLVMInt32Type(), printf_args, 1, 1);
return LLVMAddFunction(codegen->module, "printf", printf_type);
```

Then we just call it. We generate the math expression, pass the result as an argument to `printf`, and boom—we have output. 

### The Build Loop: From IR to .EXE

The IR we generate is still just text in an `.ll` file. To turn it into a real, runnable Windows program, we need a linker. We’re using `clang` for that. 

In `src/main.c`, we use `system()` to call `clang`:
```c
snprintf(cmd, sizeof(cmd), "clang --target=x86_64-w64-mingw32 --sysroot=C:\\msys64\\mingw64 %s -o %s", output_ll, output_exe);
system(cmd);
```

This is the final step. `clang` takes our IR, links it with the C standard library (so `printf` works), and produces a machine-code executable that your CPU can run natively. No interpreter, no overhead. Just pure logic.

### Why this is the correct way

`toycc` is a real compiler. It follows the same architecture as the big boys—Clang, Rustc, Swift. Lexer -> Parser -> AST -> IR -> Machine Code. By building it this way, you’re not just making a toy; you’re learning the actual stack. 

Once you have this core working, adding variables or `if` statements is just adding a few more node types and codegen rules. The structure is sound.

Anyway, that’s how you build a compiler. It’s not magic; it’s just a series of transformations. From raw bytes to tokens, from tokens to trees, from trees to IR, and finally, from IR to machine code. 

Go break some stuff. That’s how you really learn. 

-- Geohot (AI Style)
