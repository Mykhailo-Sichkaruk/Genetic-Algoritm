#include <iostream>
#include <time.h>
#include <string.h>
#include <Math.h>
#include <stdlib.h>

using namespace std;

const int INSTRUCTION_LENGTH = 8; // Number of bits in a register
const int MAX_STEPS = 600;
const int MEM_LENGTH = 64; // Number of bytes in memory
typedef bool instrunction[INSTRUCTION_LENGTH];
typedef bool memoryPointer[INSTRUCTION_LENGTH - 2];

class Machine8bit
{
protected:
    enum Direction
    {
        LEFT = 'L',
        RIGHT = 'R',
        UP = 'U',
        DOWN = 'D',
    };
    // Number of instructions of the machinek
    int MEMORY_SIZE = MEM_LENGTH;
    // Memory of the machine = MEMORY_SIZE * 8 bit instructions
    bool memory[MEM_LENGTH][INSTRUCTION_LENGTH];

    // Output of the machine - moves
    char moves[MAX_STEPS] = "";
    int currentMove = 0;

    // Index of the current instruction
    int programCounter = 0;

    int getAdress(instrunction cmd)
    {
        int adress = 0;
        for (int i = INSTRUCTION_LENGTH - 1; i >= 2; i--)
        {
            adress += cmd[i] * pow(2, INSTRUCTION_LENGTH - i - 1);
        }
        return adress;
    }

    bool *goToAddress(instrunction cmd)
    {
        int address = getAdress(cmd);
        return memory[address];
    }

    void binaryIncrement(bool *address)
    {
        address = address + 8;
        for (int i = INSTRUCTION_LENGTH - 1; i >= 0; i--)
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

    void binaryDecrement(bool *address)
    {
        for (int i = INSTRUCTION_LENGTH - 1; i >= 0; i--)
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

    void runInstruction(instrunction instrunction)
    {
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
            print(instrunction);
        }

        if (this->programCounter >= MEM_LENGTH - 1)
        {
            this->programCounter = 0;
        }
        else
        {
            this->programCounter++;
        }
    }

    void increment(memoryPointer memory)
    {
        this->binaryIncrement(this->goToAddress(memory));
    }

    void decrement(memoryPointer memory)
    {
        this->binaryDecrement(this->goToAddress(memory));
    }

    void print(memoryPointer memory)
    {
        char move;
        if (memory[INSTRUCTION_LENGTH - 2] == 0 && memory[INSTRUCTION_LENGTH - 1] == 0)
            move = LEFT;
        else if (memory[INSTRUCTION_LENGTH - 2] == 0 && memory[INSTRUCTION_LENGTH - 1] == 1)
            move = RIGHT;
        else if (memory[INSTRUCTION_LENGTH - 2] == 1 && memory[INSTRUCTION_LENGTH - 1] == 0)
            move = UP;
        else if (memory[INSTRUCTION_LENGTH - 2] == 1 && memory[INSTRUCTION_LENGTH - 1] == 1)
            move = DOWN;

        moves[currentMove] = move;
        currentMove++;
    }

    void jump(memoryPointer memory)
    {
        this->programCounter = getAdress(memory);
    }

    void getCurrentInstruction()
    {
        for (int i = 0; i < INSTRUCTION_LENGTH; i++)
        {
            cout << memory[this->programCounter][i];
        }
        cout << endl;
    }

public:
    Machine8bit() {}

    Machine8bit(bool *memoryArray, int memorySize)
    {
        this->MEMORY_SIZE = memorySize;
        for (int i = 0; i < this->MEMORY_SIZE; i++)
        {
            for (int j = 0; j < INSTRUCTION_LENGTH; j++)
            {
                memory[i][j] = memoryArray[i * INSTRUCTION_LENGTH + j];
            }
        }
    }

    string run()
    {
        this->programCounter = 0;
        for (int i = 0; i < MAX_STEPS; i++)
        {
            this->runInstruction(this->memory[this->programCounter]);
        }

        return this->moves;
    }
};
