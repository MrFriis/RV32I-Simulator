/*
 * @file RV32ISimulator.h
 * @author Martin Skovsboel Friis
 * Definition of an RISCV ISA program
 */

#ifndef RV32ISIMULATOR_H
#define RV32ISIMULATOR_H


class RV32ISimulator {
  public:

    RV32ISimulator();
    ~RV32ISimulator();

    bool readFromFile(const char * filepath);
    bool writeToFile(const char * filepath);

    void step();
    bool hasNext();

    void printRegisters();
    void printProgram();

  private:
    int length_; //Program length in number of instructions
    int pc_; //Program counter
    int registers_[32]; //
    bool ecall_; //For program termination

    //capacity currently not used
    unsigned int capacity_; //Memory capacity
    unsigned char * memory_; //Main memory

    //Internal functions
    bool save(int w, unsigned int idx, unsigned int bytes);
    bool load(int &w, unsigned int sp, unsigned int bytes, bool u=false);
    void printAsHex(unsigned int instr);
};

#endif
