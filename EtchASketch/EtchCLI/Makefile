EXENAME = etch
OBJS = main.o libEtchASketch.a

CXX = clang++
CXXFLAGS = -c -Ofast -Wall -std=c++11 -stdlib=libc++ -I../EtchASketch
LD = clang++
LDFLAGS = -std=c++11 -stdlib=libc++

LIB_SRC_PTH = ../EtchASketch/
LIB_SRC = $(wildcard $(LIB_SRC_PTH)*.cpp)

# Replace KDPoint.cpp and KDTree.cpp from src list
# with KDPoint.hpp and KDTree.hpp.
LIB_SRC := $(filter-out $(LIB_SRC_PTH)KDPoint.cpp $(LIB_SRC_PTH)KDTree.cpp, $(LIB_SRC))
LIB_SRC += $(LIB_SRC_PTH)KDPoint.hpp $(LIB_SRC_PTH)KDTree.hpp

# Generate the ordered list of LIB .o files.
LIB_OBJ = Image.o ImageFlow.o BlurImageFilter.o SobelEdgeDetector.o
LIB_OBJ += BobAndWeaveSalesman.o Salesman.o EASUtils.o

all : $(EXENAME)

$(EXENAME) : $(OBJS)
	$(LD) $(OBJS) $(LDFLAGS) -o $(EXENAME)

main.o : main.cpp libEtchASketch.a
	$(CXX) $(CXXFLAGS) main.cpp

libEtchASketch.a :
	$(CXX) $(CXXFLAGS) $(LIB_SRC); ar rcs $@ $(LIB_OBJ)

clean :
	-rm -f *.o *.a $(EXENAME)
