EXENAME = etch
OBJS = main.o libEtchASketch.a

CXX = clang++
CXXFLAGS = -c -Ofast -Wall -std=c++11 -stdlib=libc++ -I../EtchASketch
LD = clang++
LDFLAGS = -std=c++11 -stdlib=libc++

LIB_SRC_PTH = ../EtchASketch/
LIB_SRC = $(wildcard $(LIB_SRC_PTH)*.cpp)

# Filter out KDPoint.cpp, KDTree.cpp, and EtchASketch.cpp
LIB_SRC_FILTR = $(LIB_SRC_PTH)KDPoint.cpp $(LIB_SRC_PTH)KDTree.cpp
LIB_SRC_FILTR += $(LIB_SRC_PTH)EtchASketch.cpp
LIB_SRC := $(filter-out $(LIB_SRC_FILTR), $(LIB_SRC))
LIB_SRC += $(LIB_SRC_PTH)KDPoint.hpp $(LIB_SRC_PTH)KDTree.hpp

all : $(EXENAME)

$(EXENAME) : $(OBJS)
	$(LD) $(OBJS) $(LDFLAGS) -o $(EXENAME)

main.o : main.cpp libEtchASketch.a
	$(CXX) $(CXXFLAGS) main.cpp

libEtchASketch.a :
	$(CXX) $(CXXFLAGS) $(LIB_SRC); ar rcs $@ *.o

clean :
	-rm -f *.o *.a $(EXENAME)
