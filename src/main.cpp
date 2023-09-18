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
#include "filesystem.hpp"

using namespace Symbios::Graphics::Factories;
using namespace Symbios::Text;
using namespace Symbios;

class Game : public Application
{
public:
    Game() : Application() 
    {
        auto primitivesFactory = PrimitiveFactory();

        auto quad = std::make_shared<Sprite>();
        quad->type = 1;
        quad->position = glm::vec3(500.0, -500.0, 0.0);
        quad->textureId = 1;
        quad->color = glm::vec4(1.0, 1.0, 1.0, 1.0);
        quad->scale = glm::vec3(300.0, 300.0, 0.0);
        quad->texture->CreateTextureImage(Filesystem::GetProjectBasePath() + "/svamp.png");
        quad->UpdateImage();

        auto label = std::make_shared<Label>("Symbios");

        int id = 1;

        for(auto ch : label->sprites)
        {
            _sceneGraph->renderables.insert(std::make_pair(id++, ch));
        }
        //_sceneGraph->renderables.insert(std::make_pair(1, label));

        _sceneGraph->renderables.insert(std::make_pair(0, quad));

    }
};

int main(int argc, char **argv)
{
    Game game;
    game.Run();

    return 0;
}
