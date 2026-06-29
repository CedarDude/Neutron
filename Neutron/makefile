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

RAW_DIRS := . .. $(PROJDIR) $(PROJDIR)/* $(PROJDIR)/*/* $(PROJDIR)/*/*/*
SRC_DIRS := $(sort $(filter-out %*,$(foreach dir,$(RAW_DIRS),$(wildcard $(dir)/))))

SRC := $(sort $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)*.c)))
OBJ := $(patsubst %.c,$(BUILD_PATH)/%.o,$(notdir $(SRC)))

INCLUDES := $(foreach dir,$(SRC_DIRS),-I$(dir)) \
            -Iedk2/MdePkg/Include \
            -Iedk2/MdePkg/Include/X64 \
            -Iedk2/MdePkg/Include/Library \
            -Iedk2/MdePkg/Include/Protocol \
            -Iedk2/MdePkg/Include/IndustryStandard

CFLAGS += $(INCLUDES)

vpath %.c $(SRC_DIRS)

TARGET := $(BUILD_PATH)/Neutron.efi

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) -nostdlib \
	      -Wl,--subsystem,10 \
	      -Wl,--entry,efi_main \
	      -o $@ $^

$(BUILD_PATH)/%.o: %.c
	mkdir -p $(BUILD_PATH)
	$(CC) $(CFLAGS) -c $< -o $@
	@echo -e "\033[32mDo Not Open Another Razzle Window t'ill it finished!\033[0m"

clean:
	@echo -e "\033[32mDo Not Open Another Razzle Window t'ill it finished!\033[0m"
	rm -rf $(BUILD_PATH)
	rm -f Neutron.iso

efi: all
	@echo Make Sure To Have Mtools Installed!
	@echo -e "\033[32mDo Not Open Another Razzle Window t'ill it finished!\033[0m"
	mkdir -p build/iso/EFI/BOOT
	cp build/Neutron.efi build/iso/EFI/BOOT/BOOTX64.EFI

	dd if=/dev/zero of=build/esp.img bs=1M count=20
	mkfs.vfat build/esp.img

	mmd -i build/esp.img ::/EFI
	mmd -i build/esp.img ::/EFI/BOOT
	mcopy -i build/esp.img build/Neutron.efi ::/EFI/BOOT/BOOTX64.EFI
	@echo -e "\033[32mDo Not Open Another Razzle Window t'ill it finished!\033[0m"
	xorriso -as mkisofs \
	    -R \
	    -f \
	    -e esp.img \
	    -no-emul-boot \
	    -o Neutron.iso \
	    build

ucrt:
	qemu-system-x86_64 -m 1G -drive if=pflash,format=raw,readonly=on,file=C:\msys64\ucrt64\share\qemu\edk2-x86_64-code.fd -cdrom Neutron.iso -boot d

uciso: clean efi
	@echo -e "\033[32mDo Not Open Another Razzle Window t'ill it finished!\033[0m"
	@echo ""
	@echo "!!!!!! WARNING FOR LINUX/WSL USERS !!!!"
	@echo "!!!!!! You May NOT Have UCRT, SO Have AN OVMF Prepared!"
	@echo "!!!!!! END !!!!"
	@echo -e "\033[32mDo Not Open Another Razzle Window t'ill it finished!\033[0m"
	@echo ""
	$(MAKE) ucrt

re: clean efi

.PHONY: all clean efi ucrt uciso re
