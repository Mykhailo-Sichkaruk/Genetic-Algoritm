#include "machineAdapter.h"

// Adapter of 8bit Machine,
// Allows create Machine8bit instances with uint8_t array of numbers
// Convert uint8_t array to boolean representation
bool *MachineAdapter::Uint8ToBoolArray(uint8_t number)
{
    bool *result = new bool[REGISTER_LEN];
    for (int i = REGISTER_LEN - 1; i >= 0; i--)
    {
        result[i] = number % 2;
        number /= 2;
    }
    return result;
}

void MachineAdapter::copyCommand(bool *source, bool *target)
{
    for (int i = 0; i < REGISTER_LEN; i++)
    {
        target[i] = source[i];
    }
}

MachineAdapter::MachineAdapter()
{
}

MachineAdapter::MachineAdapter(uint8_t *instructions)
{
    for (int i = 0; i < MEMORY_SIZE; i++)
    {
        bool *temp = Uint8ToBoolArray(instructions[i]);
        copyCommand(temp, memory[i]);
    }
}