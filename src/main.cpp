#include "symbios.hpp"
#include "filesystem.hpp"
#include "easing.hpp"
#include "scenegraph.hpp"
#include "renderable.hpp"
#include "sprite.hpp"
#include "label.hpp"
#include <chaiscript/chaiscript.hpp>
#include <cmath>

using namespace Symbios::Animation;
using namespace Symbios::Global;

float easeOutBounce( float t ) {
    return 1 - pow( 2, -6 * t ) * abs( cos( t * M_PI * 3.5 ) );
}

class Primitives
{
public:
    Primitives()
    {
        _sceneGraph = Global::SceneGraph::GetInstance();
    }

    int CreateSprite(std::string path)
    {
        int id = 0;
        auto sprite = std::make_unique<Sprite>(path);
        id = sprite->id;
        _sceneGraph->renderables.push_back(std::move(sprite));
        return id;
    }

    void RemoveSprite(int id)
    {
        int index = 0;
        for (auto &sprite : _sceneGraph->renderables)
        {
            if(sprite->id == id)
            {
                _sceneGraph->renderables.erase(_sceneGraph->renderables.begin() + index);
                _sceneGraph->renderables[index].reset();
                break;
            }
            index++;
        }
    }

    void Translate2D(int id, float x, float y)
    {
        for (auto &renderable : _sceneGraph->renderables)
        {
            if(renderable->id == id)
            {
                renderable->Translate(x, y);
                break;
            }
        }      
    }

    void Scale2D(int id, float s)
    {
        for (auto &renderable : _sceneGraph->renderables)
        {
            if(renderable->id == id)
            {
                renderable->Scale(s);
                break;
            }
        }      
    }



private:
    SceneGraph * _sceneGraph;

};

class Game : public Application
{
public:

    SceneGraph * _sceneGraph;
    Game()
    {
        srand(time(nullptr));
        _sceneGraph = Global::SceneGraph::GetInstance();
    }


    
private:

    void OnInit()
    {
 
    }

    void OnRender(float deltaTime)
    {

    }
};


int main(int argc, char **argv)
{
    Game game;
    game.Run();
    return 0;
}
