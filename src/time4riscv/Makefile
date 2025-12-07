SRC_DIR ?= ./
OBJ_DIR ?= ./
# Find all .c and .S files recursively in SRC_DIR
SOURCES ?= $(shell find $(SRC_DIR) -name '*.c' -or -name '*.S')
# Convert all source names to object names (e.g., main.c -> main.o)
OBJECTS = $(SOURCES:.c=.o)
OBJECTS = $(OBJECTS:.S=.o)

LINKER ?= $(SRC_DIR)/dtekv-script.lds

TOOLCHAIN ?= riscv32-unknown-elf-
# Add -lm to LDFLAGS to link the Math Library
LDFLAGS = -lm
CFLAGS ?= -Wall -nostdlib -O3 -mabi=ilp32 -march=rv32imzicsr -fno-builtin


.PHONY: all build clean run

build: clean main.bin

# --- Linking Rule ---
# Use the $(TOOLCHAIN)gcc driver for linking. This driver automatically searches
# for and links libc (for printf) and, with -lm, libm (for sqrt).
main.elf: $(OBJECTS)
	$(TOOLCHAIN)gcc $(OBJECTS) softfloat.a $(LDFLAGS) -T $(LINKER) -o $@

# --- Compilation Rule for .c files ---
# Standard pattern to compile C source files into object files
%.o: %.c
	$(TOOLCHAIN)gcc -c $(CFLAGS) $< -o $@

# --- Compilation Rule for .S files (Assembly) ---
# Ensure assembly files are also compiled correctly
%.o: %.S
	$(TOOLCHAIN)gcc -c $(CFLAGS) $< -o $@

# --- Bin/Dump Rules ---
main.bin: main.elf
	$(TOOLCHAIN)objcopy --output-target binary $< $@
	$(TOOLCHAIN)objdump -D $< > $<.txt

clean:
	rm -f *.o *.elf *.bin *.txt

TOOL_DIR ?= ./tools
run: main.bin
	make -C $(TOOL_DIR) "FILE_TO_RUN=$(CURDIR)/$<"