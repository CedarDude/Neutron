PROJDIR := ./src
BUILD_PATH := ./build

CC := x86_64-w64-mingw32-gcc

CFLAGS := -ffreestanding \
          -fno-stack-protector \
          -fno-stack-check \
          -fshort-wchar \
          -mno-red-zone \
          -Wall -Wextra \
          -std=c11

INCLUDES := $(shell find $(PROJDIR) -type d)
CFLAGS += $(patsubst %,-I%,$(INCLUDES))
CFLAGS += -Iedk2/MdePkg/Include
CFLAGS += -Iedk2/MdePkg/Include/X64
CFLAGS += -Iedk2/MdePkg/Include/Protocol
CFLAGS += -Iedk2/MdePkg/Include/IndustryStandard

SRC := $(shell find $(PROJDIR) -name "*.c")
OBJ := $(patsubst $(PROJDIR)/%.c,$(BUILD_PATH)/%.o,$(SRC))

TARGET := $(BUILD_PATH)/Neutron.efi

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) -nostdlib \
	      -Wl,--subsystem,10 \
	      -Wl,--entry,efi_main \
	      -o $@ $^

$(BUILD_PATH)/%.o: $(PROJDIR)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@



clean:
	rm -rf $(BUILD_PATH)
	

EFI:
	mkdir -p build/iso/EFI/BOOT
	cp build/Neutron.efi build/iso/EFI/BOOT/BOOTX64.EFI

	dd if=/dev/zero of=build/esp.img bs=1M count=20
	mkfs.vfat build/esp.img

	mkdir -p build/mnt
	sudo mount build/esp.img build/mnt

	sudo mkdir -p build/mnt/EFI/BOOT
	sudo cp build/Neutron.efi build/mnt/EFI/BOOT/BOOTX64.EFI

	sudo umount build/mnt

	xorriso -as mkisofs \
		-R \
		-f \
		-e esp.img \
		-no-emul-boot \
		-o Neutron.iso \
		build

	rm -rf build/mnt

re: clean all