/*
 * @file isaprogram.h
 * @author Martin Skovsboel Friis
 * Definition of an RISCV ISA program
 */

#ifndef ISAPROGRAM_H
#define ISAPROGRAM_H


class ISAProgram {
  public:

    ISAProgram();
    ~ISAProgram();

    bool readFromFile(const char * filepath);
    bool writeToFile(const char * filepath);
    void step();

    bool hasNext();
    void printRegisters();
    void printProgram(); //Currenly prints instructions in hex
    void printMemory();

  private:
    unsigned int * buffer_; //Program buffer
    int length_; //Program length
    int pc_; //Program counter
    int registers_[32]; //All 32 registers
    bool ecall_;


    unsigned int capacity_;
    unsigned char * memory_; //

    bool save(int w, unsigned int sp, unsigned int bytes);
    bool load(int &w, unsigned int sp, unsigned int bytes, bool u=false);
    void printAsHex(unsigned int instr);
};

#endif
