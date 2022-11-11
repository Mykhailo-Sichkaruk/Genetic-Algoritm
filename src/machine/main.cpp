#include <iostream>
#include <time.h>
#include <string.h>
#include <Math.h>
#include <stdlib.h>
#include <node_api.h>
#include <node.h>

using namespace std;

const int REG_LENGTH = 8; // Number of bits in a register
const int MAX_STEPS = 600;
const int MEM_LENGTH = 64;        // Number of bytes in memory
typedef bool command[REG_LENGTH]; // Register type
typedef bool mem[REG_LENGTH - 2];

class Machine
{
private:
    enum Direction
    {
        LEFT = 'L',
        RIGHT = 'R',
        UP = 'U',
        DOWN = 'D',
    };
    int id;
    bool memory[MEM_LENGTH][REG_LENGTH];
    char moves[MAX_STEPS] = "";

    int currentMove = 0;
    int currentInstruction = 0;
    string strMemory;
    int getAdress(command cmd)
    {
        int adress = 0;
        for (int i = REG_LENGTH - 1; i >= 2; i--)
        {
            adress += cmd[i] * pow(2, REG_LENGTH - i - 1);
        }
        return adress;
    }

    bool *goToAddress(command cmd)
    {
        int address = getAdress(cmd);
        return memory[address];
    }

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

    void binaryDecrement(bool *address)
    {
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
    Machine() {}

    // Machine(bool *memoryArray, int id)
    // {
    //     this->id = id;
    //     for (int i = 0; i < MEM_LENGTH; i++)
    //     {
    //         for (int j = 0; j < REG_LENGTH; j++)
    //         {
    //             memory[i][j] = memoryArray[i * REG_LENGTH + j];
    //         }
    //     }
    // }

    Machine(string memoryArray)
    {
        this->strMemory = memoryArray;
        for (int i = 0; i < MEM_LENGTH; i++)
        {
            for (int j = 0; j < REG_LENGTH; j++)
            {
                memory[i][j] = memoryArray[i * REG_LENGTH + j] == '1';
            }
        }
    }

    // Machine(int id)
    // {
    //     srand(id + time(NULL));
    //     for (int i = 0; i < MEM_LENGTH; i++)
    //     {
    //         for (int j = 0; j < REG_LENGTH; j++)
    //         {
    //             this->memory[i][j] = bool(rand() % 2);
    //         }
    //     }
    // }

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

        if (this->currentInstruction >= MEM_LENGTH - 1)
        {
            this->currentInstruction = 0;
        }
        else
        {
            this->currentInstruction++;
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

    void getCurrentInstruction()
    {
        for (int i = 0; i < REG_LENGTH; i++)
        {
            cout << memory[this->currentInstruction][i];
        }
        cout << endl;
    }

    string run()
    {
        this->currentInstruction = 0;
        for (int i = 0; i < MAX_STEPS; i++)
        {
            this->runCommand(this->memory[this->currentInstruction]);
        }

        return this->moves;
    }
};

bool *generateMemory(int size, int randomSeed)
{
    bool *memory = new bool[size];
    srand(randomSeed + time(NULL));

    for (int i = 0; i < size; i++)
    {
        memory[i] = bool(rand() % 2);
    }

    return memory;
}

// int main(int argc, const char **argv)
// {
//     const int NUM_MACHINES = 1000;
//     Machine machines[NUM_MACHINES];
//     for (int i = 0; i < NUM_MACHINES; i++)
//     {
//         machines[i] = Machine(generateMemory(MEM_LENGTH * REG_LENGTH, i), i);
//         cout << "Run " << i << endl;
//         cout << machines[i].run() << endl;
//     }
// }

namespace demo
{

    using v8::FunctionCallbackInfo;
    using v8::Isolate;
    using v8::Local;
    using v8::Number;
    using v8::Object;
    using v8::String;
    using v8::Value;

    void runMachine(const FunctionCallbackInfo<Value> &args)
    {
        Isolate *isolate = args.GetIsolate();
        v8::String::Utf8Value input(isolate, args[0]);
        std::string memory(*input);
        Machine machine = Machine(memory.c_str());
        string result = machine.run();
        args.GetReturnValue().Set(String::NewFromUtf8(
                                      isolate, result.c_str())
                                      .ToLocalChecked());
    }

    void Method(const FunctionCallbackInfo<Value> &args)
    {
        Isolate *isolate = args.GetIsolate();
        args.GetReturnValue().Set(String::NewFromUtf8(
                                      isolate, "world")
                                      .ToLocalChecked());
    }

    void Initialize(Local<Object> exports)
    {
        NODE_SET_METHOD(exports, "run", runMachine);
    }

    NODE_MODULE(NODE_GYP_MODULE_NAME, Initialize)

}
