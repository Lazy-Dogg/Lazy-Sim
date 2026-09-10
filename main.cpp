#include <SFML/Graphics.hpp>
#include <thread>
#include <chrono>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <algorithm>

const int WIDTH = 200; //400
const int HEIGHT = 150; //300
const int PIXEL_SIZE = 6; //3

// reminder, revert this:
const int WINDOW_WIDTH = 1200; // <--
const int WINDOW_HEIGHT = 900; // <--

bool Running = true;

bool keys[256] = {false};
bool Mouses[32] = {false};

int SelectedMat = 1;
bool HoldingLeftMouse = false;

int grid[HEIGHT][WIDTH] = {};
int gridHeat[HEIGHT][WIDTH] = {};

int Wind = 1;

sf::Font MainFont;

int RandomNumber(int High)
{
    return rand() % High + 1;
}

void Update()
{
    for (int y = HEIGHT - 2; y >= 0; y--)
    {
        for (int x = 0; x < WIDTH; x++)
        {

            if (y <= 0)
              grid[y][x] = 0;

            if (gridHeat[y][x] < 0)
              gridHeat[y][x] = 0;

            if (gridHeat[y][x] > 255)
              gridHeat[y][x] = 255;

            //sand
            if (grid[y][x] == 1)
            {
                int RanodmStepNumber = RandomNumber(2);

                if ((grid[y + 1][x] == 0 || grid[y + 1][x] == 3) && (grid[y + RanodmStepNumber][x] == 0 || grid[y + RanodmStepNumber][x] == 3))
                {
                    if (grid[y + 1][x] == 3 || grid[y + RanodmStepNumber][x] == 3)
                    {
                      grid[y + RanodmStepNumber][x] = 1;
                      grid[y][x] = 3;
                    }
                    else
                    {
                      grid[y + RanodmStepNumber][x] = 1;
                      grid[y][x] = 0;
                    }
                }
                else
                {
                    if (grid[y + 1][x + 1] == 0 && grid[y][x + 1] == 0)
                    {
                        if (RanodmStepNumber == 1)
                        {
                            grid[y][x + 1] = 1;
                            grid[y][x] = 0;
                        }
                    }

                    if (grid[y + 1][x - 1] == 0 && grid[y][x - 1] == 0)
                    {
                        if (RanodmStepNumber == 2)
                        {
                            grid[y][x - 1] = 1;
                            grid[y][x] = 0;
                        }
                    }
                }
            }

            //water
            if (grid[y][x] == 3)
            {
                int RanodmStepNumber = RandomNumber(2);

                if (grid[y + 1][x] == 0 && grid[y + RanodmStepNumber][x] == 0)
                {
                    grid[y + RanodmStepNumber][x] = 3;
                    grid[y][x] = 0;
                }
                else
                {
                    if (grid[y][x + 1] == 0)
                    {
                        if (RanodmStepNumber == 1)
                        {
                            grid[y][x + 1] = 3;
                            grid[y][x] = 0;
                        }
                    }

                    if (grid[y][x - 1] == 0)
                    {
                        if (RanodmStepNumber == 2)
                        {
                            grid[y][x - 1] = 3;
                            grid[y][x] = 0;
                        }
                    }
                }
            }

            //heat
            if (gridHeat[y][x] > 0)
            {

                int Heat = gridHeat[y][x];

                //down
                if (y + 1 < HEIGHT)
                {
                    int Transfer = Heat / 10;

                    if (gridHeat[y + 1][x] < Heat)
                    {
                        gridHeat[y + 1][x] += Transfer;
                        gridHeat[y][x] -= Transfer;
                    }
                }

                //up
                if (y - 1 >= 0)
                {
                    int Transfer = Heat / 10;

                    if (gridHeat[y - 1][x] < Heat)
                    {
                        gridHeat[y - 1][x] += Transfer;
                        gridHeat[y][x] -= Transfer;
                    }
                }

                //right
                if (x + 1 < WIDTH)
                {
                    int Transfer = Heat / 10;

                    if (gridHeat[y][x + 1] < Heat)
                    {
                        gridHeat[y][x + 1] += Transfer;
                        gridHeat[y][x] -= Transfer;
                    }
                }

                //left
                if (x - 1 >= 0)
                {
                    int Transfer = Heat / 10;

                    if (gridHeat[y][x - 1] < Heat)
                    {
                        gridHeat[y][x - 1] += Transfer;
                        gridHeat[y][x] -= Transfer;
                    }
                }

                if (gridHeat[y][x] < 10)
                    gridHeat[y][x] = 0;

                //for water
                if (grid[y][x] == 3 && gridHeat[y][x] >= 50)
                {
                    grid[y][x] = 5;
                }
            }

        }
    }

    //steam
    for (int y = 1; y < HEIGHT; y++)
    {
        for (int x = 0; x < WIDTH; x++)
        {
            if (grid[y][x] == 5)
            {
                int RandomStepNumber = RandomNumber(2);

                if (y - RandomStepNumber >= 0 && grid[y - RandomStepNumber][x] == 0 && grid[y - 1][x] == 0)
                {
                    grid[y - RandomStepNumber][x] = 5;
                    grid[y][x] = 0;
                }
                else
                {
                    if (RandomStepNumber == 1 && x + 1 < WIDTH && grid[y][x + 1] == 0)
                    {
                        grid[y][x + 1] = 5;
                        grid[y][x] = 0;
                    }
                    else if (RandomStepNumber == 2 && x - 1 >= 0 && grid[y][x - 1] == 0)
                    {
                        grid[y][x - 1] = 5;
                        grid[y][x] = 0;
                    }
                }

                if (grid[y-1][x] == 3 || grid[y-1][x] == 1)
                {
                    grid[y][x] = grid[y-1][x];
                    grid[y-1][x] = 5;
                }

            }
        }
    }
}

int main()
{
    srand(time(0));

    sf::RenderWindow window(
        sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}),
        "Particle Sim"
    );

    if (!MainFont.openFromFile("KiwiSoda.ttf"))
    {
       std::cout << "Error Loading Font!\n";
    }

    sf::Text materialText(MainFont);
    materialText.setCharacterSize(24);
    materialText.setFillColor(sf::Color::Yellow);
    materialText.setPosition({10.f, 10.f});
    materialText.setString("Material: Sand");

    while (window.isOpen())
    {
        while (auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }

            if (event->is<sf::Event::KeyPressed>())
            {
                auto key = event->getIf<sf::Event::KeyPressed>();
                if (key)
                {
                    keys[static_cast<int>(key->scancode)] = true;
                }
            }

            if (event->is<sf::Event::KeyReleased>())
            {
                auto key = event->getIf<sf::Event::KeyReleased>();
                if (key)
                {
                    keys[static_cast<int>(key->scancode)] = false;
                }
            }

            if (auto mousePress = event->getIf<sf::Event::MouseButtonPressed>())
            {
                Mouses[static_cast<int>(mousePress->button)] = true;
            }

            if (auto mouseRelease = event->getIf<sf::Event::MouseButtonReleased>())
            {
                Mouses[static_cast<int>(mouseRelease->button)] = false;
            }
        }

        if (Mouses[static_cast<int>(sf::Mouse::Button::Left)])
        {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            int gridX = mousePos.x / PIXEL_SIZE;
            int gridY = mousePos.y / PIXEL_SIZE;

            if ((0 <= gridY && gridY < HEIGHT) && (0 <= gridX && gridX < WIDTH))
            {
                if (SelectedMat == 6)
                {
                    gridHeat[gridY][gridX] += 255;
                }
                else
                {
                    grid[gridY][gridX] = SelectedMat;
                }
            }
        }

        if (Mouses[3])
        {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            int gridX = mousePos.x / PIXEL_SIZE;
            int gridY = mousePos.y / PIXEL_SIZE;

            if ((0 <= gridY && gridY < HEIGHT) && (0 <= gridX && gridX < WIDTH))
            {
                grid[gridY][gridX] = 0;
            }
        }

        if (keys[static_cast<int>(sf::Keyboard::Scancode::Num1)]) {
            SelectedMat = 1;
            materialText.setString("Material: Sand");
            materialText.setFillColor(sf::Color::Yellow);
        }

        if (keys[static_cast<int>(sf::Keyboard::Scancode::Num2)]) {
            SelectedMat = 2;
            materialText.setString("Material: Rock");
            materialText.setFillColor(sf::Color{128, 128, 128, 255});
        }

        if (keys[static_cast<int>(sf::Keyboard::Scancode::Num3)]) {
            SelectedMat = 3;
            materialText.setString("Material: Water");
            materialText.setFillColor(sf::Color::Blue);
        }

        if (keys[static_cast<int>(sf::Keyboard::Scancode::Num4)]) {
            SelectedMat = 4;
            materialText.setString("Material: Wood");
            materialText.setFillColor(sf::Color{139, 69, 19, 255});
        }

        if (keys[static_cast<int>(sf::Keyboard::Scancode::Num5)]) {
            SelectedMat = 5;
            materialText.setString("Material: Steam/Smoke");
            materialText.setFillColor(sf::Color{140, 140, 140, 255});
        }

        if (keys[static_cast<int>(sf::Keyboard::Scancode::Num6)]) {
            SelectedMat = 6;
            materialText.setString("Material: Heat");
            materialText.setFillColor(sf::Color{255, 232, 8, 255});
        }

        if (keys[static_cast<int>(sf::Keyboard::Scancode::Space)]) {
            Running = !Running;
        }

        if (keys[static_cast<int>(sf::Keyboard::Key::R)])
        {
            std::memset(grid, 0, sizeof(grid));
            std::memset(gridHeat, 0, sizeof(grid));
        }

        if (Running)
        {
            Update();
        }

        window.clear(sf::Color{180, 210, 230, 255});

        for (int y = 0; y < HEIGHT; y++)
        {
            for (int x = 0; x < WIDTH; x++)
            {
                //sand
                if (grid[y][x] == 1)
                {
                    sf::RectangleShape pixel(sf::Vector2f(PIXEL_SIZE, PIXEL_SIZE));
                    pixel.setPosition(sf::Vector2f(x * PIXEL_SIZE, y * PIXEL_SIZE));
                    pixel.setFillColor(sf::Color::Yellow);
                    window.draw(pixel);
                }
                //rock
                if (grid[y][x] == 2)
                {
                    sf::RectangleShape pixel(sf::Vector2f(PIXEL_SIZE, PIXEL_SIZE));
                    pixel.setPosition(sf::Vector2f(x * PIXEL_SIZE, y * PIXEL_SIZE));
                    pixel.setFillColor(sf::Color{128, 128, 128, 255});
                    window.draw(pixel);
                }
                //water
                if (grid[y][x] == 3)
                {
                    sf::RectangleShape pixel(sf::Vector2f(PIXEL_SIZE, PIXEL_SIZE));
                    pixel.setPosition(sf::Vector2f(x * PIXEL_SIZE, y * PIXEL_SIZE));
                    pixel.setFillColor(sf::Color::Blue);
                    window.draw(pixel);
                }
                //wood
                if (grid[y][x] == 4)
                {
                    sf::RectangleShape pixel(sf::Vector2f(PIXEL_SIZE, PIXEL_SIZE));
                    pixel.setPosition(sf::Vector2f(x * PIXEL_SIZE, y * PIXEL_SIZE));
                    pixel.setFillColor(sf::Color{139, 69, 19, 255});
                    window.draw(pixel);
                }
                //steam
                if (grid[y][x] == 5)
                {
                    sf::RectangleShape pixel(sf::Vector2f(PIXEL_SIZE, PIXEL_SIZE));
                    pixel.setPosition(sf::Vector2f(x * PIXEL_SIZE, y * PIXEL_SIZE));
                    pixel.setFillColor(sf::Color{140, 140, 140, 140});
                    window.draw(pixel);
                }
                //heat
                if (gridHeat[y][x] > 0)
                {
                    sf::RectangleShape pixel(sf::Vector2f(PIXEL_SIZE, PIXEL_SIZE));
                    pixel.setPosition(sf::Vector2f(x * PIXEL_SIZE, y * PIXEL_SIZE));
                    pixel.setFillColor(sf::Color{255, 232, 8, static_cast<uint8_t>(std::clamp(gridHeat[y][x], 0, 255))});
                    window.draw(pixel);
                }

            }
        }

        window.draw(materialText);
        window.display();
        
        std::this_thread::sleep_for(std::chrono::milliseconds(7));
    }

    return 0;
}

