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
using namespace Symbios::Contexts;

extern "C" CGPoint touch();

struct TPoint
{
    float x;
    float y;
};

float GetTouchPointX() {
    CGPoint point = touch();
    return (float)point.x * 3.0;
}

float GetTouchPointY() {
    CGPoint point = touch();
    return (float)point.y * 3.3;
}

float sinus(float x)
{
    return sin(x);
}

int random_num(int min, int max)
{
    return rand() % max + min;
}

float random_float()
{
    float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    return r;
}

float easeOutBounce( float t ) {
    return 1 - pow( 2, -6 * t ) * abs( cos( t * M_PI * 3.5 ) );
}

class Game : public Application
{
public:
    
    Game()
    {
        srand(time(nullptr));
        _sceneGraph = Contexts::SceneGraph::GetInstance();
    }
    
private:
    SceneGraph * _sceneGraph;

    void OnInit()
    {
        this->chai.add(chaiscript::constructor<Label(const Label &)>(), "Label");
        this->chai.add(chaiscript::constructor<Label ()>(), "Label");
        this->chai.add(chaiscript::fun(&Label::SetZIndex), "SetZIndex");
        this->chai.add(chaiscript::fun(&Label::SetPosition), "SetPosition");
        this->chai.add(chaiscript::fun(&Label::SetText), "SetText");

        this->chai.add(chaiscript::fun(&GetTouchPointX), "TouchPointX");
        this->chai.add(chaiscript::fun(&GetTouchPointY), "TouchPointY");

        this->chai.add(chaiscript::fun(&random_num), "random");
        this->chai.add(chaiscript::fun(&random_float), "randomFloat");
        this->chai.add(chaiscript::fun(&sinus), "sin");
        this->chai.add(chaiscript::fun(&easeOutBounce), "easeOutBounce");

        this->chai.add(chaiscript::base_class<Renderable, Sprite>());
        this->chai.add(chaiscript::base_class<Renderable, Label>());

        this->chai.add_global(chaiscript::var(_sceneGraph), "Graph"); // global non-const, throws if object exists
        this->chai.add(chaiscript::fun(&SceneGraph::RemoveObjectById), "RemoveObjectById");
        this->chai.add(chaiscript::fun(&SceneGraph::Add), "Add");
        this->chai.add(chaiscript::fun(&SceneGraph::Remove), "Remove");

        this->chai.add(chaiscript::constructor<Sprite(std::string path)>(), "Sprite");
        this->chai.add(chaiscript::fun(&Sprite::SetScale), "SetScale");
        this->chai.add(chaiscript::fun(&Sprite::SetPosition), "SetPosition");
        this->chai.add(chaiscript::fun(&Sprite::SetName), "SetName");
        this->chai.add(chaiscript::fun(&Sprite::SetId), "SetId");
        this->chai.add(chaiscript::fun(&Sprite::SetZIndex), "SetZIndex");
        this->chai.add(chaiscript::fun(&Sprite::SetRotation), "SetRotation");
        this->chai.add(chaiscript::fun(&Sprite::SetColor), "SetColor");

        this->chai.add(chaiscript::var(std::ref(screen)), "screen"); // by reference, shared between C++ and chai
        


        this->chai.add(chaiscript::user_type<Sprite>(), "Sprite");
        this->chai.add(chaiscript::constructor<Sprite ()>(), "Sprite");
        
        //this->chai->use("/Users/joakim/Desktop/Symbios/resources/scripts/test.chai");
        this->chai.use(Filesystem::GetProjectBasePath() + "/test.chai");
    }

    void OnRender(float deltaTime)
    {
        auto onResize = chai.eval<std::function<void(int, int)>>("OnResize"); 

        onResize(screen.width, screen.height);

        auto p = chai.eval<std::function<void()>>("OnUpdate"); 
        p();
    }
};


int main(int argc, char **argv)
{
    Game game;
    game.Run();

    return 0;
}
