EXENAME = etch
OBJS = main.o libEtchASketch.a

LIB_SRC_PTH = ../EtchASketch/
LIB_SRC = $(wildcard $(LIB_SRC_PTH)*.cpp)
HDR_PTHS := $(addprefix -I,$(shell find /usr/include/c++/ -type d -print))

CXX = clang++
CXXFLAGS = -c -O0 -Wall -std=c++11 -stdlib=libc++ -I$(LIB_SRC_PTH)
LD = clang++
LDFLAGS = -std=c++11 -stdlib=libc++

# Filter out KDPoint.cpp, KDTree.cpp, and EtchASketch.cpp
LIB_SRC_FILTR = $(LIB_SRC_PTH)KDPoint.cpp $(LIB_SRC_PTH)KDTree.cpp
LIB_SRC_FILTR += $(LIB_SRC_PTH)EtchASketch.cpp
LIB_SRC := $(filter-out $(LIB_SRC_FILTR), $(LIB_SRC))
LIB_SRC += $(LIB_SRC_PTH)KDPoint.hpp $(LIB_SRC_PTH)KDTree.hpp

all : $(EXENAME)

$(EXENAME) : $(OBJS)
	$(LD) $(OBJS) /usr/lib/arm-linux-gnueabihf/libunwind.so.8 $(LDFLAGS) -o $(EXENAME)

main.o : main.cpp libEtchASketch.a
	$(CXX) $(CXXFLAGS) main.cpp

libEtchASketch.a :
	$(CXX) $(CXXFLAGS) $(LIB_SRC)
	ar rcs $@ *.o

.PHONY: clean

clean :
	-rm -f *.o *.a $(EXENAME)

