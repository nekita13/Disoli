TARGET		:= DiscirdGuiTest
TITLE		:= VPAD00022
SOURCES		:= scr
INCLUDES	:= include

LIBS = -lvita2d -lSceDisplay_stub -lSceGxm_stub -lSceAppMgr_stub \
	-lSceSysmodule_stub -lSceCtrl_stub -lScePgf_stub \
	-lScePvf_stub -lSceTouch_stub \
	-lSceCommonDialog_stub -lfreetype -lpng -ljpeg -lz -lm -lc
	
CFILES   := $(foreach dir,$(SOURCES), $(wildcard $(dir)/*.c))
CPPFILES   := $(foreach dir,$(SOURCES), $(wildcard $(dir)/*.cpp))
BINFILES := $(foreach dir,$(DATA), $(wildcard $(dir)/*.bin))
OBJS     := $(addsuffix .o,$(BINFILES)) $(CFILES:.c=.o) $(CPPFILES:.cpp=.o) 

PREFIX  = arm-vita-eabi
CC      = $(PREFIX)-gcc
CXX      = $(PREFIX)-g++
CFLAGS  = -Wl,-q -O3 -Wall
CXXFLAGS  = $(CFLAGS) -fno-exceptions -std=gnu++11
ASFLAGS = $(CFLAGS)

all: $(TARGET).vpk

$(TARGET).vpk: $(TARGET).velf
	
	vita-make-fself -s -c $< eboot.bin
	vita-mksfoex -s TITLE_ID=$(TITLE) "$(TARGET)" param.sfo
	cp -f param.sfo sce_sys/param.sfo
	vita-pack-vpk -s sce_sys/param.sfo -b eboot.bin \
				  
				  
	
%.velf: %.elf
	$(PREFIX)-strip -g $<
	vita-elf-create $< $@

$(TARGET).elf: $(OBJS)
	$(CXX) $(CXXFLAGS) $^ $(LIBS) -o $@

clean:
	@rm -rf $(TARGET).velf $(TARGET).elf $(OBJS)
