NASM       	:= nasm
LD         	:= ld
GENISO		:= genisoimage
QEMU       	:= qemu-system-i386

BUILD_DIR 	:= build
ISO_DIR   	:= $(BUILD_DIR)/iso

LOADER_OBJ 	:= $(BUILD_DIR)/loader.o
KERNEL_ELF 	:= $(BUILD_DIR)/kernel.elf
ISO_IMAGE  	:= $(BUILD_DIR)/os.iso

.PHONY: all iso run debug clean

all: iso run

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(LOADER_OBJ): loader.asm | $(BUILD_DIR)
	$(NASM) -f elf32 $< -o $@

$(KERNEL_ELF): $(LOADER_OBJ) link.ld
	$(LD) -T link.ld -melf_i386 $(LOADER_OBJ) -o $@

$(ISO_IMAGE): $(KERNEL_ELF) $(wildcard grub/*)
	rm -rf $(ISO_DIR)
	mkdir -p $(ISO_DIR)/boot/grub
	cp -a grub/. $(ISO_DIR)/boot/grub/
	cp $(KERNEL_ELF) $(ISO_DIR)/boot/kernel.elf
	cd $(BUILD_DIR) && $(GENISO) \
		-R \
		-b boot/grub/stage2_eltorito \
		-no-emul-boot \
		-boot-load-size 4 \
		-A os \
		-input-charset utf8 \
		-quiet \
		-boot-info-table \
		-o os.iso iso

iso: $(ISO_IMAGE)

run: $(ISO_IMAGE)
	$(QEMU) -drive format=raw,file=$(ISO_IMAGE),media=cdrom \
		-boot d

debug: $(ISO_IMAGE)
	$(QEMU) -drive format=raw,file=$(ISO_IMAGE),media=cdrom \
		-boot d \
		-s -S

clean:
	rm -rf $(BUILD_DIR)
