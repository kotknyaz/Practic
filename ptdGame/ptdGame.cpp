#include "ptdGame.h"

namespace ptd {
    const double FOV = (M_PI / 3);
    const double FOV_DIVIDE_BY2 = FOV / 2;
    const double CAMERA_ROTATING_KOEF = 1.f / 150.f;


    MainMenu::MainMenu()
    {
        window = new sf::RenderWindow(sf::VideoMode(800, 600), "My window");
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


    std::vector<VisibleWall> GameManager::GetVisibleWalls()
    {
        /*
        на вход: 
            у класса GameManager есть поля: 
            PlayerPos - координата игрока
            walls - vector со стенами (нулевой елем. - начало первой стены; первый элем. - конец первой стены; второй элем. - начало второй стены и т.д.)
            view - центр взгляда игрока (в радианах; ноль это вправо, 90 градусов - вверх) (чтобы узнать край вида игрока есть константа FOV_DIVIDE_BY2)
        на выход:
            массив стен аналогичный с walls (начало первой; конец первой; начало второй; конец второй; начало третьей ...)
                (для этого я думал использовать пока нереализованную функцию о которой мы говорили - CrossingRayLine (или что то такое))
        */


        std::vector<VisibleWall> t;
        return t;
    }

    CrossingRayLineInfo CrossingRayLine(const Wall& wall, double angle)
    {
        

        CrossingRayLineInfo a;
        return a;
    }



    GameManager::GameManager()
    {
        clock = new sf::Clock();

        walls.push_back(new Wall(Coord(-2, 4), Coord(3, 4)));
        walls.push_back(new Wall(Coord(3, 4), Coord(3, -4)));
        walls.push_back(new Wall(Coord(3, -4), Coord(-2, -4)));
        walls.push_back(new Wall(Coord(-2, -4), Coord(-2, 4)));
    }

    PrintInfo2D GameManager::Update2D(UpdateInfo& updateInfo)
    {
        PrintInfo2D printInfo;
        view += updateInfo.viewChange * clock->getElapsedTime().asMilliseconds() * CAMERA_ROTATING_KOEF;
        //std::cout << std::endl << 1.f / clock->getElapsedTime().asSeconds() << std::endl;
        clock->restart();
        
        //----------------------------------------- Изменение положения игрока относительно направления взгляда - начало -  -----------------------------------------
        playerPos.x += updateInfo.playerPosChange * cos(view);
        playerPos.y += updateInfo.playerPosChange * sin(view);
        std::cout << view << std::endl;

        //----------------------------------------- Изменение положения игрока относительно направления взгляда - конец -  -----------------------------------------

        // temp
        printInfo.walls = walls;
        printInfo.playerPos.x += playerPos.x;
        printInfo.playerPos.y += playerPos.y;
        //std::cout << printInfo.playerPos.x << " _ " << printInfo.playerPos.y << std::endl;
        printInfo.viewLeft = view + FOV_DIVIDE_BY2;
        printInfo.viewRight = view - FOV_DIVIDE_BY2;

        return printInfo;
    }



    Interpreter::Interpreter(sf::RenderWindow* activeWindow, GameManager* gameManager) : window(activeWindow), gm(gameManager)
    {
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
                /*
                case sf::Event::KeyPressed:         //закрытие окна
                {
                    if (event.key.code == sf::Keyboard::A)
                    {
                        toSent.viewChange += 1;
                    }
                    if (event.key.code == sf::Keyboard::D)
                    {
                        toSent.viewChange -= 1;
                    }
                    break;
                }
                */
                }
            }

            // поворот камеры
            toSent.viewChange = 0;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)))
            {
                toSent.viewChange += 1;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)))
            {
                toSent.viewChange -= 1;
            }
            
            // движение вперёд-назад (возможно временно)
            toSent.playerPosChange = 0;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)))
            {
                toSent.playerPosChange += 0.05;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)))
            {
                toSent.playerPosChange -= 0.05;
            }

            Print2D( gm->Update2D(toSent) );
        }
    }

    int Interpreter::Print2D(PrintInfo2D& info)
    {
        window->clear(sf::Color::Black);
        //вид сверху; центр - игрок; все стены х koef; y - вверх, х - вправо
        // стены - синий; взгляд - зеленый; видимые стены - красный; игрок - белый
        double koef = 45;
        double rayLenth = 500;
        sf::VertexArray vertexWalls(sf::PrimitiveType::Lines, 0);
        double halfScreenX = window->getSize().x / 2;
        double halfScreenY = window->getSize().y / 2;

        for (int i = 0; i < info.walls.size(); ++i)
        {
            //std::cout << i << "\n";
            //std::cout << info.walls[i]->coord[0].x << " " << info.walls[i]->coord[0].y << std::endl;
            //std::cout << info.walls[i]->coord[1].x << " " << info.walls[i]->coord[1].y << std::endl;
            vertexWalls.append(sf::Vertex(sf::Vector2f(halfScreenX - (info.walls[i]->coord[0].x + info.playerPos.x) * koef,
                halfScreenY - (info.walls[i]->coord[0].y - info.playerPos.y) * koef), sf::Color::Blue));
            vertexWalls.append(sf::Vertex(sf::Vector2f(halfScreenX - (info.walls[i]->coord[1].x + info.playerPos.x) * koef,
                halfScreenY - (info.walls[i]->coord[1].y - info.playerPos.y) * koef), sf::Color::Blue));
        }
        
        sf::VertexArray vertexPlayer(sf::PrimitiveType::Lines);
        vertexPlayer.append(sf::Vertex(sf::Vector2f(halfScreenX - 5, halfScreenY - 5), sf::Color::White));
        vertexPlayer.append(sf::Vertex(sf::Vector2f(halfScreenX + 5, halfScreenY + 5), sf::Color::White));
        vertexPlayer.append(sf::Vertex(sf::Vector2f(halfScreenX + 5, halfScreenY - 5), sf::Color::White));
        vertexPlayer.append(sf::Vertex(sf::Vector2f(halfScreenX - 5, halfScreenY + 5), sf::Color::White));

        sf::VertexArray vertexRays(sf::PrimitiveType::Lines, 0);
        vertexRays.append(sf::Vertex(sf::Vector2f(halfScreenX, halfScreenY), sf::Color::Green));
        vertexRays.append(sf::Vertex(sf::Vector2f(halfScreenX + cos(info.viewLeft) * rayLenth,
            halfScreenY - sin(info.viewLeft) * rayLenth), sf::Color::Green));
        vertexRays.append(sf::Vertex(sf::Vector2f(halfScreenX, halfScreenY), sf::Color::Green));
        vertexRays.append(sf::Vertex(sf::Vector2f(halfScreenX + cos(info.viewRight) * rayLenth,
            halfScreenY - sin(info.viewRight) * rayLenth), sf::Color::Green));

        /*
        sf::VertexArray vertexVisibleWalls(sf::PrimitiveType::Lines, 0);
        for (int i = 0; i < info.visibleWalls.size(); ++i)
        {
            vertexWalls.append(sf::Vertex(sf::Vector2f(halfScreenX - (info.visibleWalls[i].x + info.playerPos.x) * koef,
                halfScreenY - (info.visibleWalls[i].y - info.playerPos.y) * koef), sf::Color::Red));
        }
        */


        window->draw(vertexPlayer);
        window->draw(vertexWalls);
        window->draw(vertexRays);
        //window->draw(vertexVisibleWalls);
        window->display();
        return 0;
    }



    Coord::Coord(double newX, double newY) : x(newX), y(newY) {}

    Coord::Coord() {}


    PrintInfo2D::PrintInfo2D()
    {
        playerPos = Coord(0.f, 0.f);
        //walls = std::vector<Wall*>();
    }


    Wall::Wall() {}

    Wall::Wall(Coord a, Coord b)
    {
        coord[0].x = a.x;
        coord[0].y = a.y;
        coord[1].x = b.x;
        coord[1].y = b.y;
    }

    VisibleWall::VisibleWall() {}
}
