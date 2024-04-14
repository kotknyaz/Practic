#include "ptdGame.h"

namespace ptd {
    const double FOV = (M_PI / 3);
    const double FOV_DIVIDE_BY2 = FOV / 2;



    MainMenu::MainMenu()
    {
        window = new sf::RenderWindow(sf::VideoMode(800, 600), "My window");

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
                case sf::Event::KeyPressed:     //������ ����
                {
                    GameManager gm;
                    Interpreter inter(window, &gm);
                    break;
                }
                case sf::Event::Closed:         //�������� ����
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
        �� ����: 
            � ������ GameManager ���� ����: 
            PlayerPos - ���������� ������
            walls - vector �� ������� (������� ����. - ������ ������ �����; ������ ����. - ����� ������ �����; ������ ����. - ������ ������ ����� � �.�.)
            view - ����� ������� ������ (� ��������; ���� ��� ������, 90 �������� - �����) (����� ������ ���� ���� ������ ���� ��������� FOV_DIVIDE_BY2)
        �� �����:
            ������ ���� ����������� � walls (������ ������; ����� ������; ������ ������; ����� ������; ������ ������� ...)
                (��� ����� � ����� ������������ ���� ��������������� ������� � ������� �� �������� - GetCrossingDot (��� ��� �� �����))
        */
        std::vector<VisibleWall> t;
        return t;
    }

    GameManager::GameManager()
    {
        walls.push_back(new Wall(Coord(-2, 4), Coord(3, 4)));
        walls.push_back(new Wall(Coord(3, 4), Coord(3, -4)));
        walls.push_back(new Wall(Coord(3, -4), Coord(-2, -4)));
        walls.push_back(new Wall(Coord(-2, -4), Coord(-2, 4)));
    }

    PrintInfo2D GameManager::Update2D(UpdateInfo& updateInfo)
    {
        PrintInfo2D printInfo;

        // temp
        printInfo.walls = walls;
        printInfo.playerPos.x = playerPos.x;
        printInfo.playerPos.y = playerPos.y;
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
            while (window->pollEvent(event))
            {
                switch (event.type)
                {
                case sf::Event::Closed:         //�������� ����
                {
                    window->close();
                    break;
                }
                }
            }
            UpdateInfo toSent;
            toSent.view = 0;
            Print2D( gm->Update2D(toSent) );
        }
    }

    int Interpreter::Print2D(PrintInfo2D& info)
    {
        window->clear(sf::Color::Black);
        //��� ������; ����� - �����; ��� ����� � koef; y - �����, � - ������
        // ����� - �����; ������ - �������; ������� ����� - �������; ����� - �����
        double koef = 40;
        double rayLenth = 500;
        sf::VertexArray vertexWalls(sf::PrimitiveType::Lines, 0);
        double halfScreenX = window->getSize().x / 2;
        double halfScreenY = window->getSize().y / 2;

        for (int i = 0; i < info.walls.size(); ++i)
        {
            std::cout << i << "\n";
            std::cout << info.walls[i]->coord[0].x << " " << info.walls[i]->coord[0].y << std::endl;
            std::cout << info.walls[i]->coord[1].x << " " << info.walls[i]->coord[1].y << std::endl;
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
