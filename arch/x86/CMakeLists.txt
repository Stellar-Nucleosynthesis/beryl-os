add_library(arch OBJECT
    gdt/gdt.c
    gdt/load_gdt.asm
)

target_compile_options(arch PRIVATE
    $<$<COMPILE_LANGUAGE:C>:
        -m32
        -ffreestanding
        -nostdinc
        -fno-builtin
        -fno-stack-protector
        -Wall
        -Wextra
        -Werror
    >
)

target_include_directories(arch PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)