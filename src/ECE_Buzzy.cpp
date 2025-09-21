#include "../include/ECE_Buzzy.hpp"
#include <iostream>

ECE_Buzzy::ECE_Buzzy(sf::Texture& texturePlayer) : sf::Sprite(texturePlayer)
{
    //std::cout << "Florida Man cale and position set" << std::endl;
    setScale({ 0.5f, 0.5f });
    setPosition({ 550.f, 50.f });
}