add_library(arch OBJECT
    gdt/gdt.c
    gdt/load_gdt.asm

    paging/paging.c
    paging/paging_utils.asm

    memory/page_frame_alloc.c
    memory/page_frame_mapper.c

    memory/virt_page_alloc.c

    memory/alloc_memory.c
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

target_link_libraries(arch PRIVATE common hal)

target_include_directories(arch PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)