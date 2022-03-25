TARGET = main

# Linker script
LD_SCRIPT = STM32L476RG.ld

# Chip architecture
MCU = cortex-m4

# Tools
CC = arm-none-eabi-gcc
AS = arm-none-eabi-as
LD = arm-none-eabi-ld
OBJCOPY = arm-none-eabi-objcopy
OBJDUMP = arm-none-eabi-objdump
OBJSIZE = arm-none-eabi-size

# Assembly directives
AS_FLAGS += -c
AS_FLAGS += -O0
AS_FLAGS += -mcpu=$(MCU)
AS_FLAGS += -mthumb
AS_FLAGS += -Wall
# (Set error messages to appear on a single line.)
AS_FLAGS += -fmessage-length=0

# C compilation directives
C_FLAGS += -mcpu=$(MCU)
C_FLAGS += -mthumb
C_FLAGS += -Wall
C_FLAGS += -g
# (Set error messages to appear on a single line.)
C_FLAGS += -fmessage-length=0
# (Set system to ignore semihosted junk)
C_FLAGS += --specs=nosys.specs

# Linker directives
L_SCRIPT = ./$(LD_SCRIPT)
L_FLAGS += -mcpu=$(MCU)
L_FLAGS += -mthumb
L_FLAGS += -Wall
L_FLAGS += --specs=nosys.specs
L_FLAGS += -nostdlib
L_FLAGS += -lgcc
L_FLAGS += -T$(L_SCRIPT)

# Code
SRC_DIR  = ./Src
AS_SRC   = startup.s
C_SRC    = main.c

# Objects
OBJS_DIR  = ./Obj
OBJS = $(AS_SRC:%.s=%.o)
OBJS += $(C_SRC:%.c=%.o)


.PHONY: all
all: $(TARGET).bin

%.o: $(SRC_DIR)/%.s
	$(CC) -x assembler-with-cpp $(AS_FLAGS) ./$< -o $(OBJS_DIR)/$@

%.o: $(SRC_DIR)/%.c
	$(CC) -c $(C_FLAGS) $(INCLUDE) ./$< -o $(OBJS_DIR)/$@

$(TARGET).elf: $(OBJS)
	$(CC) $(addprefix $(OBJS_DIR)/, $^) $(L_FLAGS) -o $@

$(TARGET).bin: $(TARGET).elf
	$(OBJCOPY) -S -O binary $< $@
	$(OBJSIZE) $<

.PHONY: clean
clean:
	pushd $(OBJS_DIR); rm -f $(OBJS); popd
	rm -f $(TARGET).elf $(TARGET).bin
