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
			-lMemoryAllocationLib \
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
	@printf "\a\a\a"
	rm -rf $(BUILD_PATH)
	rm -f Neutron.iso

efi: all
	@echo Make Sure To Have Mtools Installed!
	@echo -e "\033[32mDo Not Open Another Razzle Window t'ill it finished!\033[0m"
	mkdir -p build/iso/EFI/BOOT
	cp build/Neutron.efi build/iso/EFI/BOOT/BOOTX64.EFI
	cp src/hello.txt build/iso/EFI/BOOT/HELLO.TXT

	dd if=/dev/zero of=build/esp.img bs=1M count=20
	mkfs.vfat build/esp.img

	mmd -i build/esp.img ::/EFI
	mmd -i build/esp.img ::/EFI/BOOT
	mcopy -i build/esp.img build/Neutron.efi ::/EFI/BOOT/BOOTX64.EFI
	mcopy -i build/esp.img src/hello.txt ::/EFI/BOOT/HELLO.TXT	
	
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
uciso:
	$(MAKE) clean
	$(MAKE) efi
	@echo ""
	@echo ""
	@echo ""
	@echo " _   _            _                    "
	@echo "| \ | | ___ _   _| |_ _ __ ___  _ __  "
	@echo "|  \| |/ _ \ | | | __| '__/ _ \| '_ \ "
	@echo "| |\  |  __/ |_| | |_| | | (_) | | | |"
	@echo "|_| \_|\___|\__,_|\__|_|  \___/|_| |_|"
	@echo ""
	@echo "    ____        _ __    __         "
	@echo "   / __ )__  __(_) /___/ /__  _____"
	@echo "  / __  / / / / / / __  / _ \/ ___/"
	@echo " / /_/ / /_/ / / / /_/ /  __/ /    "
	@echo "/_____/\__,_/_/_/\__,_|\___/_/     "
	@echo ""
	@printf "\033[32mDo Not Open Another Razzle Window t'ill it finished!\033[0m\n"
	@echo ""
	@echo "!!!!!! WARNING FOR LINUX/WSL USERS !!!!"
	@echo "!!!!!! You May NOT Have UCRT, SO Have AN OVMF Prepared!"
	@echo "!!!!!! END !!!!"
	@echo ""
	@printf "\033[32mDo Not Open Another Razzle Window t'ill it finished!\033[0m\n"
	@echo ""
	$(MAKE) ucrt
	

re:
	$(MAKE) clean
	$(MAKE) efi

.PHONY: all clean efi ucrt uciso re