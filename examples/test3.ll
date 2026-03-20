; ModuleID = 'toycc_module'
source_filename = "toycc_module"

@fmt = constant [4 x i8] c"%d\0A\00"

define i32 @main() {
entry:
  %printcall = call i32 (ptr, ...) @printf(ptr @fmt, i32 9)
  ret i32 0
}

declare i32 @printf(ptr, ...)
