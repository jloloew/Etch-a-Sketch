EXENAME = etch
OBJS = main.o libEtchASketch.a

CXX = clang++
CXXFLAGS = -c -Ofast -I../EtchASketch
LD = clang++
LDFLAGS =

all : $(EXENAME)

$(EXENAME) : $(OBJS)
	$(LD) $(OBJS) $(LDFLAGS) -o $(EXENAME)

main.o : main.cpp libEtchASketch.a
	$(CXX) $(CXXFLAGS) main.cpp

libEtchASketch.a : 
	echo "Currently linking against precompiled libEtchASketch.a"

clean :
	-rm -f *.o $(EXENAME)
