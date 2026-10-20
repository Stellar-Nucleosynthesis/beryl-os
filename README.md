# beryl-os

A simple x86 OS development project.

## Requirements

- **GNU Make** - build automation
- **NASM** - assembler
- **GCC** - compiler
- **GNU binutils (`ld`)** - linker
- **genisoimage** - ISO image generation
- **QEMU (`qemu-system-i386`)** - x86 emulator
- **GDB** - optional, for kernel debugging

On Debian or Ubuntu, install the dependencies with:

```bash
sudo apt install make nasm gcc binutils genisoimage qemu-system-x86 gdb
```

## Building and running

Build the bootable ISO image and run it in QEMU:

```bash
make
```

This is equivalent to:

```bash
make iso run
```

To simply build the ISO image:

```bash
make iso
```

To run the image in QEMU:

```bash
make run
```

## Debugging

Start QEMU with the GDB debugging server enabled:

```bash
make debug
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
make clean
```

This deletes the `build/` directory.

## Make targets

| Target | Description                      |
|---|----------------------------------|
| `make` | Build the ISO image and boot it  |
| `make iso` | Build the ISO image              |
| `make run` | Launch QEMU normally             |
| `make debug` | Launch QEMU paused for GDB       |
| `make clean` | Remove generated build artifacts |
