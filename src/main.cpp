#include "symbios.hpp"
#include "filesystem.hpp"
#include "easing.hpp"
#include "scenegraph.hpp"
#include "renderable.hpp"
#include "sprite.hpp"
#include <chaiscript/chaiscript.hpp>

using namespace Symbios::Animation;
using namespace Symbios::Contexts;

class Game : public Application
{
public:
    
    Game()
    {
        _sceneGraph = Contexts::SceneGraph::GetInstance();
    }
    
private:
    SceneGraph * _sceneGraph;

    void OnInit()
    {
        chai = Global::GetInstance()->GetChaiInstance();

        chai->add(chaiscript::base_class<Renderable, Sprite>());

        chai->add_global(chaiscript::var(_sceneGraph), "Graph"); // global non-const, throws if object exists
        chai->add(chaiscript::fun(&SceneGraph::RemoveObjectByName), "RemoveObjectByName");
        chai->add(chaiscript::fun(&SceneGraph::Add), "Add");

        chai->add(chaiscript::constructor<Sprite(std::string path)>(), "Sprite");
        chai->add(chaiscript::fun(&Sprite::SetScale), "SetScale");
        chai->add(chaiscript::fun(&Sprite::SetPosition), "SetPosition");
        chai->add(chaiscript::fun(&Sprite::SetName), "SetName");

        chai->add(chaiscript::var(std::ref(screen)), "screen"); // by reference, shared between C++ and chai

        chai->use("/Users/joakim/Desktop/Symbios/resources/scripts/test.chai");

        /*
        auto _sceneGraph = Contexts::SceneGraph::GetInstance();

        // Foreground - Layer 1
        auto layer_1 = std::make_shared<Sprite>();
        layer_1->zIndex = 10;
        layer_1->New(
            Filesystem::GetProjectBasePath() + "/layer_1.png", 
            glm::vec3(frame.size.width * 1.5f, frame.size.height * 2.0f * -1.0f, 0.0),
            glm::vec3(frame.size.width * 1.5f, frame.size.height * 2.0f, 0.0),
            glm::vec4(1.0, 1.0, 1.0, 1.0)
        );

        _sceneGraph->renderables.push_back(layer_1);
        */

        /*
        // Foreground - Layer 2
        auto layer_2 = std::make_shared<Sprite>();
        layer_2->zIndex = 8;
        layer_2->New(
            Filesystem::GetProjectBasePath() + "/layer_2.png", 
            glm::vec3(frame.size.width * 1.5f, frame.size.height * 2.0f * -1.0f, 0.0),
            glm::vec3(frame.size.width * 1.5f, frame.size.height * 2.0f, 0.0),
            glm::vec4(1.0, 1.0, 1.0, 1.0)
        );
        _sceneGraph->renderables.push_back(layer_2);

        
        // Foreground - Layer 3
        auto layer_3 = std::make_shared<Sprite>();
        layer_3->zIndex = 6;
        layer_3->New(
            Filesystem::GetProjectBasePath() + "/layer_3.png", 
            glm::vec3(frame.size.width * 1.5f, frame.size.height * 2.0f * -1.0f, 0.0),
            glm::vec3(frame.size.width * 1.5f, frame.size.height * 2.0f, 0.0),
            glm::vec4(1.0, 1.0, 1.0, 1.0)
        );
        _sceneGraph->renderables.push_back(layer_3);
        
        // Foreground - Layer 3
        auto banderoll = std::make_shared<Sprite>();
        banderoll->name = "banderoll";
        banderoll->zIndex = 20;
        banderoll->id = 777;
        banderoll->New(
            Filesystem::GetProjectBasePath() + "/banderoll.png",
            glm::vec3(frame.size.width * 1.5f, frame.size.height * 1.75f * -1.0f, 0.0),
            glm::vec3(frame.size.width * 1.0f, frame.size.height * 1.0f, 0.0),
            glm::vec4(1.0, 1.0, 1.0, 0.8)
        );
        _sceneGraph->renderables.push_back(banderoll);

        
        // Background
        auto background = std::make_shared<Sprite>();
        background->zIndex = 4;
        background->New(
            Filesystem::GetProjectBasePath() + "/background.png", 
            glm::vec3(frame.size.width * 1.5f, frame.size.height * 2.0f * -1.0f, 0.0),
            glm::vec3(frame.size.width * 1.5f, frame.size.height * 2.0f, 0.0),
            glm::vec4(1.0, 1.0, 1.0, 1.0)
        );
        _sceneGraph->renderables.push_back(background);  
        */   
    }

    void OnRender(float deltaTime)
    {
        auto onResize = chai->eval<std::function<void(int, int)>>("OnResize"); 

        onResize(screen.width, screen.height);

        auto p = chai->eval<std::function<void()>>("OnUpdate"); 
        p();
    }
};


int main(int argc, char **argv)
{
    Game game;
    game.Run();

    return 0;
}
