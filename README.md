# 32 bit RISCV simulator

The program has been developed as part of an assignment for the DTU Course 02155 - Computer Architecture and Engineering.
Original assignment can be found at https://github.com/schoeberl/cae-lab . The test cases found in 'tests' folder has been obtained from here.

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

### Input
The program executable is named sim. It requires the filepath to a binary file.
To run the program type
```
./sim tests/addlarge.bin
```
The input must be a binary file of compiled RISCV 32 ISA code.

### Output

At termination the program makes a binary dump of the register content in file named 'out.res'. The encoding is little endian.
