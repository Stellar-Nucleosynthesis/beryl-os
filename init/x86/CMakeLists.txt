add_library(init OBJECT
    init.asm
    setup.c
)
add_library(init_linker INTERFACE)

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

target_link_libraries(init PRIVATE
    kernel
    arch
    common
)

set(LINKER_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/link.ld)

target_link_options(init_linker INTERFACE
    -m32
    -nostdlib
    -no-pie
    "LINKER:-T,${LINKER_SCRIPT}"
)