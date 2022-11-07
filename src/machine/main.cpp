#include <iostream>
#include <string>
#include <Math.h>

using namespace std;

const int REG_LENGTH = 8; // Number of bits in a register
const int MAX_STEPS = 600;
const int MEM_LENGTH = 64;        // Number of bytes in memory
typedef bool command[REG_LENGTH]; // Register type
typedef bool mem[REG_LENGTH - 2];

class Machine
{
private:
    int id;
    bool memory[REG_LENGTH][MEM_LENGTH];
    enum Direction
    {
        LEFT = 'L',
        RIGHT = 'R',
        UP = 'U',
        DOWN = 'D',
    };
    char moves[MAX_STEPS];
    int currentMove = 0;
    int currentInstruction = 0;
    /**
     *  Get the boolean array of a command and return the integer value of address
     */
    int
    getAdress(command cmd)
    {
        int adress = 0;
        for (int i = 2; i < REG_LENGTH; i++)
        {
            adress += cmd[i] * pow(2, i);
        }
        return adress;
    }

    /**
     * @brief Return array of bits after incrementing the address
     * Incrementing is cyclic
     * @param data
     */
    void binaryIncrement(bool *address)
    {
        address = address + 8;
        for (int i = REG_LENGTH - 1; i >= 0; i--)
        {
            if (address[i] == 0)
            {
                address[i] = 1;
                break;
            }
            else
            {
                address[i] = 0;
            }
        }
    }

    /**
     */
    void binaryDecrement(bool *address)
    {
        address = address + 8;
        for (int i = REG_LENGTH - 1; i >= 0; i--)
        {
            if (address[i] == 1)
            {
                address[i] = 0;
                break;
            }
            else
            {
                address[i] = 1;
            }
        }
    }

public:
    Machine(bool memory[MEM_LENGTH][REG_LENGTH])
    {
        for (int i = 0; i < MEM_LENGTH; i++)
        {
            for (int j = 0; j < REG_LENGTH; j++)
            {
                this->memory[i][j] = memory[i][j];
            }
        }
    }

    void runCommand(command command)
    {
        if (command[0] == 0 && command[1] == 0)
        {
            increment(command);
        }
        else if (command[0] == 0 && command[1] == 1)
        {
            decrement(command);
        }
        else if (command[0] == 1 && command[1] == 0)
        {
            jump(command);
        }
        else if (command[0] == 1 && command[1] == 1)
        {
            print(command);
        }
    }

    void increment(mem memory)
    {
        int addressOfMemory = this->getAdress(memory);
        bool targetReg[REG_LENGTH] = this->memory[addressOfMemory];
        bool *result = this->binaryIncrement(targetReg);
        this->memory[addressOfMemory] = *result;
    }

    void decrement(mem memory)
    {
        copy(memory, memory + REG_LENGTH, this->memory[this->getAdress(memory)]);
        bool targetReg[REG_LENGTH] = this->memory[this->getAdress(memory)];
        bool *result = this->binaryDecrement(targetReg);
        this->memory[this->getAdress(memory)] = result;
    }

    void print(mem memory)
    {
        char move;
        if (memory[REG_LENGTH - 2] == 0 && memory[REG_LENGTH - 1] == 0)
            move = LEFT;
        else if (memory[REG_LENGTH - 2] == 0 && memory[REG_LENGTH - 1] == 1)
            move = RIGHT;
        else if (memory[REG_LENGTH - 2] == 1 && memory[REG_LENGTH - 1] == 0)
            move = UP;
        else if (memory[REG_LENGTH - 2] == 1 && memory[REG_LENGTH - 1] == 1)
            move = DOWN;

        moves[currentMove] = move;
        currentMove++;
    }

    void jump(mem memory)
    {
    }

    void run()
    {
        for (int i = 0; i < MAX_STEPS; i++)
        {
            this->runCommand(this->memory[i]);
        }
        cout << this->moves;
    }
};

bool randomMemory()
{
    bool memory[MEM_LENGTH][REG_LENGTH];

    for (int i = 0; i < MEM_LENGTH; i++)
    {
        for (int j = 0; j < REG_LENGTH; j++)
        {
            memory[i][j] = rand() % 2;
        }
    }

    return memory;
}

int main()
{
    Machine machine;
    for (int i = 0; i < 100; i++)
    {
        machine = new Machine(randomMemory());
        machine.run();
        machine = NULL;
    }
}

// Path: src\machine\main.cpp
