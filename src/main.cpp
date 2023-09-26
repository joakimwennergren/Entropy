#include "symbios.hpp"

class Game : public Application
{
public:
    
    Game()
    {

    }

    void OnRender()
    {
        
    }
};


int main(int argc, char **argv)
{
    Game game;
    game.Run();

    return 0;
}
