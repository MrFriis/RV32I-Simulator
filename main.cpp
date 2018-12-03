/*
* @file main.cpp
* @author Martin S. Friis
* RV32I simulator program
*/

#include <iostream>
#include <string>
#include <fstream>
#include "RV32ISimulator.h"

using namespace std;

int main(int argc, char**argv){

  //Check for input
  if(argc < 2){
    std::cout << "Filepath to binary input file required" << '\n';
    return 0;
  }

  //Get filepath for input file
  const char * filepath = argv[1];

  //Construct simulator object
  RV32ISimulator myProgram;
  bool read = myProgram.readFromFile(filepath); //Read the input file
  if(!read){
    std::cout << "File not found :(" << '\n';
    return 0;
  }

  //Run the program
  while(myProgram.hasNext()){
    myProgram.step();
  }

  //Perhaps get name of output file
  const char * outFile;
  if(argc != 2){
    outFile = argv[2];
  } else {
      outFile = "out.res";
  }
  
  //Dump the register content to file
  myProgram.writeToFile(outFile);


  std::cout << '\n' << "Register content dumped in file: " << outFile << '\n';

  return 0;
}
