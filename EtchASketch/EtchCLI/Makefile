UNAME_S := $(shell uname -s)

EXENAME = etch
OBJS = main.o libEtchASketch.a motor.o MotorController.o
ifeq ($(UNAME_S),Linux)
	OBJS += wiringPiWrapper.o
endif

MOTORUTILS =
ifeq ($(UNAME_S),Linux)
	MOTORUTILS += motor-left motor-right motor-up motor-down
endif

LIBUNWIND = /usr/lib/arm-linux-gnueabihf/libunwind.so.8

LIB_SRC_PTH = ../EtchASketch/
LIB_SRC = $(wildcard $(LIB_SRC_PTH)*.cpp)
ifeq ($(UNAME_S),Linux)
	HDR_PTHS := $(addprefix -I,$(shell find /usr/include/c++/ -type d -print))
endif

CXX = clang++
CXXFLAGS = -c -g -O0 -Wall -std=c++11 -stdlib=libc++ -I$(LIB_SRC_PTH)
CC = clang
CCFLAGS = -c -g -O0 -Wall -I$(LIB_SRC_PTH) -I./dummySystemIncludes/
LD = clang++
LDFLAGS = -std=c++11 -stdlib=libc++
ifeq ($(UNAME_S),Linux)
	LDFLAGS += -lwiringPi
endif
LD_OBJS = $(OBJS)
ifeq ($(UNAME_S),Linux)
	LD_OBJS += $(LIBUNWIND)
endif

MOTOR_FILES = motor.c libEtchASketch.a
ifneq ($(UNAME_S),Linux)
	MOTOR_FILES += wiringPiWrapper.o
endif

# Filter out KDPoint.cpp, KDTree.cpp, EtchASketch.cpp, motor.c and MotorController.cpp
LIB_SRC_FILTR = $(LIB_SRC_PTH)KDPoint.cpp $(LIB_SRC_PTH)KDTree.cpp
LIB_SRC_FILTR += $(LIB_SRC_PTH)EtchASketch.cpp
LIB_SRC_FILTR += motor.c
LIB_SRC_FILTR += MotorController.cpp
LIB_SRC := $(filter-out $(LIB_SRC_FILTR), $(LIB_SRC))
LIB_SRC += $(LIB_SRC_PTH)KDPoint.hpp $(LIB_SRC_PTH)KDTree.hpp

all : $(EXENAME) motor-utils

$(EXENAME) : $(OBJS)
	$(LD) $(LD_OBJS) $(LDFLAGS) -o $(EXENAME)

main.o : main.cpp libEtchASketch.a motor.o MotorController.o
	$(CXX) $(CXXFLAGS) main.cpp

# TODO: get rid of the libEtchASketch.a "dependency" for these two (the build breaks if the build order is reversed)
motor.o : $(MOTOR_FILES)
	$(CC) $(CCFLAGS) $^

wiringPiWrapper.o: ./dummySystemIncludes/wiringPiWrapper.c
	$(CC) $(CCFLAGS) $^

MotorController.o: MotorController.cpp libEtchASketch.a
	$(CXX) $(CXXFLAGS) $^
	
libEtchASketch.a :
	$(CXX) $(CXXFLAGS) $(LIB_SRC)
	ar rcs $@ *.o

motor-utils : $(MOTORUTILS)

MOTORUTILS_DEPS = motor-main.c motor.o $(LIBUNWIND)

motor-left : $(MOTORUTILS_DEPS)
	$(CC) $(CCFLAGS) -o $@ $^ -DMOTOR_MOVE_LEFT=1

motor-right : $(MOTORUTILS_DEPS)
	$(CC) $(CCFLAGS) -o $@ $^ -DMOTOR_MOVE_RIGHT=1

motor-up : $(MOTORUTILS_DEPS)
	$(CC) $(CCFLAGS) -o $@ $^ -DMOTOR_MOVE_UP=1

motor-down : $(MOTORUTILS_DEPS)
	$(CC) $(CCFLAGS) -o $@ $^ -DMOTOR_MOVE_DOWN=1

.PHONY: clean motor-utils

clean :
	-rm -f *.o *.a $(EXENAME) $(MOTORUTILS)

