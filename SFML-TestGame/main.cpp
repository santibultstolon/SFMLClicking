#include "Game.h"

int main()
{

    //Initialize srand
    std::srand(static_cast<unsigned>(time(NULL)));
    
    //Initialize Game class

    Game game;

    //Game Loop
    while (game.isRunning() && !game.getEndGame())
    {
        //Update
        game.Update();
        //Render
        game.Render();
    }

    return 0;
}