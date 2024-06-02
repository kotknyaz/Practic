#include "Interpreter.h"

namespace ptd
{
    const double FOV = (M_PI / 3);
    const double FOV_DIVIDE_BY2 = FOV / 2;
    const double CAMERA_ROTATING_KOEF = 1.f / 150.f;

    Interpreter* Interpreter::_instance = 0;
    Interpreter* Interpreter::Instance(sf::RenderWindow* activeWindow, GameManager* gameManager) {
        if (_instance == 0)
        {
            _instance = new Interpreter(activeWindow, gameManager);
        }
        return _instance;
    }

    Interpreter::Interpreter(sf::RenderWindow* activeWindow, GameManager* gameManager) : window(activeWindow), gm(gameManager)
    {
        clock = new sf::Clock();

        GameProcess();
    }

    int Interpreter::GameProcess()
    {

        while (window->isOpen())
        {
            sf::Event event;

            UpdateInfo toSent;
            while (window->pollEvent(event))
            {
                switch (event.type)
                {
                case sf::Event::Closed:         //закрытие окна
                {
                    window->close();
                    break;
                }
                }
            }

            // поворот камеры
            toSent.viewChange = 0;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)))
            {
                toSent.viewChange += clock->getElapsedTime().asMilliseconds();
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)))
            {
                toSent.viewChange -= clock->getElapsedTime().asMilliseconds();
            }

            // движение вперёд-назад (возможно временно)
            toSent.playerPosChange = 0;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)))
            {
                toSent.playerPosChange += clock->getElapsedTime().asMilliseconds();
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)))
            {
                toSent.playerPosChange -= clock->getElapsedTime().asMilliseconds();
            }
            clock->restart();


            if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Tab))
                Print(gm->Update3D(toSent));
            else
                Print(gm->Update2D(toSent));
        }
        return 0;
    }



    int Interpreter::Print(PrintInfo3D& info)
    {
        double koef = 0.45;
        //double k1 = 0.5;
        sf::VertexArray verWalls(sf::PrimitiveType::Lines, 0);

        for (int i = 0; i < info.walls.size(); ++i)
        {
            double x, y_half;
            x = window->getSize().x * (info.walls[i].angle + FOV_DIVIDE_BY2) / FOV;
            y_half = koef * window->getSize().y / info.walls[i].distance;

            if (i > 0)
            {
                int t = (i - 1) * 6;
                verWalls.append(sf::Vertex(verWalls[t].position, sf::Color::Red));
                verWalls.append(sf::Vertex(sf::Vector2f(x, window->getSize().y / 2 - y_half), sf::Color::Red));

                verWalls.append(sf::Vertex(verWalls[t + 1].position, sf::Color::Green));
                verWalls.append(sf::Vertex(sf::Vector2f(x, window->getSize().y / 2 + y_half), sf::Color::Green));
            }

            if (i == 0)
            {
                verWalls.append(sf::Vertex(sf::Vector2f(0, window->getSize().y / 2 - y_half), sf::Color::Blue));
                verWalls.append(sf::Vertex(sf::Vector2f(0, window->getSize().y / 2 + y_half), sf::Color::Blue));
            }
            else
            {
                verWalls.append(sf::Vertex(sf::Vector2f(x, window->getSize().y / 2 - y_half), sf::Color::Blue));
                verWalls.append(sf::Vertex(sf::Vector2f(x, window->getSize().y / 2 + y_half), sf::Color::Blue));
            }
        }

        window->clear(sf::Color::White);
        window->draw(verWalls);
        window->display();
        return 0;
    }

    int Interpreter::Print(PrintInfo2D& info)
    {
        window->clear(sf::Color::Black);
        //вид сверху; центр - игрок; все стены х koef; y - вверх, х - вправо
        // стены - синий; взгляд - зеленый; видимые стены - красный; игрок - белый
        double koef = 45;
        double ray = info.viewLeft;
        double interval = 0.001;
        sf::VertexArray vertexWalls(sf::PrimitiveType::Lines, 0);
        double halfScreenX = window->getSize().x / 2;
        double halfScreenY = window->getSize().y / 2;

        for (int i = 0; i < info.walls.size(); ++i)
        {
            vertexWalls.append(sf::Vertex(sf::Vector2f(halfScreenX - (-info.walls[i]->coord[0].x + info.playerPos.x) * koef,
                halfScreenY - (info.walls[i]->coord[0].y - info.playerPos.y) * koef), sf::Color::Blue));
            vertexWalls.append(sf::Vertex(sf::Vector2f(halfScreenX - (-info.walls[i]->coord[1].x + info.playerPos.x) * koef,
                halfScreenY - (info.walls[i]->coord[1].y - info.playerPos.y) * koef), sf::Color::Blue));
        }
        for (int i = 0; i < info.visibleWalls.size(); ++i)
        {
            vertexWalls.append(sf::Vertex(sf::Vector2f(halfScreenX - (-info.visibleWalls[i].coord[0].x + info.playerPos.x) * koef,
                halfScreenY - (info.visibleWalls[i].coord[0].y - info.playerPos.y) * koef), sf::Color::Red));
            vertexWalls.append(sf::Vertex(sf::Vector2f(halfScreenX - (-info.visibleWalls[i].coord[1].x + info.playerPos.x) * koef,
                halfScreenY - (info.visibleWalls[i].coord[1].y - info.playerPos.y) * koef), sf::Color::Red));
        }

        sf::VertexArray vertexPlayer(sf::PrimitiveType::Lines);
        vertexPlayer.append(sf::Vertex(sf::Vector2f(halfScreenX - 5, halfScreenY - 5), sf::Color::White));
        vertexPlayer.append(sf::Vertex(sf::Vector2f(halfScreenX + 5, halfScreenY + 5), sf::Color::White));
        vertexPlayer.append(sf::Vertex(sf::Vector2f(halfScreenX + 5, halfScreenY - 5), sf::Color::White));
        vertexPlayer.append(sf::Vertex(sf::Vector2f(halfScreenX - 5, halfScreenY + 5), sf::Color::White));

        sf::VertexArray vertexRays(sf::PrimitiveType::Lines, 0);
        for (int i = 0; i < info.VRL.size(); i++) {
            vertexRays.append(sf::Vertex(sf::Vector2f(halfScreenX, halfScreenY), sf::Color::Green));
            vertexRays.append(sf::Vertex(sf::Vector2f(halfScreenX + cos(ray) * info.VRL[i].length * koef,
                halfScreenY - sin(ray) * info.VRL[i].length * koef), sf::Color::Green));
            ray -= interval;
        }

        window->draw(vertexRays);
        window->draw(vertexPlayer);
        window->draw(vertexWalls);
        window->display();
        return 0;
    }

    Interpreter::~Interpreter()
    {
        delete clock;
    }

}