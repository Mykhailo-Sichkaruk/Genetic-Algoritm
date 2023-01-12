#include "machineAdapter.h"

// Adapter of 8bit Machine,
// Allows create Machine8bit instances with uint8_t array of numbers
// Convert uint8_t array to boolean representation
namespace demo
{
    bool *MachineAdapter::Uint8ToBoolArray(uint8_t number)
    {
        bool *result = new bool[INSTRUCTION_LENGTH];
        for (int i = INSTRUCTION_LENGTH - 1; i >= 0; i--)
        {
            result[i] = number % 2;
            number /= 2;
        }
        return result;
    }

    void MachineAdapter::copyCommand(bool *source, bool *target)
    {
        for (int i = 0; i < INSTRUCTION_LENGTH; i++)
        {
            target[i] = source[i];
        }
    }

    MachineAdapter::MachineAdapter()
    {
        this->MEMORY_SIZE = MEM_LENGTH;
    }

    MachineAdapter::MachineAdapter(uint8_t *instructions, int memorySize)
    {
        this->MEMORY_SIZE = memorySize;
        for (int i = 0; i < MEMORY_SIZE; i++)
        {
            bool *temp = Uint8ToBoolArray(instructions[i]);
            copyCommand(temp, memory[i]);
        }
    }
}