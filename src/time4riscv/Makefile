SRC_DIR ?= ./
OBJ_DIR ?= ./
SOURCES ?= $(shell find $(SRC_DIR) -name '*.c' -or -name '*.S')
OBJECTS ?= $(addsuffix .o, $(basename $(notdir $(SOURCES))))
LINKER ?= $(SRC_DIR)/dtekv-script.lds

TOOLCHAIN ?= riscv32-unknown-elf-

# ----------------------------------------------------------------------
# THE FIX: Using -mfloat-abi=soft
# This instructs the compiler to generate code that uses soft-float 
# conventions, which is necessary when linking the softfloat.a library.
# The ABI is kept as -mabi=ilp32 to satisfy the rv32im_zicsr hardware check.
# ----------------------------------------------------------------------
CFLAGS ?= -Wall -nostartfiles -O3 -mabi=ilp32 -march=rv32im_zicsr -mfloat-abi=soft 
# ----------------------------------------------------------------------


build: clean main.bin

# Compilation Step (creates .o files)
main.elf: 
	$(TOOLCHAIN)gcc -c $(CFLAGS) $(SOURCES)

# Linking Step (creates main.elf)
# Crucial: The linker line still explicitly links softfloat.a
	$(TOOLCHAIN)gcc $(CFLAGS) -T $(LINKER) -o $@ $(filter-out boot.o, $(OBJECTS)) softfloat.a -lgcc

main.bin: main.elf
	$(TOOLCHAIN)objcopy --output-target binary $< $@
	$(TOOLCHAIN)objdump -D $< > $<.txt

clean:
	rm -f *.o *.elf *.bin *.txt

TOOL_DIR ?= ./tools
run: main.bin
	make -C $(TOOL_DIR) "FILE_TO_RUN=$(CURDIR)/$<"