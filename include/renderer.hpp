#pragma once
#include <SFML/Graphics.hpp>

extern long double realBounds[2];
extern long double imaginaryBounds[2];

sf::Sprite renderMandelbrot(sf::Texture& texture, unsigned int width, unsigned int height, int maxIterations, int colorId);