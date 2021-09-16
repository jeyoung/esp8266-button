SDK_HOME = /home/jeyoung/source/github/esp-open-sdk/sdk
CC = $(SDK_HOME)/../xtensa-lx106-elf/bin/xtensa-lx106-elf-gcc
AR = $(SDK_HOME)/../xtensa-lx106-elf/bin/xtensa-lx106-elf-ar
CFLAGS = -I. -I./lib -I$(SDK_HOME)/include -I$(SDK_HOME)/driver_lib/driver -I$(SDK_HOME)/driver_lib/include/driver -mlongcalls -Wall
LDLIBS = -nostdlib -Wl,--start-group -lc -lgcc -ldriver -lmain -lnet80211 -lwpa -llwip -lpp -lphy -lc -lmbedtls -lssl -lcrypto -lwps -Wl,--end-group -lbutton
LDFLAGS = -L. -L$(SDK_HOME)/lib -T$(SDK_HOME)/ld/eagle.app.v6.ld --sysroot=$(SDK_HOME)/ld $(LDLIBS)
PROGRAM = main

ESPTOOL = esptool
BAUDRATE = 921600

.PHONY:
all: clean $(PROGRAM)

$(PROGRAM)-0x00000.bin: $(PROGRAM)
	$(ESPTOOL) elf2image $^

$(PROGRAM): $(PROGRAM).o

$(PROGRAM).o: $(PROGRAM).c libbutton.a

libbutton.a: lib/button.o
	$(AR) ra s $@ $^

lib/button.o: lib/button.c

flash: clean $(PROGRAM)-0x00000.bin
	$(ESPTOOL) -b $(BAUDRATE) write_flash 0x0 $(PROGRAM)-0x00000.bin 0x10000 $(PROGRAM)-0x10000.bin

clean:
	$(RM) -f $(PROGRAM) $(PROGRAM).o $(PROGRAM)-0x00000.bin $(PROGRAM)-0x10000.bin lib/*.o
