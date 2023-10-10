#include "symbios.hpp"
#include "filesystem.hpp"
#include "easing.hpp"
#include "scenegraph.hpp"
#include "renderable.hpp"
#include "sprite.hpp"
#include "label.hpp"
#include <chaiscript/chaiscript.hpp>
#include <cmath>
#include <sol/sol.hpp>

using namespace Symbios::Animation;
using namespace Symbios::Contexts;

float easeOutBounce( float t ) {
    return 1 - pow( 2, -6 * t ) * abs( cos( t * M_PI * 3.5 ) );
}

class Primitives
{
public:
    Primitives()
    {
        _sceneGraph = Contexts::SceneGraph::GetInstance();
    }

    Sprite * CreateSprite(std::string path)
    {
        const auto sprite = new Sprite(path);
        _sceneGraph->Add(sprite);
        return sprite;
    }

    void RemoveSprite(Sprite * sprite)
    {
        for(auto renderable : _sceneGraph->renderables)
        {
            if(renderable == sprite)
            {
                PLOG_DEBUG << "FOUND SPRITE... deleting";
                delete sprite;
            }
        }
    }

private:
        SceneGraph * _sceneGraph;

};

class Game : public Application
{
public:
    sol::state lua;
    Game()
    {
        srand(time(nullptr));
        _sceneGraph = Contexts::SceneGraph::GetInstance();
    }


    
private:
    SceneGraph * _sceneGraph;

    void OnInit()
    {

        lua.open_libraries(sol::lib::base);
        lua.open_libraries(sol::lib::math);
        lua.open_libraries(sol::lib::table);
        lua.open_libraries(sol::lib::os);
        lua.open_libraries(sol::lib::package);

        lua.set_function("CreateSprite", &Primitives::CreateSprite, Primitives());
        lua.set_function("RemoveSprite", &Primitives::RemoveSprite, Primitives());

        lua.new_usertype<SceneGraph>("SceneGraph",
            "new", sol::no_constructor,
            "getInstance", &SceneGraph::GetInstance,
            "Add", &SceneGraph::Add
        );

        // make usertype metatable
        sol::usertype<Sprite> sprite_type = lua.new_usertype<Sprite>(
            "Sprite", sol::constructors<Sprite(std::string)>(),
            sol::base_classes, sol::bases<Renderable>()
        );

        // typical member function that returns a variable
        sprite_type["SetPosition"] = &Sprite::SetPosition;
        sprite_type["SetScale"] = &Sprite::SetScale;

        lua.script_file(Filesystem::GetProjectBasePath() +  "/main.lua");

        /*
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
        */
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
