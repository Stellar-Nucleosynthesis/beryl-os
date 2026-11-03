add_library(init OBJECT
    init.asm
    setup.c
)

target_compile_options(init PRIVATE
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

target_link_libraries(init PRIVATE kernel)