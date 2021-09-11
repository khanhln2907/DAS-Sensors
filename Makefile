# The name of your project (used to name the compiled .hex file)
TARGET = "Sensor"
#$(notdir $(CURDIR))

# The teensy version to use, 30, 31, 35, 36, or LC
TEENSY = 36

# Set to 24000000, 48000000, or 96000000 to set CPU core speed
TEENSY_CORE_SPEED = 180000000

# Some libraries will require this to be defined
# If you define this, you will break the default main.cpp
#ARDUINO = 10600

# configurable options
OPTIONS = -DUSB_SERIAL -DLAYOUT_US_ENGLISH

# directory to build in
BUILDDIR = $(abspath $(CURDIR)/Build)

#************************************************************************
# Location of Teensyduino utilities, Toolchain, and Arduino Libraries.
# To use this makefile without Arduino, copy the resources from these
# locations and edit the pathnames.  The rest of Arduino is not needed.
#************************************************************************

# path location for Teensy Loader, teensy_post_compile and teensy_reboot
TOOLSPATH = $(CURDIR)/tools

# path location for Teensy 3 core
COREPATH = ../cores_git/teensy3

# path location for  libraries
LIBRARYPATH = ./lib/

# path location for the arm-none-eabi compiler
COMPILERPATH = /usr/share/gcc-arm-none-eabi-10.3-2021.07/bin

#************************************************************************
# Settings below this point usually do not need to be edited
#************************************************************************

# CPPFLAGS = compiler options for C and C++
CPPFLAGS = -Wall -g -Os -mthumb -ffunction-sections -fdata-sections -nostdlib -MMD $(OPTIONS) -DTEENSYDUINO=124 -DF_CPU=$(TEENSY_CORE_SPEED) -Isrc -I$(COREPATH)

# compiler options for C++ only
CXXFLAGS = -std=gnu++0x -felide-constructors -fno-exceptions -fno-rtti

# compiler options for C only
CFLAGS =

# linker options
LDFLAGS = -Os -Wl,--gc-sections -mthumb

# additional libraries to link
LIBS = -lm 

# compiler options specific to teensy version
ifeq ($(TEENSY), 30)
    CPPFLAGS += -D__MK20DX128__ -mcpu=cortex-m4
    LDSCRIPT = $(COREPATH)/mk20dx128.ld
    LDFLAGS += -mcpu=cortex-m4 -T$(LDSCRIPT)
else ifeq ($(TEENSY), 31)
    CPPFLAGS += -D__MK20DX256__ -mcpu=cortex-m4
    LDSCRIPT = $(COREPATH)/mk20dx256.ld
    LDFLAGS += -mcpu=cortex-m4 -T$(LDSCRIPT)
else ifeq ($(TEENSY), LC)
    CPPFLAGS += -D__MKL26Z64__ -mcpu=cortex-m0plus
    LDSCRIPT = $(COREPATH)/mkl26z64.ld
    LDFLAGS += -mcpu=cortex-m0plus -T$(LDSCRIPT)
    LIBS += -larm_cortexM0l_math
else ifeq ($(TEENSY), 35)
    CPPFLAGS += -D__MK64FX512__ -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16
    LDSCRIPT = $(COREPATH)/mk64fx512.ld
    LDFLAGS += -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 -T$(LDSCRIPT)
    LIBS += -larm_cortexM4lf_math
else ifeq ($(TEENSY), 36)
    CPPFLAGS += -D__MK66FX1M0__ -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16
    LDSCRIPT = $(COREPATH)/mk66fx1m0.ld
    LDFLAGS += -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 -T$(LDSCRIPT)
    MCU = mk66fx1m0
    #LIBS += -larm_cortexM4lf_math
else
    $(error Invalid setting for TEENSY)
endif

# set arduino define if given
ifdef ARDUINO
	CPPFLAGS += -DARDUINO=$(ARDUINO)
else
	CPPFLAGS += -DUSING_MAKEFILE
endif

# names for the compiler programs
CC = $(abspath $(COMPILERPATH))/arm-none-eabi-gcc
CXX = $(abspath $(COMPILERPATH))/arm-none-eabi-g++
OBJCOPY = $(abspath $(COMPILERPATH))/arm-none-eabi-objcopy
SIZE = $(abspath $(COMPILERPATH))/arm-none-eabi-size

# automatically create lists of the sources and objects
LC_FILES := $(wildcard $(LIBRARYPATH)/*/*.c)
LCPP_FILES := $(wildcard $(LIBRARYPATH)/*/*.cpp)
TC_FILES := $(wildcard $(COREPATH)/*.c)
TCPP_FILES := $(wildcard $(COREPATH)/*.cpp)
C_FILES := $(wildcard src/*.c)
CPP_FILES := $(wildcard src/*.cpp)
INO_FILES := $(wildcard src/*.ino)

# Dependencies
DEP_FILES_DIR_NAME = Common_git
DEP_FILES_PATH = ../$(DEP_FILES_DIR_NAME)/lib
DEP_FILES_CPP = $(wildcard $(DEP_FILES_PATH)/*/*.cpp)

# Include paths for libraries
L_INC := $(foreach lib,$(filter %/, $(wildcard $(LIBRARYPATH)/*/)), -I$(lib))
L_INC += $(foreach lib,$(filter %/, $(wildcard $(DEP_FILES_PATH)/*/)), -I$(lib)) 

# Object construction for local libraries
SOURCES := $(C_FILES:.c=.o) $(CPP_FILES:.cpp=.o) $(INO_FILES:.ino=.o) $(TC_FILES:.c=.o) $(TCPP_FILES:.cpp=.o) $(LC_FILES:.c=.o) $(LCPP_FILES:.cpp=.o) #$(DEP_FILES_CPP:.cpp=.o)
OBJS := $(foreach src,$(SOURCES), $(BUILDDIR)/$(src))

# Object construction for dependencies

# Teensyduino Commands
UPLOADER = teensy_loader_cli
TEENSY_VERBOSE = -v
# If WAIT_FOR_DEVICE is set, programmer blocks until device is programmable
# (push button).  If not, and no device found, it exits with error.
TEENSY_WAIT_FOR_DEVICE = -w
TEENSY_FLAGS += -mmcu=$(MCU)
TEENSY_FLAGS += $(TEENSY_VERBOSE)
TEENSY_FLAGS += $(TEENSY_WAIT_FOR_DEVICE)


all: hex

build: 
	$(TARGET).elf

hex: $(BUILDDIR)/$(TARGET).hex

flash: $(BUILDDIR)/$(TARGET).hex 
	@echo Flashing...
	$(UPLOADER) $(TEENSY_FLAGS) $<  

$(BUILDDIR)/%.o: %.c
	@echo -e "[CC]\t$<"
	@mkdir -p "$(dir $@)"
	@$(CC) $(CPPFLAGS) $(CFLAGS) $(L_INC) -o "$@" -c "$<"

$(BUILDDIR)/%.o: %.cpp
	@echo -e "[CXX]\t$<"
	@mkdir -p "$(dir $@)"
	@$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(L_INC) -o "$@" -c "$<"

$(BUILDDIR)/%.o: %.ino
	@echo -e "[CXX]\t$<"
	@mkdir -p "$(dir $@)"
	@$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(L_INC) -o "$@" -x c++ -include Arduino.h -c "$<"

$(BUILDDIR)/$(TARGET).elf: $(OBJS) $(LDSCRIPT)
	@echo -e "[LD]\t$@"
	@$(CC) $(LDFLAGS) -o "$@" $(OBJS) $(LIBS) 	

%.hex: %.elf
	@echo -e "[HEX]\t$@"
	@$(SIZE) "$<"
	@$(OBJCOPY) -O ihex -R .eeprom "$<" "$@"

# compiler generated dependency info
-include $(OBJS:.o=.d)

clean:
	@echo Cleaning...
	@rm -rf "$(BUILDDIR)"
	@rm -f "$(TARGET).elf" "$(TARGET).hex"

rebuild:
	make clean
	@rm -rf "$(BUILDDIR)"
	@rm -rf $(filter %/, "$(BUILDDIR))
	@rm -f "$(TARGET).elf" "$(TARGET).hex"

print:
	@echo $(L_INC)
	@echo $(SOURCES)

delpio:
	@echo Cleaning PIO dep...
	@rm -rf ".pio/libdeps"
