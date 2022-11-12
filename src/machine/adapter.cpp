#include "machine.cpp"

// Adapter of 8bit Machine,
// Allows create Machine8bit instances with uint8_t array of numbers
// Convert uint8_t array to boolean representation
class MachineAdapter : public Machine8bit
{
private:
    bool *Uint8ToBool(uint8_t number)
    {
        bool *result = new bool[INSTRUCTION_LENGTH];
        for (int i = INSTRUCTION_LENGTH - 1; i >= 0; i--)
        {
            result[i] = number % 2;
            number /= 2;
        }
        return result;
    }

    void copyCommand(bool *source, bool *target)
    {
        for (int i = 0; i < INSTRUCTION_LENGTH; i++)
        {
            target[i] = source[i];
        }
    }

public:
    MachineAdapter(uint8_t *instructions, int memorySize)
    {
        this->MEMORY_SIZE = memorySize;
        for (int i = 0; i < this->MEMORY_SIZE; i++)
        {
            bool *temp = Uint8ToBool(instructions[i]);
            this->copyCommand(temp, memory[i]);
        }
    }
};
