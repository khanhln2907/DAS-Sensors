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
BUILDDIR = $(abspath $(CURDIR)/build/)

#************************************************************************
# Location of Teensyduino utilities, Toolchain, and Arduino Libraries.
# To use this makefile without Arduino, copy the resources from these
# locations and edit the pathnames.  The rest of Arduino is not needed.
#************************************************************************

ifeq ($(QPCPP),)
QPCPP := ../qpcpp_git
endif

COMMON_DIR := ../Common_git/lib
TEENSY_CORE_PATH := ../cores_git/teensy3

# QP port used in this project
QP_PORT_DIR := $(QPCPP)/ports/arm-cm/qv/gnu

# list of all source directories used by this project
VPATH = \
	.. \
	../.. \
	$(QPCPP)/src/qf \
	$(QPCPP)/src/qv \
	$(QPCPP)/src/qs \
	$(QP_PORT_DIR) \

# list of all include directories needed by this project
QP_INCLUDES  = \
	-I../.. \
	-I$(QPCPP)/include \
	-I$(QPCPP)/src \
	-I$(QP_PORT_DIR) \

#-----------------------------------------------------------------------------
# files
#

QP_SRCS := \
	qep_hsm.cpp \
	qep_msm.cpp \
	qf_act.cpp \
	qf_actq.cpp \
	qf_defer.cpp \
	qf_dyn.cpp \
	qf_mem.cpp \
	qf_ps.cpp \
	qf_qact.cpp \
	qf_qeq.cpp \
	qf_qmact.cpp \
	qf_time.cpp \
	qv.cpp \
	qv_port.cpp

QP_ASMS :=

QS_SRCS := \
	qs.cpp \
	qs_rx.cpp \
	qs_fp.cpp

# defines
DEFINES   :=

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
LC_FILES := $(subst ../,,$(wildcard $(LIBRARYPATH)/*/*.c))
LCPP_FILES := $(subst ../,,$(wildcard $(LIBRARYPATH)/*/*.cpp))
TC_FILES := $(subst ../,,$(wildcard $(COREPATH)/*.c))
TCPP_FILES := $(subst ../,,$(wildcard $(COREPATH)/*.cpp))
C_FILES := $(subst ../,,$(wildcard src/*.c))  
CPP_FILES := $(subst ../,,$(wildcard src/*.cpp)) 
INO_FILES := $(subst ../,,$(wildcard src/*.c))  

# C++ source files
DEP_INCLUDES := $(foreach header, $(filter %/, $(wildcard $(COMMON_DIR)/*/)), -I$(header))
DEP_SRCS := $(subst ../,,$(wildcard $(COMMON_DIR)/*/*.cpp))
#DEP_SRCS := $(wildcard $(COMMON_DIR)/*/*.cpp)

# Include paths for libraries
L_INC := $(foreach lib,$(filter %/, $(wildcard $(LIBRARYPATH)/*/)), -I$(lib))
L_INC += $(QP_INCLUDES) $(DEP_INCLUDES)

# Sources path of local libraries
SOURCES := $(C_FILES:.c=.o) $(CPP_FILES:.cpp=.o) $(INO_FILES:.ino=.o) $(TC_FILES:.c=.o) $(TCPP_FILES:.cpp=.o) $(LC_FILES:.c=.o) $(LCPP_FILES:.cpp=.o) #$(DEP_FILES_CPP:.cpp=.o)

# Source paths of dependencies
SOURCES += $(QP_SRCS:.cpp=.o) $(DEP_SRCS:.cpp=.o)


# Object construction for dependencies
OBJS := $(foreach src,$(SOURCES), $(BUILDDIR)/$(src))
#OBJS += $(foreach src,$(SOURCES), $(BUILDDIR)/Dependencies/$(src))

# Teensyduino Commands
UPLOADER = teensy_loader_cli
TEENSY_VERBOSE = -v
# If WAIT_FOR_DEVICE is set, programmer blocks until device is programmable
# (push button).  If not, and no device found, it exits with error.
TEENSY_WAIT_FOR_DEVICE = -w
TEENSY_FLAGS += -mmcu=$(MCU)
TEENSY_FLAGS += $(TEENSY_VERBOSE)
TEENSY_FLAGS += $(TEENSY_WAIT_FOR_DEVICE)

BIN_PATH = $(BUILDDIR)/bin

all: hex

build: 
	$(BIN_PATH)/$(TARGET).elf

hex: $(BIN_PATH)/$(TARGET).hex

flash: $(BIN_PATH)/$(TARGET).hex 
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

$(BIN_PATH)/$(TARGET).elf: $(OBJS) $(LDSCRIPT)
	@echo -e "[LD]\t$@"
	@if [ ! -d $(BIN_PATH) ]; then \
   		mkdir $(BIN_PATH); \
	fi
	cd $(BIN_PATH)
	@$(CXX) $(LDFLAGS) -o "$@" $(OBJS) $(LIBS) 	

$(BIN_PATH)/$(TARGET).hex: $(BIN_PATH)/$(TARGET).elf
	@echo -e "[HEX]\t$@"
	@$(SIZE) "$<"
	@$(OBJCOPY) -O ihex -R .eeprom "$<" "$@"

# compiler generated dependency info
-include $(OBJS:.o=.d)

clean:
	@echo Cleaning...
	@rm -rf "$(BUILDDIR)"
	@rm -f "$(TARGET).elf" "$(TARGET).hex"
	@if [ -d $(BIN_PATH) ]; then \
   		rm -f $(BIN_PATH); \
	fi
	

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
