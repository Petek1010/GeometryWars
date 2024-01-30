#include <SFML/Graphics.hpp>
#include <iostream>
#include "Game.h"
#include "Entity.h"



int main()
{
 
    Game game ("Config.txt");
    
    
    game.run();
   
    return 0;
}