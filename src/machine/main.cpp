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
    bool memory[MEM_LENGTH][REG_LENGTH];
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
    int getAdress(command cmd)
    {
        int adress = 0;
        // Return the integer value of the address
        for (int i = REG_LENGTH; i >= 2; i--)
        {
            adress += cmd[i] * pow(2, i - 2);
        }

        return adress;
    }

    bool *goToAddress(command cmd)
    {
        int address = getAdress(cmd);
        return memory[address];
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

    /**
     */
    void binaryDecrement(bool *address)
    {
        cout << "Decrementing: " << address[0] << endl;
        for (int i = REG_LENGTH - 1; i >= 0; i--)
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

public:
    Machine()
    {
        for (int i = 0; i < MEM_LENGTH; i++)
        {
            for (int j = 0; j < REG_LENGTH; j++)
            {
                this->memory[i][j] = bool(rand() % 2);
                cout << this->memory[i][j];
            }
        }

        cout << "Machine created" << endl;
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
        this->binaryIncrement(this->goToAddress(memory));
    }

    void decrement(mem memory)
    {
        this->binaryDecrement(this->goToAddress(memory));
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
        this->currentInstruction = getAdress(memory);
    }

    void run()
    {
        this->currentInstruction = 0;
        for (int i = 0; i < MAX_STEPS; i++)
        {
            this->runCommand(this->memory[this->currentInstruction]);
            cout << this->currentInstruction << endl;
        }
        // Print all moves
        cout << "Moves: " << endl;
        for (int i = 0; i < MAX_STEPS; i++)
        {
            cout << moves[i];
        }
    }
};

int main()
{
    for (int i = 0; i < 10; i++)
    {
        cout << "Run " << i << endl;
        Machine machine;
        machine.run();
    }
}

// Path: src\machine\main.cpp
