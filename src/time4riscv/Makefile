SRC_DIR ?= ./
OBJ_DIR ?= ./
SOURCES ?= $(shell find $(SRC_DIR) -name '*.c' -or -name '*.S')
OBJECTS ?= $(addsuffix .o, $(basename $(notdir $(SOURCES))))
LINKER ?= $(SRC_DIR)/dtekv-script.lds

TOOLCHAIN ?= riscv32-unknown-elf-

# ----------------------------------------------------------------------
# FINAL FIX: Environment Control
# -nostdlib: Completely disables standard libraries, eliminating hard-float dependencies.
# -ffreestanding: Declares this as a bare-metal environment.
# -O1: Reduces optimization aggression to prevent substitution of C code with hard-float assembly.
# ----------------------------------------------------------------------
CFLAGS ?= -Wall -nostdlib -ffreestanding -O1 -mabi=ilp32 -march=rv32im_zicsr
# ----------------------------------------------------------------------


build: clean main.bin

# Compilation Step (creates .o files)
main.elf: 
	$(TOOLCHAIN)gcc -c $(CFLAGS) $(SOURCES)

# Linking Step (creates main.elf)
# We rely solely on softfloat.a and -lgcc to provide necessary math routines.
	$(TOOLCHAIN)gcc $(CFLAGS) -T $(LINKER) -o $@ $(filter-out boot.o, $(OBJECTS)) softfloat.a -lgcc

main.bin: main.elf
	$(TOOLCHAIN)objcopy --output-target binary $< $@
	$(TOOLCHAIN)objdump -D $< > $<.txt

clean:
	rm -f *.o *.elf *.bin *.txt

TOOL_DIR ?= ./tools
run: main.bin
	make -C $(TOOL_DIR) "FILE_TO_RUN=$(CURDIR)/$<"