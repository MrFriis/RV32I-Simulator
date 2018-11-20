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
    void step();
    //bool writeToFile(const char *filepath)

    bool hasNext();
    void printRegisters();
    void printProgram(); //Currenly prints instructions in hex

  private:
    unsigned int * buffer_; //Program buffer
    int length_; //Program length
    int pc_; //Program counter
    int registers_[32]; //All 32 registers

    //Helper methods
    //PrintAsHex
};

#endif
