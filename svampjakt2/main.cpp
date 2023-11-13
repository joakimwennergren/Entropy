#include "symbios.hpp"
#include "filesystem.hpp"
#include "easing.hpp"
#include "scenegraph.hpp"
#include "renderable.hpp"
#include "sprite.hpp"
#include "label.hpp"
#include "leafsystem.hpp"

// @testing
#include "httpclient.hpp"

#include <cmath>

using namespace Entropy::Network;
using namespace Entropy::Animation;
using namespace Symbios::Global;
using namespace Symbios::Text;

class Game : public Application
{
public:
    SceneGraph *_sceneGraph;
    Game()
    {
        auto client = HttpClient();
        client.Get("https://dog.ceo/api/breeds/list/all");
        _sceneGraph = Global::SceneGraph::GetInstance();
    }

private:
    void OnInit()
    {
        leafSystem = std::make_unique<LeafSystem>();

        auto layer_1 = std::make_shared<Sprite>("layer_1.png");
        layers.push_back(layer_1);
        SceneGraph::GetInstance()->renderables.push_back(layer_1);

        auto layer_2 = std::make_shared<Sprite>("layer_2.png");
        layers.push_back(layer_2);
        SceneGraph::GetInstance()->renderables.push_back(layer_2);

        auto layer_3 = std::make_shared<Sprite>("layer_3.png");
        layers.push_back(layer_3);
        SceneGraph::GetInstance()->renderables.push_back(layer_3);

        auto background = std::make_shared<Sprite>("background.png");
        layers.push_back(background);
        SceneGraph::GetInstance()->renderables.push_back(background);

        // auto title = std::make_shared<Sprite>("title.png");
        // this->title = title;
        // SceneGraph::GetInstance()->renderables.push_back(title);

        /*
        titleText = std::make_unique<Label>();
        for(auto &c : titleText->children)
        {
            SceneGraph::GetInstance()->renderables.push_back(std::move(c));
        }
        */
    }

    void OnRender(float deltaTime)
    {
        leafSystem->Update(screen.width, screen.height);

        for (auto layer : layers)
        {
            layer->Scale2D(screen.width / 2.0, screen.height / 2.0);
            layer->Translate(screen.width / 2.0, screen.height / 2.0 * -1);
        }

        /*
        // Scale and translate layers
        for (auto &renderable : SceneGraph::GetInstance()->renderables)
        {
            if(renderable->id == title)
            {
                renderable->ZIndex(99);
                renderable->Scale2D(screen.width/3.0, screen.height/3.6);
                renderable->Translate(screen.width/2.0, screen.height/2.0 * -1);
            }
        }
        // Update leafsystem

       */
    }

    std::vector<std::shared_ptr<Sprite>> layers;
    std::unique_ptr<LeafSystem> leafSystem;
};

int main(int argc, char **argv)
{
    Game game;
    game.Run();
    return 0;
}
