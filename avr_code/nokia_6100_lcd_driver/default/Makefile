###############################################################################
# Makefile for the project nokia_6100_lcd_driver
###############################################################################

## General Flags
PROJECT = nokia_6100_lcd_driver
MCU = atmega32u4
TARGET = nokia_6100_lcd_driver.elf
CC = avr-gcc

CPP = avr-g++

## Options common to compile, link and assembly rules
COMMON = -mmcu=$(MCU)

## Compile options common for all C compilation units.
CFLAGS = $(COMMON)
CFLAGS += -Wall -gdwarf-2 -Os -std=gnu99 -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums  -DEPSON 
CFLAGS += -MD -MP -MT $(*F).o -MF dep/$(@F).d 

## Assembly specific flags
ASMFLAGS = $(COMMON)
ASMFLAGS += $(CFLAGS)
ASMFLAGS += -x assembler-with-cpp -Wa,-gdwarf2

## Linker flags
LDFLAGS = $(COMMON)
LDFLAGS +=  -Wl,-Map=nokia_6100_lcd_driver.map


## Intel Hex file production flags
HEX_FLASH_FLAGS = -R .eeprom -R .fuse -R .lock -R .signature

HEX_EEPROM_FLAGS = -j .eeprom
HEX_EEPROM_FLAGS += --set-section-flags=.eeprom="alloc,load"
HEX_EEPROM_FLAGS += --change-section-lma .eeprom=0 --no-change-warnings


## Objects that must be built in order to link
OBJECTS = demo_globe.o nokialcd_driver.o nokialcd_font.o nokialcd_scroll.o sysconfig-atmega168.o 

## Objects explicitly added by the user
LINKONLYOBJECTS = 

## Build
all: $(TARGET) nokia_6100_lcd_driver.hex nokia_6100_lcd_driver.eep nokia_6100_lcd_driver.lss size

## Compile
demo_globe.o: ../demo_globe.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

nokialcd_driver.o: ../nokialcd_driver.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

nokialcd_font.o: ../nokialcd_font.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

nokialcd_scroll.o: ../nokialcd_scroll.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

sysconfig-atmega168.o: ../sysconfig-atmega168.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

##Link
$(TARGET): $(OBJECTS)
	 $(CC) $(LDFLAGS) $(OBJECTS) $(LINKONLYOBJECTS) $(LIBDIRS) $(LIBS) -o $(TARGET)

%.hex: $(TARGET)
	avr-objcopy -O ihex $(HEX_FLASH_FLAGS)  $< $@

%.eep: $(TARGET)
	-avr-objcopy $(HEX_EEPROM_FLAGS) -O ihex $< $@ || exit 0

%.lss: $(TARGET)
	avr-objdump -h -S $< > $@

size: ${TARGET}
	@echo
	@avr-size -C --mcu=${MCU} ${TARGET}

## Clean target
.PHONY: clean
clean:
	-rm -rf $(OBJECTS) nokia_6100_lcd_driver.elf dep/* nokia_6100_lcd_driver.hex nokia_6100_lcd_driver.eep nokia_6100_lcd_driver.lss nokia_6100_lcd_driver.map


## Other dependencies
-include $(shell mkdir dep 2>NUL) $(wildcard dep/*)

