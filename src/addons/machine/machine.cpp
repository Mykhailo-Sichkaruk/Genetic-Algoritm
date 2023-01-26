#include <iostream>
#include <time.h>
#include <string.h>
#include <Math.h>
#include <stdlib.h>
#include "../const.h"
#include "machine.h"

int Machine8bit::getAdress(instrunction cmd)
{
    int adress = 0;
    for (int i = REGISTER_LEN - 1; i >= 2; i--)
    {
        adress += cmd[i] * pow(2, REGISTER_LEN - i - 1);
    }
    return adress;
}

bool *Machine8bit::goToAddress(instrunction cmd)
{
    int address = getAdress(cmd);
    return memory[address];
}

void Machine8bit::binaryIncrement(bool *address)
{
    address = address + 8;
    for (int i = REGISTER_LEN - 1; i >= 0; i--)
    {
        if (!address[i])
        {
            address[i] = bool(1);
            break;
        }
        else
        {
            address[i] = bool(0);
        }
    }
}

void Machine8bit::binaryDecrement(bool *address)
{
    for (int i = REGISTER_LEN - 1; i >= 0; i--)
    {
        if (address[i])
        {
            address[i] = bool(0);
            break;
        }
        else
        {
            address[i] = bool(1);
        }
    }
}

// Run instruction and return move if there is one or ' ' if there is not
char Machine8bit::runInstruction(instrunction instrunction)
{
    char move = ' ';
    if (instrunction[0] == 0 && instrunction[1] == 0)
    {
        increment(instrunction);
    }
    else if (instrunction[0] == 0 && instrunction[1] == 1)
    {
        decrement(instrunction);
    }
    else if (instrunction[0] == 1 && instrunction[1] == 0)
    {
        jump(instrunction);
    }
    else if (instrunction[0] == 1 && instrunction[1] == 1)
    {
        move = print(instrunction);
    }

    if (this->programCounter >= this->MEMORY_SIZE - 1)
    {
        this->programCounter = 0;
    }
    else
    {
        this->programCounter++;
    }

    return move;
}

void Machine8bit::increment(memoryPointer memory)
{
    this->binaryIncrement(this->goToAddress(memory));
}

void Machine8bit::decrement(memoryPointer memory)
{
    this->binaryDecrement(this->goToAddress(memory));
}

char Machine8bit::print(memoryPointer memory)
{
    char move;
    if (memory[REGISTER_LEN - 2] == 0 && memory[REGISTER_LEN - 1] == 0)
        move = LEFT;
    else if (memory[REGISTER_LEN - 2] == 0 && memory[REGISTER_LEN - 1] == 1)
        move = RIGHT;
    else if (memory[REGISTER_LEN - 2] == 1 && memory[REGISTER_LEN - 1] == 0)
        move = UP;
    else if (memory[REGISTER_LEN - 2] == 1 && memory[REGISTER_LEN - 1] == 1)
        move = DOWN;

    return move;
}

void Machine8bit::jump(memoryPointer memory)
{
    this->programCounter = getAdress(memory);
}

Machine8bit::Machine8bit() {}

Machine8bit::Machine8bit(bool *memoryArray)
{
    for (int i = 0; i < MEMORY_SIZE; i++)
    {
        for (int j = 0; j < REGISTER_LEN; j++)
        {
            memory[i][j] = memoryArray[i * REGISTER_LEN + j];
        }
    }
}

std::string Machine8bit::run(int instructionCount)
{
    std::string moves = "";
    char move = ' ';
    for (int i = 0; i < instructionCount; i++)
    {
        move = this->runInstruction(this->memory[this->programCounter]);
        if (move != ' ')
        {
            moves += move;
        }
    }

    return moves;
}
