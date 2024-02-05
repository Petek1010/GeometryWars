#include <SFML/Graphics.hpp>
#include "Game.h"

int main()
{
    Game game ("Config.txt");
    game.run();
   
    return 0;
}