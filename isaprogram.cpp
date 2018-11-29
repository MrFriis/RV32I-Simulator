/*
* @file isaprogram.cpp
* @author Martin Friis
* Implementation for the isaprogram class
*/

#include <iostream>
#include <fstream>
#include "isaprogram.h"

//ISAProgram Constructor
ISAProgram::ISAProgram(){
  pc_ = 0;
  length_=0;
  memory_ = NULL;
  capacity_ = 0;
  ecall_=false;
  //Init registers
  for(int i=0; i<32; i++){
    registers_[i] = 0;
  }

  //Preallocating memory.
  memory_ = new unsigned char[0x100010];
  capacity_ = 0x100010;

  //Fill memory
  for(unsigned int i = 0; i<capacity_; i++){
    memory_[i] = 0;
  }
}

//Delete main memory
ISAProgram::~ISAProgram(){
  //Implemnt
  if(memory_ != NULL){
    delete [] memory_;
    memory_ = NULL;
  }
}

//Function that read instructions from a binary file to main memory
//@input filepath - the path to search for binary file
//@output - indication if file was found.

bool ISAProgram::readFromFile(const char * filepath){
  FILE *fp;
  unsigned int * buffer;
  long int fileSize;

  fp = fopen(filepath,"rb");
  if(fp==NULL){
    return false;
  }

  fseek(fp, 0, SEEK_END);
  fileSize = ftell(fp);  //size of file in bytes
  fseek(fp, 0, SEEK_SET);

  length_ = fileSize/4;
  buffer = new unsigned int[length_]; //Allocate buffer

  fread(buffer, sizeof(buffer), length_, fp);
  fclose(fp);

  //Save the program to the memory
  for(int i=0; i<length_; i++){
    save(buffer[i], i*4, 4);
  }

  //Reclaim memory
  delete[] buffer;
  buffer=NULL;

  //File was read
  return true;
}

/* Dumps the registry content at current state to file
* @input filepath - where to place file
* @output - indication if write was successful
*/


bool ISAProgram::writeToFile(const char * filepath){
  std::ofstream outfile(filepath, std::ios::binary);
  outfile.write((char*) &registers_, 32*sizeof(int));
  outfile.close();
  return true;
}


/* Step through the program by decoding and executing next instruction pointed to
* by pc_. This function contain the implementation of the RISC-V 32RVI instruction subset
* For different subset/functionality this function must be overwritten.
*/

void ISAProgram::step(){

  //Get fields
  int instr = 0;
  load(instr, 4*pc_, 4);
  printAsHex(instr); //REMOVE
  int opcode = instr & 0x7f;
  int funct3 = (instr>>12) & 0x7;
  int imm = (instr >> 20);
  int rd = (instr >> 7) & 0x1f;
  int rs1 = (instr >> 15) & 0x1f;
  int rs2 = imm & 0x1f;
  int offset; //For program branch

  switch (opcode) {
    case 0x03 : //load Instructions
      switch(funct3) {
        case 0x0 : //lb
          std::cout << "lb x" << rd << " " << imm << "(x" << rs1 << ")" << '\n';
          load(registers_[rd], registers_[rs1]+imm, 1);
          break;
        case 0x1 : //lH
          std::cout << "lh x" << rd << " " << imm << "(x" << rs1 << ")" << '\n';
          load(registers_[rd], registers_[rs1]+imm, 2);
        break;
        case 0x2 : //lW
          std::cout << "lw x" << rd << " " << imm << "(x" << rs1 << ")" << '\n';
          load(registers_[rd], registers_[rs1]+imm, 4);
        break;
        case 0x4 : //lbu
          std::cout << "lbu x" << rd << " " << imm << "(x" << rs1 << ")" << '\n';
          load(registers_[rd], registers_[rs1]+imm, 1, true);
        break;
        case 0x5 : //lhu
          std::cout << "lhu x" << rd << " " << imm << "(x" << rs1 << ")" << '\n';
          load(registers_[rd], registers_[rs1]+imm, 2, true);
        break;
      }
      break;
    case 0x13 :
      switch (funct3) {
        case 0x0:
          std::cout << "addi x" << rd << " x" << rs1 << " " << imm <<  '\n';
          registers_[rd] = registers_[rs1] + imm;
          break;
        case 0x1: //
          std::cout << "slli x" << rd << " x" << rs1 << " " << imm <<  '\n';
          registers_[rd] = registers_[rs1] << imm;
          break;
        case 0x2:
          std::cout << "slti x" << rd << " x" << rs1 << " " << imm <<  '\n';
          if(registers_[rs1] < imm){registers_[rd] = 1;}
          else {registers_[rd] = 0;}
          break;
        case 0x3:
          std::cout << "sltiu x" << rd << " x" << rs1 << " " << (unsigned int) (imm & 0xfff) <<  '\n';
          if(registers_[rs1] < ((unsigned int) (imm & 0xfff) )){registers_[rd] = 1;}
          else {registers_[rd] = 0;}
          break;
        case 0x4:
          std::cout << "xori x" << rd << " x" << rs1 << " " << imm <<  '\n';
          registers_[rd] = registers_[rs1] ^ imm;
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
        case 0x6 :
          std::cout << "ori x" << rd << " x" << rs1 << " " << imm <<  '\n';
          registers_[rd] = registers_[rs1] | imm;
          break;
        case 0x7 :
            std::cout << "andi x" << rd << " x" << rs1 << " " << imm <<  '\n';
            registers_[rd] = registers_[rs1] & imm;
          break;
        } break;
    case 0x17 : //auipc
      imm = ((instr >> 12) << 12);
      std::cout << "auipc x" << rd << " " << imm << '\n';
      registers_[rd] = pc_*4+imm;
      break;
    case 0x23 : //Save Instructions
      imm = (imm >> 5) << 5 | rd; //Get immediate field
      switch (funct3) {
        case 0x0:
          std::cout << "sb x" << rs2 << " " << imm << "(" << "x" << rs1 << ")" << '\n';
          save(registers_[rs2], registers_[rs1]+imm, 1);
          break;
        case 0x1:
          std::cout << "sh x" << rs2 << " " << imm << "(" << "x" << rs1 << ")" << '\n';
          save(registers_[rs2], registers_[rs1]+imm, 2);
          break;
        case 0x2 :
          std::cout << "sw x" << rs2 << " " << imm << "(" << "x" << rs1 << ")" << '\n';
          save(registers_[rs2], registers_[rs1]+imm, 4);
          //std::cout << "Memory saved " << (unsigned int) memory_[registers_[rs1]+imm] << " at address: " << registers_[rs1]+imm << '\n'; //REMOVE
          break;
      } break;
    case 0x33 :
      switch(funct3){
        case 0x0 :
          if((instr >> 25) == 0){
            std::cout << "add x" << rd << " x" << rs1 << " x" << rs2 <<  '\n';
            registers_[rd] = registers_[rs1] + registers_[rs2];
          } else {
            std::cout << "sub x" << rd << " x" << rs1 << " x" << rs2 <<  '\n';
            registers_[rd] = registers_[rs1] - registers_[rs2];
          }
          break;
        case 0x1:
          std::cout << "sll x" << rd << " x" << rs1 << " x" << (registers_[rs2] & 0x1f) <<  '\n';
          registers_[rd] = registers_[rs1] << (registers_[rs2] & 0x1f);
          break;
        case 0x2:
          std::cout << "slt x" << rd << " x" << rs1 << " x" << rs2 <<  '\n';
          if(registers_[rs1] < registers_[rs2]){registers_[rd]=1;}
          else{registers_[rd] = 0;}
          break;
        case 0x3:
          std::cout << "sltu x" << rd << " x" << rs1 << " x" << rs2 <<  '\n';
          if(registers_[rs1] < (unsigned int) registers_[rs2]){registers_[rd]=1;}
          else{registers_[rd] = 0;}
          break;
        case 0x4:
          std::cout << "XOR x" << rd << " x" << rs1 << " x" << rs2 <<  '\n';
          registers_[rd] = registers_[rs1]^registers_[rs2];
          break;
        case 0x5:
          if((instr >> 25) == 0){
            std::cout << "SRL x" << rd << " x" << rs1 << " x" << (registers_[rs2] & 0x1f) <<  '\n';
            registers_[rd] = ((unsigned int) registers_[rs1]) >> (registers_[rs2] & 0x1f);
          } else {
            std::cout << "SRA x" << rd << " x" << rs1 << " x" << (registers_[rs2] & 0x1f) <<  '\n';
            registers_[rd] = registers_[rs1] >> (registers_[rs2] & 0x1f);
          }
          break;
        case 0x6 :
          std::cout << "OR x" << rd << " x" << rs1 << " x" << rs2 <<  '\n';
          registers_[rd] = registers_[rs1] | registers_[rs2];
          break;
        case 0x7 :
          std::cout << "AND x" << rd << " x" << rs1 << " x" << rs2 <<  '\n';
          registers_[rd] = registers_[rs1] & registers_[rs2];
          break;
      }
      break;
    case 0x37 :
      std::cout << "lui x" << rd << " " << ((instr >> 12) & 0xfffff) << '\n';
      registers_[rd] = ((instr >> 12) & 0xfffff) << 12;
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
          if(registers_[rs1]>=registers_[rs2]){pc_ = pc_+(offset >> 2)-1;}
          break;
        case 0x6 :
          std::cout << "bltu x" << rs1 << " x" << rs2 << " " << offset <<  '\n';
          if(registers_[rs1]< (unsigned) registers_[rs2]){pc_ = pc_+(offset/4)-1;}
          break;
        case 0x7 :
          std::cout << "bgeu x" << rs1 << " x" << rs2 << " " << offset <<  '\n';
          if(registers_[rs1]>= (unsigned) registers_[rs2]){pc_ = pc_+(offset >> 2)-1;}
          break;
      } break;
    case 0x67: //jalr
      std::cout << "jalr x" << rd << " x" << rs1 << " " << imm << std::endl;
      registers_[rd] = (pc_+1) << 2;
      pc_ = (registers_[rs1]+imm) >> 2;
      pc_ = pc_-1;
      break;
    case 0x6f: //jal
      //Get imm
      imm = (instr>>30) << 20; //Get instr[20]
      imm = imm | (((instr >> 12) & 0xff) << 12); //Get instr[19:12]
      imm = imm | (((instr >> 20) & 0x1) << 11); //Get instr[11]
      imm = imm | (((instr >> 21) & 0x3ff) << 1); //Get instr[10:1]
      std::cout << "jal x" << rd << " " << imm << std::endl;
      registers_[rd] = (pc_+1) << 2;
      pc_ = pc_+(imm >> 2)-1; //Because of inc after switch
      break;
    case 0x73 :
      std::cout << "Ecall - Exiting program" << '\n';
      ecall_ = true;
      break;
  }
  registers_[0] = 0; //jalr x0 xX is not supposed to cast exception, so this is the easier way
  pc_++; //Increment program counter
}

/* Return indication if program is ended
*/

bool ISAProgram::hasNext(){
  return ((pc_ < length_) & !ecall_);
}

void ISAProgram::printRegisters(){
  std::cout << "Value of registers: \n" << std::endl;
  for(int i=0; i<32; i++){
    std::cout << "x" << i << " " << registers_[i] << std::endl;
  }
}


void ISAProgram::printProgram(){
  std::cout << "Program" << '\n' << std::endl;
  for(int i=0; i<length_; i++){
    int instr;
    load(instr, 4*i, 4);
    printAsHex(instr);
    std::cout << "" << '\n';
  }
}


/*
* Saves 1-4 bytes from the integer w to memory
* Byte ordering is little endian format
* @input w - integer to be saved
* @input idx - memory idx to save least sig. byte to
* @input bytes - number of bytes to save
*/
bool ISAProgram::save(int w, unsigned int idx, unsigned int bytes){
  for(unsigned int i=0; i < bytes; i++){
    memory_[idx+i] = (unsigned char) (w>>(8*i)) & 0xff;
  }
  return true;
}

/* Single function to load from memory. Is
* @input &w - register to place content passed by reference
* @input sp - pointer to location of least significant byte (little endian format)
* @input
*/


bool ISAProgram::load(int &w, unsigned int sp, unsigned int bytes, bool u){
  w = 0;
  for(unsigned int i=0; i<bytes; i++){
    w = w | ((unsigned int) memory_[sp+i]) << (8*i);
  }

  if(u != true){
    if((memory_[sp+bytes-1] >> 7) == 1){ //For sign extension
      for(int i=4; i>bytes; i--){
        w = w | 0xff << (8*(i-1));
      }
    }
  }
  return true;
}

/* Test function used to print content of memory
*
*/

void ISAProgram::printAsHex(unsigned int instr){
  int res = 0;
  std::cout << "0x";
  for(int i = 0; i<8; i++){
    res = (instr >> (4*(7-i))) & 0xf;
    switch(res){
      case 10:
        std::cout << "a"; break;
      case 11:
        std::cout << "b"; break;
      case 12:
        std::cout << "c"; break;
      case 13:
        std::cout << "d"; break;
      case 14:
        std::cout << "e"; break;
      case 15:
        std::cout << "f"; break;
      default :
        std::cout << res; break;
    }
  }
  std::cout << "    ";
}
