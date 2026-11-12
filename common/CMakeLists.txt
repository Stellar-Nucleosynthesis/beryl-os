add_library(common OBJECT
    error/panic.c
)

target_compile_options(common PRIVATE
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

target_include_directories(common PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)