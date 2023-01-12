// obj.h
#ifndef ADAPTER
#define ADAPTER

#include "machine.cpp"

namespace demo
{
    class MachineAdapter : public Machine8bit
    {
    private:
        bool *Uint8ToBoolArray(uint8_t number);
        void copyCommand(bool *source, bool *target);

    public:
        MachineAdapter();
        MachineAdapter(uint8_t *instructions, int memorySize);
    };
}
#endif