#include "MainMenu.h"


namespace ptd
{

    MainMenu::MainMenu()
    {
        window = new sf::RenderWindow(sf::VideoMode(1200, 1000), "My window");
        window->setFramerateLimit(60);

        Menu();
    }

    int MainMenu::Menu()
    {
        while (window->isOpen())
        {
            sf::Event event;
            while (window->pollEvent(event))
            {
                switch (event.type)
                {
                case sf::Event::KeyPressed:     //начало игры
                {
                    GameManager gm;
                    Interpreter inter(window, &gm);
                    break;
                }
                case sf::Event::Closed:         //закрытие окна
                {
                    window->close();
                    break;
                }
                }
            }
        }

        return 0;
    }

    MainMenu::~MainMenu()
    {
        delete window;
    }
}