# 32 bit RISCV simulator

## Compiling the programs

The compilation and assembling of the program is simplified by the make build
automation tool. Simply call the following instruction in main program folder
```
make all
```
The compilation and linkage procedure is specified in the `Makefile` file.
Use the cmd
```
make clean
```
to clean the main folder for all executables and object files

## Input and output
The program executable is named sim. It requires the filepath to a binary file.
To run the program type
```
./sim tests/addlarge.bin
```
The input must be a binary file of compiled RISCV 32 ISA code.
At termination the program will output the state of all 32 registers. 
