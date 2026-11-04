add_library(kernel OBJECT
    kmain.c
)

target_compile_options(kernel PRIVATE
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

target_include_directories(kernel PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)