#include <node.h>
#include "./game/game.cpp"

using namespace std;
using namespace v8;

void runGame(const FunctionCallbackInfo<Value> &args)
{
    Isolate *isolate = args.GetIsolate();
    // Get the first argument passed to this function
    String::Utf8Value movesUtf8(isolate, args[0]);
    // Convert the C++ string to a C string
    string moves = string(*movesUtf8);

    // Create a new instance of the game
    GrabStoresGame game = GrabStoresGame();
    // Run the game
    int result = game.run(moves);

    // Create a new integer value
    Local<Number> resultValue = Number::New(isolate, result);
    // Return the result
    args.GetReturnValue().Set(resultValue);
}

void Initialize(Local<Object> exports)
{
    NODE_SET_METHOD(exports, "run", runGame);
}

NODE_MODULE(NODE_GYP_MODULE_NAME, Initialize)