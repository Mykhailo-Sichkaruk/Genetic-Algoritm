// obj.h
#ifndef OBJ_H
#define OBJ_H

#include <node.h>
#include <node_object_wrap.h>
#include "machineAdapter.h"

namespace demo
{
    class MachineObj : public node::ObjectWrap
    {
    public:
        static void Init(v8::Local<v8::Object> exports);

    private:
        explicit MachineObj(uint8_t *instructions, int memorySize);
        ~MachineObj();

        static void New(const v8::FunctionCallbackInfo<v8::Value> &args);
        static void Run(const v8::FunctionCallbackInfo<v8::Value> &args);

        MachineAdapter machine;
    };
}
#endif
