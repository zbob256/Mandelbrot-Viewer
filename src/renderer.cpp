#include <SFML/Graphics.hpp>
#include <iostream>
#include <thread>
#include <vector>
#include "mandelbrot.hpp"
#include "color.hpp"

long double realBounds[2] = {-2.2L, 1.0L};
long double imaginaryBounds[2] = {-1.2L, 1.2L};

sf::Sprite renderMandelbrot(sf::Texture& texture, unsigned int width, unsigned int height, int maxIterations, int colorId) {
    sf::Image image({width, height}, sf::Color::Black);

    unsigned int threadCount = std::thread::hardware_concurrency();

    if (threadCount == 0) {
        threadCount = 4;
    }

    std::vector<std::thread> threads;

    auto worker = [&](unsigned int yStart, unsigned int yEnd) {
        for (unsigned int y = yStart; y < yEnd; y++) {
            long double imaginary = imaginaryBounds[0] + (static_cast<long double>(y) / height) * (imaginaryBounds[1] - imaginaryBounds[0]);

            for (unsigned int x = 0; x < width; x++) {
                long double real = realBounds[0] + (static_cast<long double>(x) / width) * (realBounds[1] - realBounds[0]);

                int iterations = mandelbrot(real, imaginary, maxIterations);

                sf::Color pixelColor = color(iterations, maxIterations, colorId);
                image.setPixel({x, y}, pixelColor);
            }
        }
    };

    unsigned int rowsPerThread = height / threadCount;

    for (unsigned int t = 0; t < threadCount; t++) {
        unsigned int yStart = t * rowsPerThread;
        unsigned int yEnd = (t == threadCount - 1) ? height : yStart + rowsPerThread;

        threads.emplace_back(worker, yStart, yEnd);
    }

    for (auto& t : threads) {
        t.join();
    }

    if (!texture.loadFromImage(image)) {
        std::cout << "Image could not load." << std::endl;
    }

    return sf::Sprite(texture);
}