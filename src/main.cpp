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


// extern "C" void say_hello();

// Factories
#include "primitive_factory.hpp"
#include "sprite.hpp"
#include "label.hpp"
#include "quad.hpp"
#include "filesystem.hpp"

using namespace Symbios::Graphics::Factories;
using namespace Symbios::Graphics::Primitives;
using namespace Symbios::Text;
using namespace Symbios;

class Game : public Application
{
public:
    Game() : Application() 
    {
        srand (time(NULL));
    }
};

int main(int argc, char **argv)
{
    Game game;
    game.Run();

    return 0;
}
