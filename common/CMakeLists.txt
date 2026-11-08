add_library(common INTERFACE)

target_compile_options(common INTERFACE
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

target_include_directories(common INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)