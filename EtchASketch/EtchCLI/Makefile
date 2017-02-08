EXENAME = etch
OBJS = main.o libEtchASketch.a

CXX = clang++
CXXFLAGS = -c -Ofast -Wall -std=c++11 -stdlib=libc++ -I../EtchASketch
LD = clang++
LDFLAGS = -std=c++11 -stdlib=libc++

all : $(EXENAME)

$(EXENAME) : $(OBJS)
	$(LD) $(OBJS) $(LDFLAGS) -o $(EXENAME)

main.o : main.cpp libEtchASketch.a
	$(CXX) $(CXXFLAGS) main.cpp

libEtchASketch.a : Image.o ImageFlow.o BlurImageFilter.o SobelEdgeDetector.o BobAndWeaveSalesman.o Salesman.o EASUtils.o
	ar rcs $@ $^

Image.o : ../EtchASketch/Image.cpp ../EtchASketch/Image.hpp
	$(CXX) $(CXXFLAGS) ../EtchASketch/Image.cpp

ImageFlow.o : ../EtchASketch/ImageFlow.cpp ../EtchASketch/ImageFlow.hpp
	$(CXX) $(CXXFLAGS) ../EtchASketch/ImageFlow.cpp

BlurImageFilter.o : ../EtchASketch/BlurImageFilter.cpp ../EtchASketch/BlurImageFilter.hpp
	$(CXX) $(CXXFLAGS) ../EtchASketch/BlurImageFilter.cpp

SobelEdgeDetector.o : ../EtchASketch/SobelEdgeDetector.cpp ../EtchASketch/SobelEdgeDetector.hpp
	$(CXX) $(CXXFLAGS) ../EtchASketch/SobelEdgeDetector.cpp

BobAndWeaveSalesman.o : ../EtchASketch/BobAndWeaveSalesman.cpp ../EtchASketch/BobAndWeaveSalesman.hpp
	$(CXX) $(CXXFLAGS) ../EtchASketch/BobAndWeaveSalesman.cpp

Salesman.o : ../EtchASketch/Salesman.cpp ../EtchASketch/Salesman.hpp
	$(CXX) $(CXXFLAGS) ../EtchASketch/Salesman.cpp

EASUtils.o : ../EtchASketch/EASUtils.cpp ../EtchASketch/EASUtils.hpp
	$(CXX) $(CXXFLAGS) ../EtchASketch/EASUtils.cpp

clean :
	-rm -f *.o *.a $(EXENAME)
