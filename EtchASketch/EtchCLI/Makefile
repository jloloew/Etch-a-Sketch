EXENAME = etch
OBJS = main.o libEtchASketch.a motor.o MotorController.o

LIB_SRC_PTH = ../EtchASketch/
LIB_SRC = $(wildcard $(LIB_SRC_PTH)*.cpp)
HDR_PTHS := $(addprefix -I,$(shell find /usr/include/c++/ -type d -print))

CXX = clang++
CXXFLAGS = -c -g -O0 -Wall -std=c++11 -stdlib=libc++ -I$(LIB_SRC_PTH)
CC = clang
CCFLAGS = -c -g -O0 -Wall -I$(LIB_SRC_PTH)
LD = clang++
LDFLAGS = -std=c++11 -stdlib=libc++ -lwiringPi

# Filter out KDPoint.cpp, KDTree.cpp, EtchASketch.cpp, motor.c and MotorController.cpp
LIB_SRC_FILTR = $(LIB_SRC_PTH)KDPoint.cpp $(LIB_SRC_PTH)KDTree.cpp
LIB_SRC_FILTR += $(LIB_SRC_PTH)EtchASketch.cpp
LIB_SRC_FILTR += motor.c
LIB_SRC_FILTR += MotorController.cpp
LIB_SRC := $(filter-out $(LIB_SRC_FILTR), $(LIB_SRC))
LIB_SRC += $(LIB_SRC_PTH)KDPoint.hpp $(LIB_SRC_PTH)KDTree.hpp

all : $(EXENAME)

$(EXENAME) : $(OBJS)
	$(LD) $(OBJS) /usr/lib/arm-linux-gnueabihf/libunwind.so.8 $(LDFLAGS) -o $(EXENAME)

main.o : main.cpp libEtchASketch.a motor.o MotorController.o
	$(CXX) $(CXXFLAGS) main.cpp

# TODO: get rid of the libEtchASketch.a "dependency" for these two (the build breaks if the build order is reversed)
motor.o : motor.c libEtchASketch.a
	$(CC) $(CCFLAGS) $^

MotorController.o: MotorController.cpp libEtchASketch.a
	$(CXX) $(CXXFLAGS) $^
	
libEtchASketch.a :
	$(CXX) $(CXXFLAGS) $(LIB_SRC)
	ar rcs $@ *.o

.PHONY: clean

clean :
	-rm -f *.o *.a $(EXENAME)

