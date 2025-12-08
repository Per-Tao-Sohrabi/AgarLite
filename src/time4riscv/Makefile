SRC_DIR ?= ./
OBJ_DIR ?= ./
SOURCES ?= $(shell find $(SRC_DIR) -name '*.c' -or -name '*.S')
OBJECTS ?= $(addsuffix .o, $(basename $(notdir $(SOURCES))))
LINKER ?= $(SRC_DIR)/dtekv-script.lds

TOOLCHAIN ?= riscv32-unknown-elf-
# ----------------------------------------------------------------------
# FINAL FIX: CFLAGS reverted to original to avoid compiler errors.
# The soft-float control is moved to the linking stage.
# ----------------------------------------------------------------------
CFLAGS ?= -Wall -nostartfiles -O3 -mabi=ilp32 -march=rv32im_zicsr
# ----------------------------------------------------------------------


build: clean main.bin

# Compilation Step (creates .o files)
main.elf: 
	$(TOOLCHAIN)gcc -c $(CFLAGS) $(SOURCES)

# Linking Step (creates main.elf)
# FIX: Added -lm to ensure all math dependencies are resolved.
# Order is important: Place libraries (softfloat.a, -lm, -lgcc) at the end.
	$(TOOLCHAIN)gcc $(CFLAGS) -T $(LINKER) -o $@ $(filter-out boot.o, $(OBJECTS)) softfloat.a -lm -lgcc

main.bin: main.elf
	$(TOOLCHAIN)objcopy --output-target binary $< $@
	$(TOOLCHAIN)objdump -D $< > $<.txt

clean:
	rm -f *.o *.elf *.bin *.txt

TOOL_DIR ?= ./tools
run: main.bin
	make -C $(TOOL_DIR) "FILE_TO_RUN=$(CURDIR)/$<"