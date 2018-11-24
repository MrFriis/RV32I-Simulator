
#include <iostream>
#include <fstream>
#include "isaprogram.h"


ISAProgram::ISAProgram(){
  pc_ = 0;
  buffer_ = NULL;
  //Init registers
  for(int i=0; i<32; i++){
    registers_[i] = 0;
  }
}

//DELETE buffer_
ISAProgram::~ISAProgram(){
  //Implemnt
  if(buffer_ != NULL){
      delete[] buffer_;
      buffer_ = NULL;
  }
}

bool ISAProgram::readFromFile(const char * filepath){
  FILE *fp;
  long int fileSize;

  fp = fopen(filepath,"rb");
  if(fp==NULL){
    return false;
  }

  fseek(fp, 0, SEEK_END);
  fileSize = ftell(fp);  //size of file in bytes
  fseek(fp, 0, SEEK_SET);

  length_ = fileSize/4;
  buffer_ = new unsigned int[length_];

  fread(buffer_, sizeof(buffer_), length_, fp);
  fclose(fp);

  return true;
}

bool ISAProgram::writeToFile(const char * filepath){
  std::ofstream outfile(filepath, std::ios::binary);
  outfile.write((char*) &registers_, 32*sizeof(int));
  outfile.close();
  return true;
}


/*
* Function to step through program
*/

void ISAProgram::step(){

  //Get fields
  int instr = *(buffer_+pc_);
  int opcode = instr & 0x7f;
  int funct3 = (instr>>12) & 0x7;
  int imm = (instr >> 20);
  int rd = (instr >> 7) & 0x1f;
  int rs1 = (instr >> 15) & 0x1f;
  int rs2 = imm & 0x1f;
  int offset; //For program branch

  switch (opcode) {
    case 0x37 :
      std::cout << "lui x" << rd << " " << (instr >> 12) << '\n';
      registers_[rd] = (instr >> 12) << 12;
      break;
    case 0x13 :
      switch (funct3) {
        case 0x0:
          std::cout << "addi x" << rd << " x" << rs1 << " " << imm <<  '\n';
          registers_[rd] = registers_[rs1] + imm; break;
        case 0x1: //
          std::cout << "slli x" << rd << " x" << rs1 << " " << imm <<  '\n';
          registers_[rd] = registers_[rs1] << imm;
          break;
        case 0x5 :
          if(imm & 0xf00 == 0){
            std::cout << "slri x" << rd << " x" << rs1 << " " << imm <<  '\n';
            registers_[rd] = registers_[rs1] >> imm;
          } else {
            std::cout << "srai x" << rd << " x" << rs1 << " " << (imm & 0x1f) <<  '\n';
            registers_[rd] = registers_[rs1] >> (imm & 0x1f);
          }
          break;
        } break;
    case 0x33 :
      std::cout << "add x" << rd << " x" << rs1 << " x" << rs2 <<  '\n';
      registers_[rd] = registers_[rs1] + registers_[rs2];
      break;
    case 0x63 : //Branch case
      offset = ((instr>>25)<<5)+((instr>>7)& 0x1f)-1;
      if(offset>0){offset++;}
      switch(funct3){
        case 0x0 :
          std::cout << "beq x" << rs1 << " x" << rs2 << " " << offset <<  '\n';
          if(registers_[rs1]==registers_[rs2]){pc_ = pc_+(offset/4)-1;}
          break;
        case 0x1:
          std::cout << "bne x" << rs1 << " x" << rs2 << " " << offset <<  '\n';
          if(registers_[rs1]!=registers_[rs2]){pc_ = pc_+(offset/4)-1;}
          break;
        case 0x4:
          std::cout << "blt x" << rs1 << " x" << rs2 << " " << offset <<  '\n';
          if(registers_[rs1]<registers_[rs2]){pc_ = pc_+(offset/4)-1;}
          break;
        case 0x5 :
          std::cout << "bge x" << rs1 << " x" << rs2 << " " << offset <<  '\n';
          if(registers_[rs1]>=registers_[rs2]){pc_ = pc_+(offset/4)-1;}
          break;
      } break;
    case 0x73 :
      std::cout << "Ecall - Exiting program" << '\n';
      break;
  }

  pc_++; //Increment program counter
}

bool ISAProgram::hasNext(){
  return pc_ < length_? true : false;
}

void ISAProgram::printRegisters(){
  std::cout << "Value of registers: \n" << std::endl;
  for(int i=0; i<32; i++){
    std::cout << "x" << i << " " << registers_[i] << std::endl;
  }
}


/*
*
*/

void ISAProgram::printProgram(){
  std::cout << "Program" << '\n' << std::endl;
  for(int i=0; i<length_; i++){
    std::cout << *(buffer_+i) << '\n';
  }
}
