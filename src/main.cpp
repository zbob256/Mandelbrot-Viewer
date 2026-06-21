#include <SFML/Graphics.hpp>
#include <iostream>
#include <algorithm>
#include <chrono>
#include "renderer.hpp"

const unsigned int ITERATION_INCREMENT = 50;
const int MINIMUM_ITERATIONS = 50;
const long double ZOOM_OUT_FACTOR = 2.5;
const long double PAN_FACTOR = 0.1;

unsigned int WIDTH = 800;
unsigned int HEIGHT = 600;

int MAX_ITERATIONS = 100;
int COLOR_ID = 0;

bool hasFirstClick = false;
bool fullscreen = false;
sf::Vector2i firstClick;

int main() {
    std::cout << "\n\n\nOpening window. Press [C] for controls." << std::endl;

    sf::RenderWindow window(sf::VideoMode({WIDTH, HEIGHT}), "Mandelbrot Viewer");
    sf::Cursor cursor(sf::Cursor::Type::Cross);
    sf::Image icon;
    bool iconLoaded = icon.loadFromFile("../assets/icon.png");
    
    window.setFramerateLimit(60);
    window.setMouseCursor(cursor);

    if (iconLoaded) {
        window.setIcon(icon);
    } else {
        std::cout << "Window icon failed to load." << std::endl;
    }
    
    sf::Texture texture;
    sf::Sprite mandelbrotImage = renderMandelbrot(texture, WIDTH, HEIGHT, MAX_ITERATIONS, COLOR_ID);

    while (window.isOpen()) {
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                std::cout << "Closing window.\n\n\n" << std::endl;

                window.close();
            } else if (const auto* resized = event->getIf<sf::Event::Resized>()) {
                WIDTH = resized->size.x;
                HEIGHT = resized->size.y;

                window.setView(sf::View(sf::FloatRect(sf::Vector2f(0.f, 0.f), sf::Vector2f(static_cast<float>(WIDTH),static_cast<float>(HEIGHT)))));

                mandelbrotImage = renderMandelbrot(texture, WIDTH, HEIGHT, MAX_ITERATIONS, COLOR_ID);
            } else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                switch (keyPressed->code) {
                    case sf::Keyboard::Key::Escape:
                        std::cout << "Closing window.\n\n\n" << std::endl;

                        window.close();

                        break;
                    case sf::Keyboard::Key::C:
                        std::cout << "\nControls:" << std::endl;
                        std::cout << "[Left Click] Two clicks make a rectangle for new view." << std::endl;
                        std::cout << "[Right Click] Zoom out." << std::endl;
                        std::cout << "[R] Reset view." << std::endl;
                        std::cout << "[F] Toggle fullscreen." << std::endl;
                        std::cout << "[T] Redraw and time current frame." << std::endl;
                        std::cout << "[Z] Undo first click." << std::endl;
                        std::cout << "[S] Correct aspect ratio." << std::endl;
                        std::cout << "[-/=] Decrease and increase max iterations." << std::endl;
                        std::cout << "[Arrow Keys] Move around." << std::endl;
                        std::cout << "[1 -> 0] Change color scheme." << std::endl;
                        std::cout << "[ESC] Quit.\n" << std::endl;
                        
                        break;
                    case sf::Keyboard::Key::R:
                        realBounds[0] = -2.2L;
                        realBounds[1] = 1.0L;
                        imaginaryBounds[0] = -1.2L;
                        imaginaryBounds[1] = 1.2L;

                        std::cout << "Reseting view." << std::endl;

                        mandelbrotImage = renderMandelbrot(texture, WIDTH, HEIGHT, MAX_ITERATIONS, COLOR_ID);
                        
                        break;
                    case sf::Keyboard::Key::F:
                        fullscreen = !fullscreen;

                        if (fullscreen) {
                            std::cout << "Entering fullscreen mode." << std::endl;

                            window.create(sf::VideoMode::getDesktopMode(), "Mandelbrot Viewer", sf::State::Fullscreen);
                        } else {
                            std::cout << "Exiting fullscreen mode." << std::endl;

                            window.create(sf::VideoMode({800, 600}), "Mandelbrot Viewer", sf::Style::Default);
                        }

                        WIDTH = window.getSize().x;
                        HEIGHT = window.getSize().y;

                        if (iconLoaded) {
                            window.setIcon(icon);
                        }

                        window.setMouseCursor(cursor);
                        window.setFramerateLimit(60);
                        window.setView(sf::View(sf::FloatRect(sf::Vector2f(0.f, 0.f), sf::Vector2f((float)WIDTH, (float)HEIGHT))));

                        mandelbrotImage = renderMandelbrot(texture, WIDTH, HEIGHT, MAX_ITERATIONS, COLOR_ID);

                        break;
                    case sf::Keyboard::Key::T: {
                        std::cout << "Redrawing and timing current frame." << std::endl;

                        auto start = std::chrono::high_resolution_clock::now();

                        mandelbrotImage = renderMandelbrot(texture, WIDTH, HEIGHT, MAX_ITERATIONS, COLOR_ID);

                        auto end = std::chrono::high_resolution_clock::now();
                        std::chrono::duration<double> seconds = end - start;

                        std::cout << "Render time: " << seconds.count() << " seconds" << std::endl;

                        break;
                    } case sf::Keyboard::Key::Z:
                        if (hasFirstClick) {
                            std::cout << "Undoing first click." << std::endl;
                        } else {
                            std::cout << "No first click to undo." << std::endl;
                        }
                        
                        hasFirstClick = false;

                        break;
                    case sf::Keyboard::Key::S: {
                        long double centerReal = (realBounds[0] + realBounds[1]) / 2.0;
                        long double centerImag = (imaginaryBounds[0] + imaginaryBounds[1]) / 2.0;

                        long double realRange = realBounds[1] - realBounds[0];
                        long double imagRange = realRange * (long double)HEIGHT / WIDTH;

                        imaginaryBounds[0] = centerImag - imagRange / 2.0;
                        imaginaryBounds[1] = centerImag + imagRange / 2.0;

                        std::cout << "Correcting aspect ratio." << std::endl;

                        mandelbrotImage = renderMandelbrot(texture, WIDTH, HEIGHT, MAX_ITERATIONS, COLOR_ID);

                        break;
                    } case sf::Keyboard::Key::Equal:
                        MAX_ITERATIONS += ITERATION_INCREMENT;

                        std::cout << "Increasing maximum iterations to " << MAX_ITERATIONS << "." << std::endl;

                        mandelbrotImage = renderMandelbrot(texture, WIDTH, HEIGHT, MAX_ITERATIONS, COLOR_ID);
                        
                        break;
                    case sf::Keyboard::Key::Hyphen:
                        MAX_ITERATIONS -= ITERATION_INCREMENT;

                        if (MAX_ITERATIONS < MINIMUM_ITERATIONS) {
                            MAX_ITERATIONS = MINIMUM_ITERATIONS;
                        }

                        std::cout << "Decreasing maximum iterations to " << MAX_ITERATIONS << "." << std::endl;
                        
                        mandelbrotImage = renderMandelbrot(texture, WIDTH, HEIGHT, MAX_ITERATIONS, COLOR_ID);
                        
                        break;
                    case sf::Keyboard::Key::Left: {
                        long double shift = (realBounds[1] - realBounds[0]) * PAN_FACTOR;

                        realBounds[0] -= shift;
                        realBounds[1] -= shift;
                        
                        std::cout << "Shifting left." << std::endl;

                        mandelbrotImage = renderMandelbrot(texture, WIDTH, HEIGHT, MAX_ITERATIONS, COLOR_ID);

                        break;
                    } case sf::Keyboard::Key::Right: {
                        long double shift = (realBounds[1] - realBounds[0]) * PAN_FACTOR;

                        realBounds[0] += shift;
                        realBounds[1] += shift;

                        std::cout << "Shifting right." << std::endl;

                        mandelbrotImage = renderMandelbrot(texture, WIDTH, HEIGHT, MAX_ITERATIONS, COLOR_ID);

                        break;
                    } case sf::Keyboard::Key::Up: {
                        long double shift = (imaginaryBounds[1] - imaginaryBounds[0]) * PAN_FACTOR;

                        imaginaryBounds[0] -= shift;
                        imaginaryBounds[1] -= shift;

                        std::cout << "Shifting up." << std::endl;

                        mandelbrotImage = renderMandelbrot(texture, WIDTH, HEIGHT, MAX_ITERATIONS, COLOR_ID);

                        break;
                    } case sf::Keyboard::Key::Down: {
                        long double shift = (imaginaryBounds[1] - imaginaryBounds[0]) * PAN_FACTOR;

                        imaginaryBounds[0] += shift;
                        imaginaryBounds[1] += shift;

                        std::cout << "Shifting down." << std::endl;

                        mandelbrotImage = renderMandelbrot(texture, WIDTH, HEIGHT, MAX_ITERATIONS, COLOR_ID);

                        break;
                    } case sf::Keyboard::Key::Num0:
                        COLOR_ID = 0;

                        std::cout << "Changing color to greyscale." << std::endl;

                        mandelbrotImage = renderMandelbrot(texture, WIDTH, HEIGHT, MAX_ITERATIONS, COLOR_ID);
                        
                        break;
                    case sf::Keyboard::Key::Num1:
                        COLOR_ID = 1;

                        std::cout << "Changing color to red." << std::endl;

                        mandelbrotImage = renderMandelbrot(texture, WIDTH, HEIGHT, MAX_ITERATIONS, COLOR_ID);

                        break;
                    case sf::Keyboard::Key::Num2:
                        COLOR_ID = 2;

                        std::cout << "Changing color to orange." << std::endl;

                        mandelbrotImage = renderMandelbrot(texture, WIDTH, HEIGHT, MAX_ITERATIONS, COLOR_ID);

                        break;
                    case sf::Keyboard::Key::Num3:
                        COLOR_ID = 3;

                        std::cout << "Changing color to yellow." << std::endl;

                        mandelbrotImage = renderMandelbrot(texture, WIDTH, HEIGHT, MAX_ITERATIONS, COLOR_ID);

                        break;
                    case sf::Keyboard::Key::Num4:
                        COLOR_ID = 4;

                        std::cout << "Changing color to green." << std::endl;

                        mandelbrotImage = renderMandelbrot(texture, WIDTH, HEIGHT, MAX_ITERATIONS, COLOR_ID);

                        break;
                    case sf::Keyboard::Key::Num5:
                        COLOR_ID = 5;

                        std::cout << "Changing color to cyan." << std::endl;

                        mandelbrotImage = renderMandelbrot(texture, WIDTH, HEIGHT, MAX_ITERATIONS, COLOR_ID);

                        break;
                    case sf::Keyboard::Key::Num6:
                        COLOR_ID = 6;

                        std::cout << "Changing color to blue." << std::endl;

                        mandelbrotImage = renderMandelbrot(texture, WIDTH, HEIGHT, MAX_ITERATIONS, COLOR_ID);

                        break;
                    case sf::Keyboard::Key::Num7:
                        COLOR_ID = 7;

                        std::cout << "Changing color to purple." << std::endl;

                        mandelbrotImage = renderMandelbrot(texture, WIDTH, HEIGHT, MAX_ITERATIONS, COLOR_ID);

                        break;
                    case sf::Keyboard::Key::Num8:
                        COLOR_ID = 8;

                        std::cout << "Changing color to sinebow." << std::endl;

                        mandelbrotImage = renderMandelbrot(texture, WIDTH, HEIGHT, MAX_ITERATIONS, COLOR_ID);

                        break;
                    case sf::Keyboard::Key::Num9:
                        COLOR_ID = 9;

                        std::cout << "Changing color to ocean." << std::endl;

                        mandelbrotImage = renderMandelbrot(texture, WIDTH, HEIGHT, MAX_ITERATIONS, COLOR_ID);

                        break;
                    default:
                        break;
                }
            } else if (const auto* mouse = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (mouse->button == sf::Mouse::Button::Left) {
                    if (!hasFirstClick) {
                        firstClick = mouse->position;
                        hasFirstClick = true;

                        std::cout << "First clicked registered." << std::endl;
                    } else {
                        sf::Vector2i secondClick = mouse->position;
                        hasFirstClick = false;

                        long double real1 = realBounds[0] + (long double)firstClick.x / WIDTH * (realBounds[1] - realBounds[0]);
                        long double real2 = realBounds[0] + (long double)secondClick.x / WIDTH * (realBounds[1] - realBounds[0]);

                        long double imag1 = imaginaryBounds[0] + (long double)firstClick.y / HEIGHT * (imaginaryBounds[1] - imaginaryBounds[0]);
                        long double imag2 = imaginaryBounds[0] + (long double)secondClick.y / HEIGHT * (imaginaryBounds[1] - imaginaryBounds[0]);

                        realBounds[0] = std::min(real1, real2);
                        realBounds[1] = std::max(real1, real2);

                        imaginaryBounds[0] = std::min(imag1, imag2);
                        imaginaryBounds[1] = std::max(imag1, imag2);

                        std::cout << "Second click registered, zooming in." << std::endl;

                        mandelbrotImage = renderMandelbrot(texture, WIDTH, HEIGHT, MAX_ITERATIONS, COLOR_ID);
                    }
                } else if (mouse->button == sf::Mouse::Button::Right) {
                    long double realCenter = (realBounds[0] + realBounds[1]) / 2;
                    long double imagCenter = (imaginaryBounds[0] + imaginaryBounds[1]) / 2;

                    long double realRange = (realBounds[1] - realBounds[0]) * ZOOM_OUT_FACTOR / 2;
                    long double imagRange = (imaginaryBounds[1] - imaginaryBounds[0]) * ZOOM_OUT_FACTOR / 2;

                    realBounds[0] = realCenter - realRange;
                    realBounds[1] = realCenter + realRange;

                    imaginaryBounds[0] = imagCenter - imagRange;
                    imaginaryBounds[1] = imagCenter + imagRange;

                    std::cout << "Zooming out." << std::endl;

                    mandelbrotImage = renderMandelbrot(texture, WIDTH, HEIGHT, MAX_ITERATIONS, COLOR_ID);
                }
            }
        }

        window.clear(sf::Color::Black);

        window.draw(mandelbrotImage);

        window.display();
    }
}
