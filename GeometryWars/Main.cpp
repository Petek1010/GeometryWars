#include <SFML/Graphics.hpp>
#include <iostream>
#include "Vec2.h"
#include "Entity.h"


int main()
{
 
    Vec2 v1(20, 70);
    Vec2 v2(30, 10);
    
    
    v1.normalize();
   
    std::cout << v1.x << " " << v1.y << std::endl;
    std::cout << v1.length() << "\n";


    std::cout << '\a';
    return 0;
}