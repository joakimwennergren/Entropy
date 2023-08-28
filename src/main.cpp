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

// *** define C++ function ***
static int MyCppFunction(lua_State *L) // Lua callable functions must be this format
{
    const int num = (int)lua_tonumber(L, 1); // get first param from stack
    const char *str = lua_tostring(L, 2);    // get second param from stack
    std::cout << "Hello from C++!" << std::endl;
    std::cout << "num = " << num << ", str = " << str << std::endl;
    return 0; // how many params we're passing to Lua
}

int main(int argc, char **argv)
{
    lua_State *L = luaL_newstate();                                             // create a new lua instance
    luaL_openlibs(L);                                                           // give lua access to basic libraries
    lua_register(L, "CallMyCppFunction", MyCppFunction);                        // register our C++ function with Lua
    luaL_dofile(L, "/Users/joakim/Desktop/Symbios/resources/scripts/test.lua"); // loads the Lua script

    lua_close(L);

    Game game;
    game.Run();

    return 0;
}
