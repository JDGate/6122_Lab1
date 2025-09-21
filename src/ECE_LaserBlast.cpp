#include "../include/ECE_LaserBlast.hpp"
#include <iostream>

ECE_LaserBlast::ECE_LaserBlast(sf::Texture& texturePlayer) : sf::Sprite(texturePlayer)
{
    //std::cout << "Gator scale and position set" << std::endl;
    setScale({ 0.5f, 0.5f });
}