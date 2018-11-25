//main.cpp

#include <iostream>
#include <string>
#include <fstream>
#include "isaprogram.h"

using namespace std;

int main(int argc, char**argv){

  if(argc < 2){
    std::cout << "Filepath to binary input file required" << '\n';
    return 0;
  }

  const char * filepath = argv[1];

  ISAProgram myProgram;
  bool read = myProgram.readFromFile(filepath);
  if(!read){
    std::cout << "File not found :(" << '\n';
    return 0;
  }

  myProgram.printProgram();



  while(myProgram.hasNext()){
    myProgram.step();
  }
  myProgram.printRegisters();
  //myProgram.writeToFile("out.bin");

  return 0;
}

/*
* Todo:
*   - fix pc offset algortihm for branchinstructions
*   - include save load instructions (need to simulate stack)
      (stack is FILO data structure)
*   - include more instructions branch bltu bgeu & auipc
*   - implement get and set methods
*   - implement a write to file method
*   - make ISAProgram a base class, and create inherited class
*     RV32I, which override step function.
*   - make possible to inherit as RV64I (maybe template header for in to long int conversion)
*   - secure robust program - include error in data access fixes
*   - make program terminate on 'ecall'
*/
