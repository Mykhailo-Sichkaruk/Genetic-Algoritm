#ifndef MACHINE_H
#define MACHINE_H

#include <iostream>
#include <time.h>
#include <string.h>
#include <Math.h>
#include <stdlib.h>
#include "./../const.h"
// #include "machine.cpp"

const int REGISTER_LEN = 8; // Number of bits in a register
typedef bool instrunction[REGISTER_LEN];
typedef bool memoryPointer[REGISTER_LEN - 2];

class Machine8bit
{
protected:
    // Number of instructions of the machine
    static const int MEMORY_SIZE = 64;
    // Memory of the machine = MEMORY_SIZE * 8 bit instructions
    bool memory[MEMORY_SIZE][REGISTER_LEN];

    // Output of the machine - moves
    int currentMove;

    // Index of the current instruction
    int programCounter;
    int getAdress(instrunction cmd);
    bool *goToAddress(instrunction cmd);
    void binaryIncrement(bool *address);
    void binaryDecrement(bool *address);
    void increment(memoryPointer memory);
    void decrement(memoryPointer memory);
    char print(memoryPointer memory);
    void jump(memoryPointer memory);
    void getCurrentInstruction();

public:
    Machine8bit();
    Machine8bit(bool *memoryArray);
    // Run single instrunction and return move if there is one or ' ' if there is not
    char runInstruction(instrunction instrunction);
    bool *getMemory();
    // Run all instructions and return moves string
    std::string run(int instructionCount);
};
#endif