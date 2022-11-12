#include <node.h>
#include "./machine/machineAdapter.cpp"

using namespace v8;
using namespace std;

void runMachine(const FunctionCallbackInfo<Value> &args)
{
    Isolate *isolate = args.GetIsolate();

    // Get the first argument - Typed Array of uint8
    Local<ArrayBuffer> contents = args[0].As<ArrayBuffer>();
    uint8_t *ptr = (uint8_t *)contents->GetBackingStore()->Data();
    // Get the second argument - memory size of the machine (int)
    int length = args[1].As<Number>()->Value();

    // Create MachineAdapter instance
    MachineAdapter machine = MachineAdapter(ptr, length);
    // Run the machine and get the result - moves
    string result = machine.run();

    // Return the result to JS
    args.GetReturnValue()
        .Set(String::NewFromUtf8(
                 isolate, result.c_str())
                 .ToLocalChecked());
}

void Initialize(Local<Object> exports)
{
    NODE_SET_METHOD(exports, "run", runMachine);
}

NODE_MODULE(NODE_GYP_MODULE_NAME, Initialize)