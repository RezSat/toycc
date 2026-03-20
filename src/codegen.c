#include <stdio.h>
#include <stdlib.h>
#include "codegen.h"

void codegen_init(Codegen* codegen, const char* module_name) {
    codegen->module = LLVMModuleCreateWithName(module_name);
    codegen->builder = LLVMCreateBuilder();
}

static LLVMTypeRef get_printf_type(void) {
    LLVMTypeRef printf_args[] = { LLVMPointerType(LLVMInt8Type(), 0) };
    return LLVMFunctionType(LLVMInt32Type(), printf_args, 1, 1);
}

static LLVMValueRef get_printf(Codegen* codegen) {
    LLVMValueRef printf_func = LLVMGetNamedFunction(codegen->module, "printf");
    if (printf_func) return printf_func;

    LLVMTypeRef printf_type = get_printf_type();
    return LLVMAddFunction(codegen->module, "printf", printf_type);
}

static LLVMValueRef get_format_string(Codegen* codegen) {
    LLVMValueRef format_str = LLVMGetNamedGlobal(codegen->module, "fmt");
    if (format_str) return format_str;

    const char* str = "%d\n";
    LLVMValueRef str_val = LLVMConstString(str, 4, 1);
    format_str = LLVMAddGlobal(codegen->module, LLVMTypeOf(str_val), "fmt");
    LLVMSetInitializer(format_str, str_val);
    LLVMSetGlobalConstant(format_str, 1);
    return format_str;
}

LLVMValueRef codegen_generate(Codegen* codegen, ASTNode* node) {
    if (!node) return NULL;

    switch (node->type) {
        case AST_INT:
            return LLVMConstInt(LLVMInt32Type(), node->data.int_literal.value, 0);

        case AST_BINARY_OP: {
            LLVMValueRef left = codegen_generate(codegen, node->data.binary_op.left);
            LLVMValueRef right = codegen_generate(codegen, node->data.binary_op.right);
            switch (node->data.binary_op.op) {
                case '+': return LLVMBuildAdd(codegen->builder, left, right, "addtmp");
                case '-': return LLVMBuildSub(codegen->builder, left, right, "subtmp");
                case '*': return LLVMBuildMul(codegen->builder, left, right, "multmp");
                case '/': return LLVMBuildSDiv(codegen->builder, left, right, "divtmp");
            }
            break;
        }

        case AST_PRINT: {
            LLVMValueRef val = codegen_generate(codegen, node->data.print_stmt.expr);
            LLVMValueRef printf_func = get_printf(codegen);
            LLVMValueRef fmt_str = get_format_string(codegen);

            // GEP to get pointer to format string
            LLVMTypeRef fmt_str_type = LLVMGetElementType(LLVMTypeOf(fmt_str));
            // Actually LLVMGetElementType is deprecated.
            // But fmt_str is a GlobalVariable, which is a pointer.
            // The type of fmt_str is [4 x i8]*
            // So the element type is [4 x i8]
            LLVMTypeRef element_type = LLVMArrayType(LLVMInt8Type(), 4);

            LLVMValueRef indices[] = { LLVMConstInt(LLVMInt32Type(), 0, 0), LLVMConstInt(LLVMInt32Type(), 0, 0) };
            LLVMValueRef fmt_ptr = LLVMBuildGEP2(codegen->builder, element_type, fmt_str, indices, 2, "fmtptr");

            LLVMValueRef args[] = { fmt_ptr, val };
            return LLVMBuildCall2(codegen->builder, get_printf_type(), printf_func, args, 2, "printcall");
        }
    }
    return NULL;
}

void codegen_finish(Codegen* codegen, const char* output_file) {
    char* error = NULL;
    if (LLVMPrintModuleToFile(codegen->module, output_file, &error)) {
        fprintf(stderr, "Error writing IR to file: %s\n", error);
        LLVMDisposeMessage(error);
    }
}

void codegen_cleanup(Codegen* codegen) {
    LLVMDisposeBuilder(codegen->builder);
    LLVMDisposeModule(codegen->module);
}
