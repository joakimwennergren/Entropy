#include "config.hpp"

#ifdef BUILD_FOR_IOS
#define NS_PRIVATE_IMPLEMENTATION
#define UI_PRIVATE_IMPLEMENTATION
#define MTL_PRIVATE_IMPLEMENTATION
#define MTK_PRIVATE_IMPLEMENTATION
#define CA_PRIVATE_IMPLEMENTATION
#include <Metal/Metal.hpp>
#include <UIKit/UIKit.hpp>
#include <MetalKit/MetalKit.hpp>
#endif

#include "application.hpp"
#include "state.hpp"

// include Lua headers
extern "C"
{
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

// extern "C" void say_hello();

class Game : public Application
{
public:
    Game() : Application()
    {
    }
};

int main(int argc, char **argv)
{
    Singleton::GetInstance("test")->InitState();
    // create a new lua instance
    luaL_openlibs(Singleton::GetInstance("test")->GetState()); // give lua access to basic libraries
    luaL_dofile(Singleton::GetInstance("test")->GetState(), "C:\\Symbios\\resources\\scripts\\test.lua");

    Game game;
    game.Run();

    lua_close(Singleton::GetInstance("test")->GetState());

    return 0;
}
