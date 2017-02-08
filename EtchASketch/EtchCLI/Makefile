EXENAME = etch
OBJS = main.o libEtchASketch.a

CXX = clang++
CXXFLAGS = -c -Ofast -Wall -std=c++11 -stdlib=libc++ -I../EtchASketch
LD = clang++
LDFLAGS = -std=c++11 -stdlib=libc++

LIB_SRC=$(wildcard ../EtchASketch/*.cpp)

# This generates the list of .o files. The order is hardcoded for now.
LIB_OBJ = Image.o ImageFlow.o BlurImageFilter.o SobelEdgeDetector.o BobAndWeaveSalesman.o Salesman.o EASUtils.o

all : $(EXENAME)

$(EXENAME) : $(OBJS)
	$(LD) $(OBJS) $(LDFLAGS) -o $(EXENAME)

main.o : main.cpp libEtchASketch.a
	$(CXX) $(CXXFLAGS) main.cpp

libEtchASketch.a :
	$(CXX) $(CXXFLAGS) $(LIB_SRC); ar rcs $@ $(LIB_OBJ)

clean :
	-rm -f *.o *.a $(EXENAME)
