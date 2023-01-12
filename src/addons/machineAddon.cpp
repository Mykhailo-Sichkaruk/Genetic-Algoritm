#include <node.h>
#include "./machine/obj.h"

namespace demo
{

    using v8::Local;
    using v8::Object;

    void InitAll(Local<Object> exports)
    {
        MachineObj::Init(exports);
    }

    NODE_MODULE(NODE_GYP_MODULE_NAME, InitAll)

}