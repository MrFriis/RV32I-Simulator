EXENAME = sim
OBJS = main.o isaprogram.o

CXX = g++
CXXFLAGS = -c
LD = g++



all: $(EXENAME)

$(EXENAME) : $(OBJS)
	$(LD) $(OBJS) -o $(EXENAME)

main.o : main.cpp isaprogram.h
	$(CXX) $(CXXFLAGS) main.cpp -o main.o

isaprogram.o : isaprogram.cpp isaprogram.h
	$(CXX) $(CXXFLAGS) isaprogram.cpp -o isaprogram.o

clean:
	rm *.o $(EXENAME)
