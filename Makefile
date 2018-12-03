EXENAME = sim
OBJS = main.o rv32isimulator.o

CXX = g++
CXXFLAGS = -c
LD = g++



all: $(EXENAME)

$(EXENAME) : $(OBJS)
	$(LD) $(OBJS) -o $(EXENAME)

main.o : main.cpp RV32ISimulator.h
	$(CXX) $(CXXFLAGS) main.cpp -o main.o

rv32isimulator.o : RV32ISimulator.cpp RV32ISimulator.h
	$(CXX) $(CXXFLAGS) RV32ISimulator.cpp -o rv32isimulator.o

clean:
	rm *.o $(EXENAME)
