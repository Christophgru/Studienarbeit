#include <SFML/Graphics.hpp>
#include <iostream>

int main()
{
    sf::RenderWindow window( { 1920u, 1080u }, "CMake SFML Project" );
    window.setFramerateLimit(144);

    while (window.isOpen())
    {
        for (auto event = sf::Event{}; window.pollEvent(event);)
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }

        window.clear();
        window.display();
    }
}

