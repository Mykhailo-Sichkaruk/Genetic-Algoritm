#include "machineAdapter.h"
#include "obj.h"

namespace demo
{
    using namespace std;
    using v8::ArrayBuffer;
    using v8::Context;
    using v8::Function;
    using v8::FunctionCallbackInfo;
    using v8::FunctionTemplate;
    using v8::Isolate;
    using v8::Local;
    using v8::Number;
    using v8::Object;
    using v8::ObjectTemplate;
    using v8::String;
    using v8::Value;

    MachineObj::MachineObj(uint8_t *instructions, int memorySize)
    {
        this->machine = MachineAdapter::MachineAdapter(instructions, memorySize);
    }

    MachineObj::~MachineObj()
    {
    }

    void MachineObj::Init(Local<Object> exports)
    {
        Isolate *isolate = exports->GetIsolate();
        Local<Context> context = isolate->GetCurrentContext();

        Local<ObjectTemplate> addon_data_tpl = ObjectTemplate::New(isolate);
        addon_data_tpl->SetInternalFieldCount(1); // 1 field for the MyObject::New()
        Local<Object> addon_data = addon_data_tpl->NewInstance(context).ToLocalChecked();

        // Prepare constructor template
        Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New, addon_data);
        tpl->SetClassName(String::NewFromUtf8(isolate, "Machine").ToLocalChecked());
        tpl->InstanceTemplate()->SetInternalFieldCount(1);

        // Prototype
        NODE_SET_PROTOTYPE_METHOD(tpl, "run", Run);

        Local<Function> constructor = tpl->GetFunction(context).ToLocalChecked();
        addon_data->SetInternalField(0, constructor);
        exports->Set(context, String::NewFromUtf8(isolate, "Machine").ToLocalChecked(),
                     constructor)
            .FromJust();
    }

    void MachineObj::New(const FunctionCallbackInfo<Value> &args)
    {
        Isolate *isolate = args.GetIsolate();
        Local<Context> context = isolate->GetCurrentContext();

        if (args.IsConstructCall())
        {
            // Invoked as constructor: `new MachineObj(...)`

            /// Check if there is only one argument
            if (args.Length() != 1)
            {
                isolate->ThrowException(v8::Exception::TypeError(
                    String::NewFromUtf8(isolate, "Wrong number of arguments")
                        .ToLocalChecked()));
                return;
            }
            // Check if first argument is an array buffer
            if (!args[0]->IsArrayBuffer())
            {
                // Check if first argument is an array buffer
                isolate->ThrowException(v8::Exception::TypeError(
                    String::NewFromUtf8(isolate, "First argument must be an ArrayBuffer")
                        .ToLocalChecked()));
                return;
            }
            // Get the first argument - Typed Array of uint8
            Local<ArrayBuffer> contents = args[0].As<ArrayBuffer>();
            uint8_t *ptr = (uint8_t *)contents->GetBackingStore()->Data();
            // Get the length of the array
            int length = args[0].As<ArrayBuffer>()->ByteLength();
            MachineObj *obj = new MachineObj(ptr, length);
            obj->Wrap(args.This());
            args.GetReturnValue().Set(args.This());
        }
        else
        {
            // Invoked as plain function `MyObject(...)`, turn into construct call.
            // const int argc = 1;
            // Local<Value> argv[argc] = {args[0]};
            // Local<Function> cons =
            //     args.Data().As<Object>()->GetInternalField(0).As<Function>();
            // Local<Object> result =
            //     cons->NewInstance(context, argc, argv).ToLocalChecked();
            // args.GetReturnValue().Set(result);
        }
    }

    void MachineObj::Run(const FunctionCallbackInfo<Value> &args)
    {
        Isolate *isolate = args.GetIsolate();

        MachineObj *obj = ObjectWrap::Unwrap<MachineObj>(args.Holder());
        string result = obj->machine.run();

        args.GetReturnValue()
            .Set(String::NewFromUtf8(
                     isolate, result.c_str())
                     .ToLocalChecked());
    }

}