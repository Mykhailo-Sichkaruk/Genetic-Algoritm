#include "main.cpp"
#include <node.h>

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