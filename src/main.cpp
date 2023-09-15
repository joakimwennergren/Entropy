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
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

// extern "C" void say_hello();

// Factories
#include "primitive_factory.hpp"
#include "sprite.hpp"
#include "label.hpp"

using namespace Symbios::Graphics::Factories;
using namespace Symbios::Text;

class Game : public Application
{
public:
    Game() : Application()
    {
        
        auto primitivesFactory = PrimitiveFactory(_context);

        getGlobalNamespace(Singleton::GetInstance("test")->GetState())
            .beginClass<PrimitiveFactory>("PrimitiveFactory")
            .addConstructor<void (*)()>()
            .addFunction("NewQuad", &PrimitiveFactory::Test)
            .endClass();

        auto quad = std::make_shared<Sprite>(_context);
        quad->type = 1;
        quad->position = glm::vec3(200.0, -500.0, 0.0);
        quad->textureId = 1;
        quad->color = glm::vec4(1.0, 1.0, 1.0, 1.0);
        quad->scale = glm::vec3(100.0, 100.0, 0.0);
        quad->texture->CreateTextureImage("/Users/joakim/Desktop/Symbios/resources/textures/svamp.png");
        quad->UpdateImage();

        auto label = std::make_shared<Label>(_context, "Symbios ");

        _sceneGraph->renderables.insert(std::make_pair(0, quad));

        int id = 1;

        for(auto ch : label->sprites)
        {
            _sceneGraph->renderables.insert(std::make_pair(id++, ch));
        }
        //_sceneGraph->renderables.insert(std::make_pair(1, label));
    }
};

int main(int argc, char **argv)
{
    Singleton::GetInstance("test")->InitState();

    luaL_openlibs(Singleton::GetInstance("test")->GetState());

    Game game;
    game.Run();

    lua_close(Singleton::GetInstance("test")->GetState());

    return 0;
}
