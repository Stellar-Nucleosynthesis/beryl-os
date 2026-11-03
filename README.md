# beryl-os

A simple x86 OS development project.

## Requirements

- **CMake** - build system
- **NASM** - assembler
- **GCC** - compiler
- **GNU binutils (`ld`)** - linker
- **genisoimage** - ISO image generation
- **QEMU (`qemu-system-i386`)** - x86 emulator
- **GDB** - optional, for kernel debugging

On Debian or Ubuntu, install the dependencies with:

```bash
sudo apt install cmake nasm gcc binutils genisoimage qemu-system-x86 gdb
```

## Building and running

Configure and build the project:

```bash
cmake -S . -B build
cmake --build build
```

This builds the bootable ISO image.

To run the image in QEMU:

```bash
cmake --build build --target run
```

## Debugging

Start QEMU with the GDB debugging server enabled:

```bash
cmake --build build --target debug
```

This starts QEMU with two options:

- `-s` enables the GDB server on TCP port `1234`.
- `-S` pauses the virtual CPU until a debugger resumes execution.

In another terminal, start GDB:

```bash
gdb
```

Connect to QEMU:

```gdb
target remote :1234
```

## Cleaning

Remove generated build artifacts:

```bash
cmake --build build --target clean
```

This removes generated build files but keeps the CMake configuration.

## Build targets

| Target | Description |
|---|---|
| `cmake --build build` | Build the ISO image |
| `cmake --build build --target init` | Build the init module |
| `cmake --build build --target kernel` | Build the kernel module |
| `cmake --build build --target kernel_image` | Link the kernel |
| `cmake --build build --target run` | Launch QEMU normally |
| `cmake --build build --target debug` | Launch QEMU paused for GDB |
| `cmake --build build --target clean` | Remove generated build artifacts |