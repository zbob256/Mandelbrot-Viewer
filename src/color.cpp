#include <SFML/Graphics.hpp>
#include <cmath>
#include "color.hpp"

sf::Color color(int iterations, int maxIterations, int colorId) {
    if (iterations == maxIterations) {
        return sf::Color::Black;
    }

    float t = (float)iterations / maxIterations;

    switch (colorId) {
        case 0:
            return sf::Color(int(t * 255), int(t * 255), int(t * 255));
        case 1:
            return sf::Color(int(t * 255), 0, 0);
        case 2:
            return sf::Color(int(t * 255), int(t * 127.5f), 0);
        case 3:
            return sf::Color(int(t * 255), int(t * 255), 0);
        case 4:
            return sf::Color(0, int(t * 255), 0);
        case 5:
            return sf::Color(0, int(t * 255), int(t * 255));
        case 6:
            return sf::Color(0, 0, int(t * 255));
        case 7:
            return sf::Color(int(t * 148), 0, int(t * 211));
        case 8: {
            int r = (int)(127.5f * (sin(6.28318f * t) + 1));
            int g = (int)(127.5f * (sin(6.28318f * t + 2.094f) + 1));
            int b = (int)(127.5f * (sin(6.28318f * t + 4.188f) + 1));

            return sf::Color(r, g, b);
        } case 9:
            if (t < 0.25f) {
                return sf::Color(0, 0, (int)(255 * t * 4));
            } else if (t < 0.5f) {
                return sf::Color(0, (int)(255 * (t - 0.25f) * 4), 255);
            } else if (t < 0.75f) {
                return sf::Color((int)(255 * (t - 0.5f) * 4), 255, 255);
            }

            return sf::Color(255, 255, (int)(255 * (1.0f - (t - 0.75f) * 4)));
        default:
            return sf::Color::Black;
    }
}