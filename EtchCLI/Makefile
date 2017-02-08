EXENAME = etch
OBJS = main.o

CXX = clang++
CXXFLAGS = -c
LD = clang++
LDFLAGS = -std=c++1y

all : $(EXENAME)

$(EXENAME) : $(OBJS)
	$(LD) $(OBJS) $(LDFLAGS) -o $(EXENAME)

main.o : main.cpp
	$(CXX) $(CXXFLAGS) main.cpp

clean :
	-rm -f *.o $(EXENAME)
