SRC_DIR ?= ./
OBJ_DIR ?= ./
SOURCES ?= $(shell find $(SRC_DIR) -name '*.c' -or -name '*.S')
OBJECTS ?= $(addsuffix .o, $(basename $(notdir $(SOURCES))))
LINKER ?= $(SRC_DIR)/dtekv-script.lds

TOOLCHAIN ?= riscv32-unknown-elf-
# ----------------------------------------------------------------------
# THE FIX: Added -msoft-float to force software floating-point emulation
# ----------------------------------------------------------------------
CFLAGS ?= -Wall -nostartfiles -O3 -mabi=ilp32 -march=rv32im_zicsr -msoft-float
# ----------------------------------------------------------------------


build: clean main.bin

# Compilation Step (creates .o files)
main.elf: 
	$(TOOLCHAIN)gcc -c $(CFLAGS) $(SOURCES)

# Linking Step (creates main.elf)
	$(TOOLCHAIN)gcc $(CFLAGS) -T $(LINKER) -o $@ $(filter-out boot.o, $(OBJECTS)) softfloat.a -lgcc

main.bin: main.elf
	$(TOOLCHAIN)objcopy --output-target binary $< $@
	$(TOOLCHAIN)objdump -D $< > $<.txt

clean:
	rm -f *.o *.elf *.bin *.txt

TOOL_DIR ?= ./tools
run: main.bin
	make -C $(TOOL_DIR) "FILE_TO_RUN=$(CURDIR)/$<"