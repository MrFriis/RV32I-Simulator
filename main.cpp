/*
* @file main.cpp
* @author Martin S. Friis
* RV32I simulator program
*/

#include <iostream>
#include <string>
#include <fstream>
#include "isaprogram.h"

using namespace std;

int main(int argc, char**argv){

  //Check for input
  if(argc < 2){
    std::cout << "Filepath to binary input file required" << '\n';
    return 0;
  }

  //Get filepath for input file
  const char * filepath = argv[1];

  //Construct simulation object
  ISAProgram myProgram;
  bool read = myProgram.readFromFile(filepath); //Read the input file
  if(!read){
    std::cout << "File not found :(" << '\n';
    return 0;
  }

  //myProgram.printProgram();

  //myProgram.printProgram();

  //Run the program
  while(myProgram.hasNext()){
    myProgram.step();
  }

  //Write register content
  myProgram.printRegisters();

  //Dump the register content to file
  myProgram.writeToFile("out.bin");

  return 0;
}

// Fix memory by combining program buffer and the memory. Store program from mem_0 to mem_[length]
// Load instruction by using load function to get int instruction.
// The auipc will make sense as it will load pc_
// - fix pc_ to be increments of 4 when fixing the memory.


/*
* Todo:
*   - include more instructions branch bltu bgeu & auipc
*   - make ISAProgram a base class, and create inherited class
*     RV32I, which override step function.
*   - make possible to inherit as RV64I (maybe template header for in to long int conversion)
*   - secure robust program - include error in data access fixes
*/
