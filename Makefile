# Makefile for PIC18F43K22 Temperature Controller
# Requires XC8 compiler: https://www.microchip.com/mplab/compilers

MCU = 18F43K22
TARGET = atc
SRCS = src/main.c src/lcd.c src/adc.c src/keypd.c
HEX = $(TARGET).hex

# Check if XC8 is installed
XC8 := $(shell which xc8-cc 2>/dev/null)

# Compiler flags (if XC8 available)
CFLAGS = -mcpu=$(MCU) -O2 --warn=-3 --chip=$(MCU)

.PHONY: all clean program help

# Default: check if compiler exists
all:
	@if [ -z "$(XC8)" ]; then \
		echo "Error: XC8 compiler not found"; \
		echo ""; \
		echo "Options:"; \
		echo "  1. Install XC8: https://www.microchip.com/mplab/compilers"; \
		echo "  2. Use MPLAB X IDE (recommended)"; \
		echo ""; \
		exit 1; \
	else \
		$(MAKE) build; \
	fi

# Build hex file (only if XC8 available)
build: $(SRCS)
	xc8-cc $(CFLAGS) -o$(TARGET) $(SRCS)
	@echo ""
	@echo "✓ Build complete: $(HEX)"
	@echo "  Program with: make program"

# Clean build artifacts
clean:
	@rm -f $(TARGET).* *.p1 *.d *.pre *.sym *.cmf *.cof *.lst *.rlf *.sdb *.obj
	@rm -rf build dist funclist *.o .generated_files
	@echo "✓ Clean complete"

# Program PIC (requires PICkit and hex file)
program:
	@if [ ! -f "$(HEX)" ]; then \
		echo "Error: $(HEX) not found. Run 'make' first"; \
		exit 1; \
	fi
	@echo "Programming $(MCU)..."
	@pk2cmd -P$(MCU) -F$(HEX) -M -R || \
		echo "Note: Requires PICkit programmer connected"

# Show help
help:
	@echo "Makefile for PIC18F43K22 Temperature Controller"
	@echo ""
	@echo "Targets:"
	@echo "  make         - Build hex file (requires XC8)"
	@echo "  make clean   - Remove build files"
	@echo "  make program - Flash to PIC (requires PICkit)"
	@echo ""
	@echo "Requirements:"
	@echo "  XC8 compiler: https://www.microchip.com/mplab/compilers"
	@echo "  MPLAB X IDE: https://www.microchip.com/mplab/mplab-x-ide"
	@echo ""
	@echo "Without XC8:"
	@echo "  Use MPLAB X IDE for compilation (recommended)"
