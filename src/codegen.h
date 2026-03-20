#ifndef CODEGEN_H
#define CODEGEN_H

#include <llvm-c/Core.h>
#include "ast.h"

typedef struct {
    LLVMModuleRef module;
    LLVMBuilderRef builder;
} Codegen;

void codegen_init(Codegen* codegen, const char* module_name);
LLVMValueRef codegen_generate(Codegen* codegen, ASTNode* node);
void codegen_finish(Codegen* codegen, const char* output_file);
void codegen_cleanup(Codegen* codegen);

#endif
