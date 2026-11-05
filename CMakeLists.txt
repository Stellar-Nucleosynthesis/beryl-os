cmake_minimum_required(VERSION 3.20)

set(CMAKE_ASM_NASM_OBJECT_FORMAT elf32)

project(beryl
    LANGUAGES C ASM_NASM
)

find_program(GENISOIMAGE genisoimage REQUIRED)
find_program(QEMU qemu-system-i386 REQUIRED)

add_subdirectory(init)
add_subdirectory(kernel)
add_subdirectory(arch)

add_executable(kernel_image
    $<TARGET_OBJECTS:init>
    $<TARGET_OBJECTS:kernel>
    $<TARGET_OBJECTS:arch>
)

set_target_properties(kernel_image PROPERTIES
    OUTPUT_NAME kernel
    SUFFIX .elf
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
)

target_link_libraries(kernel_image PRIVATE init_linker)

set(ISO_DIR "${CMAKE_BINARY_DIR}/iso")
set(ISO_IMAGE "${CMAKE_BINARY_DIR}/os.iso")

file(GLOB GRUB_FILES CONFIGURE_DEPENDS
    "${CMAKE_SOURCE_DIR}/grub/*"
)

add_custom_command(
    OUTPUT "${ISO_IMAGE}"

    COMMAND ${CMAKE_COMMAND} -E rm -rf "${ISO_DIR}"

    COMMAND ${CMAKE_COMMAND} -E make_directory
        "${ISO_DIR}/boot/grub"

    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/grub"
        "${ISO_DIR}/boot/grub"

    COMMAND ${CMAKE_COMMAND} -E copy
        "$<TARGET_FILE:kernel_image>"
        "${ISO_DIR}/boot/kernel.elf"

    COMMAND ${GENISOIMAGE}
        -R
        -b boot/grub/stage2_eltorito
        -no-emul-boot
        -boot-load-size 4
        -A os
        -input-charset utf8
        -quiet
        -boot-info-table
        -o "${ISO_IMAGE}"
        iso

    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"

    DEPENDS
        kernel_image
        ${GRUB_FILES}

    VERBATIM
)

add_custom_target(iso ALL
    DEPENDS "${ISO_IMAGE}"
)

add_custom_target(run
    COMMAND ${QEMU}
        -drive format=raw,file=${ISO_IMAGE},media=cdrom
        -no-reboot
        -boot d

    DEPENDS iso
    USES_TERMINAL
)

add_custom_target(debug
    COMMAND ${QEMU}
        -drive format=raw,file=${ISO_IMAGE},media=cdrom
        -no-reboot
        -boot d
        -s
        -S

    DEPENDS iso
    USES_TERMINAL
)