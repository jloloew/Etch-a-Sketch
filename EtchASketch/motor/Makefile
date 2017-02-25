EXENAME = testMotor
OBJS = main.o motor.o

LIB_SRC_PTH = ../EtchCLI/
LIB_SRC = $(wildcard $(LIB_SRC_PTH)*.cpp)
HDR_PTHS := $(addprefix -I,$(shell find /usr/include/c++/ -type d -print))

CC = clang
CCFLAGS = -c -O0 -Wall -I$(LIB_SRC_PTH)
LD = clang
LDFLAGS = -lwiringPi

all : $(EXENAME)

$(EXENAME) : $(OBJS)
	$(LD) $(OBJS) $(LDFLAGS) -o $(EXENAME)

main.o : main.c motor.o
	$(CC) $(CCFLAGS) $^

motor.o : $(LIB_SRC_PTH)motor.c
	$(CC) $(CCFLAGS) $^

.PHONY: clean

clean :
	-rm -f *.o *.a $(EXENAME)

