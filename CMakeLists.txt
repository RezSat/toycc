cmake_minimum_required(VERSION 3.10)
project(toycc C)

set(CMAKE_C_STANDARD 99)

# LLVM integration via DLL
set(LLVM_ROOT "C:/CompInfra/clang_llvm")
include_directories("${LLVM_ROOT}/include")

add_executable(toycc 
    src/main.c 
    src/lexer.c
    src/parser.c
    src/codegen.c
)

# Link against the LLVM-C import library
target_link_libraries(toycc PRIVATE "${LLVM_ROOT}/lib/LLVM-C.lib")

target_include_directories(toycc PRIVATE src)
