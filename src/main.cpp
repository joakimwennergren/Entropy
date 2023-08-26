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

// extern "C" void say_hello();

class Game : public Application
{
public:
    Game() : Application()
    {
    }
};

int main()
{
    Game game;
    game.Run();
    return 0;
}
