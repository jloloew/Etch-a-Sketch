EXENAME = etcher
OBJS = png.o rgbapixel.o image.o

CXX = clang++
CXXFLAGS = -std=c++1y -stdlib=libc++ -c -g -O0 -Wall -Wextra -pedantic
LD = clang++
LDFLAGS = -std=c++1y -stdlib=libc++ -lpng -lc++abi

all : etcher

etcher : main.o image.o png.o rgbapixel.o
	$(LD) main.o image.o png.o rgbapixel.o $(LDFLAGS) -o $(EXENAME)

main.o : image.o main.cpp
	$(CXX) $(CXXFLAGS) main.cpp

image.o : png.o image.cpp image.h
	$(CXX) $(CXXFLAGS) image.cpp

png.o : png.cpp png.h rgbapixel.o
	$(CXX) $(CXXFLAGS) png.cpp

rgbapixel.o : rgbapixel.cpp rgbapixel.h
	$(CXX) $(CXXFLAGS) rgbapixel.cpp

clean :
	-rm -f *.o $(EXENAME)
