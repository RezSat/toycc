#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"
#include "parser.h"
#include "codegen.h"

char* read_file(const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (!file) {
        fprintf(stderr, "Could not open file %s\n", filename);
        return NULL;
    }
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* buffer = (char*)malloc(size + 1);
    fread(buffer, 1, size, file);
    buffer[size] = '\0';

    fclose(file);
    return buffer;
}

void get_output_name(const char* input_path, char* output_exe, char* output_ll) {
    // Basic path handling to change .toy to .exe and .ll
    strcpy(output_exe, input_path);
    strcpy(output_ll, input_path);
    
    char* dot = strrchr(output_exe, '.');
    if (dot) {
        strcpy(dot, ".exe");
    } else {
        strcat(output_exe, ".exe");
    }

    dot = strrchr(output_ll, '.');
    if (dot) {
        strcpy(dot, ".ll");
    } else {
        strcat(output_ll, ".ll");
    }
}

int main(int argc, char** argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: toycc <filename.toy>\n");
        return 1;
    }

    const char* input_file = argv[1];
    char output_exe[256];
    char output_ll[256];
    get_output_name(input_file, output_exe, output_ll);

    char* source = read_file(input_file);
    if (!source) return 1;

    Lexer lexer;
    lexer_init(&lexer, source);

    Parser parser;
    parser_init(&parser, &lexer);

    ASTNode* root = parser_parse(&parser);
    if (root) {
        Codegen cg;
        codegen_init(&cg, "toycc_module");

        LLVMTypeRef main_func_type = LLVMFunctionType(LLVMInt32Type(), NULL, 0, 0);
        LLVMValueRef main_func = LLVMAddFunction(cg.module, "main", main_func_type);
        LLVMBasicBlockRef entry = LLVMAppendBasicBlock(main_func, "entry");
        LLVMPositionBuilderAtEnd(cg.builder, entry);

        codegen_generate(&cg, root);

        LLVMBuildRet(cg.builder, LLVMConstInt(LLVMInt32Type(), 0, 0));

        // 1. Generate the LLVM IR file
        codegen_finish(&cg, output_ll);
        printf("Compiling %s -> %s\n", input_file, output_exe);

        codegen_cleanup(&cg);
        parser_free_node(root);

        // 2. Invoke clang to create the executable
        // Using MinGW target/sysroot as it's the most reliable on this machine
        char cmd[1024];
        snprintf(cmd, sizeof(cmd), "clang --target=x86_64-w64-mingw32 --sysroot=C:\\msys64\\mingw64 %s -o %s", output_ll, output_exe);
        int res = system(cmd);
        
        if (res == 0) {
            printf("Successfully built: %s\n", output_exe);
        } else {
            fprintf(stderr, "Error: clang failed to produce executable.\n");
            return 1;
        }
    } else {
        fprintf(stderr, "Parsing failed.\n");
        return 1;
    }

    free(source);
    return 0;
}
