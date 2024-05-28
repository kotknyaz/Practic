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
        std::vector<RayLine> VRL = GameManager::GetCrossingRayLines();
        std::vector<VisibleWall> t;
        VisibleWall VW;

        bool isbeginwall = true;
        bool isotherwall = false;
        bool isin = false;
        for (int i = 0; i < walls.size(); i++) {
            for (int j = 0; j < VRL.size(); j++) {
                if ((VRL[j].coord[1].x <= std::max(walls[i]->coord[0].x, walls[i]->coord[1].x)) && (VRL[j].coord[1].x >= std::min(walls[i]->coord[0].x, walls[i]->coord[1].x)) && (VRL[j].coord[1].y <= std::max(walls[i]->coord[0].y, walls[i]->coord[1].y)) && (VRL[j].coord[1].y >= std::min(walls[i]->coord[0].y, walls[i]->coord[1].y))) {
                    isin = true;
                    if (isbeginwall) {
                        if (j == 0) { //первый луч
                            VW.coord[0].x = VRL[j].coord[1].x;
                            VW.coord[0].y = VRL[j].coord[1].y;
                        }
                        /*else if (j == VRL.size() - 1) { // последний луч
                            VW.coord[1].x = VRL[j].coord[1].x;
                            VW.coord[1].y = VRL[j].coord[1].y;
                            if ((VRL[j].coord[0].y < VRL[j].coord[1].y) && (VRL[j].coord[0].x < VRL[j].coord[1].x)) { //Северо-восточная часть
                                VW.coord[0].x = walls[i]->coord[1].x;
                                VW.coord[0].y = walls[i]->coord[1].y;
                            }
                            else if ((VRL[j].coord[0].y > VRL[j].coord[1].y) && (VRL[j].coord[0].x < VRL[j].coord[1].x)) { //Юго-восточная часть
                                VW.coord[0].x = walls[i]->coord[1].x;
                                VW.coord[0].y = walls[i]->coord[1].y;
                            }
                            else if ((VRL[j].coord[0].y > VRL[j].coord[1].y) && (VRL[j].coord[0].x > VRL[j].coord[1].x)) { //Юго-западная часть
                                VW.coord[0].x = walls[i]->coord[0].x;
                                VW.coord[0].y = walls[i]->coord[0].y;
                            }
                            else if ((VRL[j].coord[0].y < VRL[j].coord[1].y) && (VRL[j].coord[0].x > VRL[j].coord[1].x)) { //Северо-западная часть
                                VW.coord[0].x = walls[i]->coord[0].x;
                                VW.coord[0].y = walls[i]->coord[0].y;
                            }
                            t.push_back(VW);
                        }*/
                        else { //не первый луч (пока работает)
                            VW.coord[1].x = VRL[j].coord[1].x;
                            VW.coord[1].y = VRL[j].coord[1].y;
                            if ((VRL[j].coord[0].y < VRL[j].coord[1].y) && (VRL[j].coord[0].x < VRL[j].coord[1].x)) { //Северо-восточная часть
                                VW.coord[0].x = walls[i]->coord[1].x;
                                VW.coord[0].y = walls[i]->coord[1].y;
                            }
                            else if ((VRL[j].coord[0].y > VRL[j].coord[1].y) && (VRL[j].coord[0].x < VRL[j].coord[1].x)) { //Юго-восточная часть
                                VW.coord[0].x = walls[i]->coord[1].x;
                                VW.coord[0].y = walls[i]->coord[1].y;
                            }
                            else if ((VRL[j].coord[0].y > VRL[j].coord[1].y) && (VRL[j].coord[0].x > VRL[j].coord[1].x)) { //Юго-западная часть
                                VW.coord[0].x = walls[i]->coord[0].x;
                                VW.coord[0].y = walls[i]->coord[0].y;
                            }
                            else if ((VRL[j].coord[0].y < VRL[j].coord[1].y) && (VRL[j].coord[0].x > VRL[j].coord[1].x)) { //Северо-западная часть
                                VW.coord[0].x = walls[i]->coord[0].x;
                                VW.coord[0].y = walls[i]->coord[0].y;
                            }
                        }
                        
                        isbeginwall = false;
                    }
                    else {
                        VW.coord[1].x = VRL[j].coord[1].x;
                        VW.coord[1].y = VRL[j].coord[1].y;
                    }
                }
                else if(!isbeginwall) {
                    if ((VRL[j].coord[0].y < VRL[j].coord[1].y) && (VRL[j].coord[0].x < VRL[j].coord[1].x)) { //Северо-восточная часть
                        VW.coord[1].x = walls[i]->coord[1].x;
                        VW.coord[1].y = walls[i]->coord[1].y;
                    }
                    else if ((VRL[j].coord[0].y > VRL[j].coord[1].y) && (VRL[j].coord[0].x < VRL[j].coord[1].x)) { //Юго-восточная часть
                        VW.coord[1].x = walls[i]->coord[0].x;
                        VW.coord[1].y = walls[i]->coord[0].y;
                    }
                    else if ((VRL[j].coord[0].y > VRL[j].coord[1].y) && (VRL[j].coord[0].x > VRL[j].coord[1].x)) { //Юго-западная часть
                        VW.coord[1].x = walls[i]->coord[0].x;
                        VW.coord[1].y = walls[i]->coord[0].y;
                    }
                    else if ((VRL[j].coord[0].y < VRL[j].coord[1].y) && (VRL[j].coord[0].x > VRL[j].coord[1].x)) { //Северо-западная часть
                        VW.coord[1].x = walls[i]->coord[1].x;
                        VW.coord[1].y = walls[i]->coord[1].y;
                    }
                    break;
                }
            }
            if (isin) { t.push_back(VW); }
            isbeginwall = true;
            isin = false;
        }
        //std::cout << t.size() << std::endl;
        return t;
    }

    std::vector<RayLine> GameManager::GetCrossingRayLines() {
        std::vector<RayLine> VRL;
        RayLine RL;
        double interval = 0.01;
        double leftview = view + FOV_DIVIDE_BY2;
        double rightview = view - FOV_DIVIDE_BY2;
        double currentview = leftview;
        while (currentview >= rightview) {
            RL.coord[0].x = playerPos.x;
            RL.coord[0].y = playerPos.y;
            RL.coord[1].x = cos(currentview) + playerPos.x;
            RL.coord[1].y = sin(currentview) + playerPos.y;
            VRL.push_back(RL);
            currentview -= interval;
        }

        double k1, k2, b1, b2, xp, yp, minlength = DBL_MAX, xm = 999, ym = 999;
        for (int i = 0; i < VRL.size(); i++) {
            if (VRL[i].coord[0].x == VRL[i].coord[1].x) {
                k1 = DBL_MAX;
                b1 = 0;
            }
            else {
                k1 = (VRL[i].coord[0].y - VRL[i].coord[1].y) / (VRL[i].coord[0].x - VRL[i].coord[1].x);
                b1 = VRL[i].coord[0].y - (k1 * VRL[i].coord[0].x);
            }
            for (int j = 0; j < walls.size(); j++) {
                if (walls[j]->coord[0].x == walls[j]->coord[1].x) {
                    xp = walls[j]->coord[0].x;
                    yp = (k1 * xp) + b1;
                }
                else {
                    k2 = (walls[j]->coord[0].y - walls[j]->coord[1].y) / (walls[j]->coord[0].x - walls[j]->coord[1].x);
                    b2 = walls[j]->coord[0].y - (k2 * walls[j]->coord[0].x);
                    if (k1 == k2) { continue; }
                    xp = (b2 - b1) / (k1 - k2);
                    yp = (k2 * xp) + b2;
                }
                if ((xp > std::max(walls[j]->coord[0].x, walls[j]->coord[1].x)) || (xp < std::min(walls[j]->coord[0].x, walls[j]->coord[1].x))) { continue; }
                if ((yp > std::max(walls[j]->coord[0].y, walls[j]->coord[1].y)) || (yp < std::min(walls[j]->coord[0].y, walls[j]->coord[1].y))) { continue; }
                if ((VRL[i].coord[0].x < VRL[i].coord[1].x) && (xp < VRL[i].coord[0].x)) { continue; }
                if ((VRL[i].coord[0].x > VRL[i].coord[1].x) && (xp > VRL[i].coord[0].x)) { continue; }

                VRL[i].coord[1].x = xp;
                VRL[i].coord[1].y = yp;

                VRL[i].length = sqrt(((xp - VRL[i].coord[0].x) * (xp - VRL[i].coord[0].x)) + ((yp - VRL[i].coord[0].y) * (yp - VRL[i].coord[0].y)));
                    
                if (VRL[i].length > minlength) {
                    VRL[i].length = minlength;
                    VRL[i].coord[1].x = xm;
                    VRL[i].coord[1].y = ym;
                }
                else {
                    minlength = VRL[i].length;
                    xm = xp;
                    ym = yp;
                }
            }
            minlength = DBL_MAX;
            xm = 999;
            ym = 999;
        }
        std::cout << VRL.size() << std::endl;
        return VRL;
    }

    CrossingRayLineInfo GameManager::CrossingRayLine(const Wall& wall)
    {
        CrossingRayLineInfo a;
        RayLine RL;
        RL.coord[0].x = playerPos.x;
        RL.coord[0].y = playerPos.y;
        RL.coord[1].x = cos(view) + playerPos.x;
        RL.coord[1].y = sin(view) + playerPos.y;

        double k1, k2, b1, b2, xp, yp;
        if (RL.coord[0].x == RL.coord[1].x) {
            k1 = DBL_MAX;
            b1 = 0;
        }
        else {
            k1 = (RL.coord[0].y - RL.coord[1].y) / (RL.coord[0].x - RL.coord[1].x);
            b1 = RL.coord[0].y - (k1 * RL.coord[0].x);
        }
        if (wall.coord[0].x == wall.coord[1].x) {
            xp = wall.coord[0].x;
            yp = (k1 * xp) + b1;
        }
        else {
            k2 = (wall.coord[0].y - wall.coord[1].y) / (wall.coord[0].x - wall.coord[1].x);
            b2 = wall.coord[0].y - (k2 * wall.coord[0].x);
            if (k1 == k2) { return a; }
            xp = (b2 - b1) / (k1 - k2);
            yp = (k2 * xp) + b2;
        }
        if ((xp > std::max(wall.coord[0].x, wall.coord[1].x)) || (xp < std::min(wall.coord[0].x, wall.coord[1].x))) {return a;}
        if ((yp > std::max(wall.coord[0].y, wall.coord[1].y)) || (yp < std::min(wall.coord[0].y, wall.coord[1].y))) { return a; }
        if ((RL.coord[0].x < RL.coord[1].x) && (xp < RL.coord[0].x)) { return a; }
        if ((RL.coord[0].x > RL.coord[1].x) && (xp > RL.coord[0].x)) { return a; }

        a.distance = sqrt(((xp - RL.coord[0].x) * (xp - RL.coord[0].x)) + ((yp - RL.coord[0].y) * (yp - RL.coord[0].y)));
        a.isCrossing = true;
        return a;
    }

    /*RayLine GameManager::GetCollisionRayLines() {
        std::vector<RayLine> VRL;
        RayLine RL;
        double delta = 0.1;
        double interval = 0.1;
        double currentview = 0;;
        double c, s;
        while (currentview <= 2 * M_PI) {
            RL.coord[0].x = cos(currentview);
            RL.coord[0].y = sin(currentview);
            RL.coord[1].x = cos(currentview) + playerPos.x;
            RL.coord[1].y = sin(currentview) + playerPos.y;
            VRL.push_back(RL);
            currentview += interval;
        }

        double k1, k2, b1, b2, xp, yp, minlength = DBL_MAX, xm = 999, ym = 999;
        for (int i = 0; i < VRL.size(); i++) {
            if (playerPos.x == VRL[i].coord[1].x) {
                k1 = DBL_MAX;
                b1 = 0;
            }
            else {
                k1 = (playerPos.y - VRL[i].coord[1].y) / (playerPos.x - VRL[i].coord[1].x);
                b1 = playerPos.y - (k1 * playerPos.x);
            }
            for (int j = 0; j < walls.size(); j++) {

                if (walls[j]->coord[0].x == walls[j]->coord[1].x) {
                    xp = walls[j]->coord[0].x;
                    yp = (k1 * xp) + b1;
                }
                else {
                    k2 = (walls[j]->coord[0].y - walls[j]->coord[1].y) / (walls[j]->coord[0].x - walls[j]->coord[1].x);
                    b2 = walls[j]->coord[0].y - (k2 * walls[j]->coord[0].x);
                    if (k1 == k2) { continue; }
                    xp = (b2 - b1) / (k1 - k2);
                    yp = (k2 * xp) + b2;
                }
                if ((xp > std::max(walls[j]->coord[0].x, walls[j]->coord[1].x)) || (xp < std::min(walls[j]->coord[0].x, walls[j]->coord[1].x))) { continue; }
                if ((yp > std::max(walls[j]->coord[0].y, walls[j]->coord[1].y)) || (yp < std::min(walls[j]->coord[0].y, walls[j]->coord[1].y))) { continue; }
                if ((playerPos.x < VRL[i].coord[1].x) && (xp < playerPos.x)) { continue; }
                if ((playerPos.x > VRL[i].coord[1].x) && (xp > playerPos.x)) { continue; }

                VRL[i].length = sqrt(((xp - playerPos.x) * (xp - playerPos.x)) + ((yp - playerPos.y) * (yp - playerPos.y)));

                if (VRL[i].length > minlength) {
                    VRL[i].length = minlength;
                    VRL[i].coord[1].x = xm;
                    VRL[i].coord[1].y = ym;
                }
                else {
                    minlength = VRL[i].length;
                    xm = xp;
                    ym = yp;
                }
                if (VRL[i].length <= delta) {
                    RL.coord[1].x = VRL[i].coord[0].x;
                    RL.coord[1].y = VRL[i].coord[0].y;
                    RL.length = VRL[i].length;
                    return RL;
                }
            }
            minlength = DBL_MAX;
            xm = 999;
            ym = 999;
        }

        return RL;
    }*/

    GameManager::GameManager()
    {
        clock = new sf::Clock();

        walls.push_back(new Wall(Coord(-2, 4), Coord(3, 4)));
        walls.push_back(new Wall(Coord(3, -4), Coord(3, 4)));
        walls.push_back(new Wall(Coord(-2, -4), Coord(3, -4)));
        walls.push_back(new Wall(Coord(-2, -4), Coord(-2, 4)));
        walls.push_back(new Wall(Coord(-0.2, 0.4), Coord(0.3, 0.4)));
        walls.push_back(new Wall(Coord(0.3, -0.4), Coord(0.3, 0.4)));
        walls.push_back(new Wall(Coord(-0.2, -0.4), Coord(0.3, -0.4)));
        walls.push_back(new Wall(Coord(-0.2, -0.4), Coord(-0.2, 0.4)));
    }

    PrintInfo2D GameManager::Update2D(UpdateInfo& updateInfo)
    {
        PrintInfo2D printInfo;
        RayLine RL;
        view += updateInfo.viewChange * clock->getElapsedTime().asMilliseconds() * CAMERA_ROTATING_KOEF;
        //std::cout << std::endl << 1.f / clock->getElapsedTime().asSeconds() << std::endl;
        clock->restart();
        


        printInfo.VRL = GetCrossingRayLines(); // получаем вектор лучей: VRL[0].coord[0].x - точка начала по иксу (т.е. координата положения игрока по иксу), и тоже самое с y, 
                                               // VRL[0].coord[1].x - точка конца по иксу (т.е. координата точки пересечения со стеной по иксу), и тоже самое с y,
                                               // VRL[0].lenght - длина луча (расстояние до стены)
                                               // и т.д. (VRL[1]..., VRL[2]...)

        printInfo.visibleWalls = GetVisibleWalls(); // получаем вектор видимых стен: visibleWalls[0].coord[0].x - точка начала по иксу, и тоже самое с y,
                                                    // visibleWalls[0].coord[1].x - точка конца по иксу, и тоже самое по y
                                                    // и т.д. (visibleWalls[1]..., visibleWalls[2]...)
        //RL = GetCollisionRayLines();
 
        //----------------------------------------- Изменение положения игрока относительно направления взгляда -----------------------------------------
        playerPos.x += updateInfo.playerPosChange * cos(view);
        playerPos.y += updateInfo.playerPosChange * sin(view);
        //---------------------------------------------------------
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
                toSent.viewChange += 0.5;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)))
            {
                toSent.viewChange -= 0.5;
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
        double ray = info.viewLeft;
        double interval = 0.01;
        sf::VertexArray vertexWalls(sf::PrimitiveType::Lines, 0);
        double halfScreenX = window->getSize().x / 2;
        double halfScreenY = window->getSize().y / 2;

        for (int i = 0; i < info.walls.size(); ++i)
        {
            //std::cout << i << "\n";
            //std::cout << info.walls[i]->coord[0].x << " " << info.walls[i]->coord[0].y << std::endl;
            //std::cout << info.walls[i]->coord[1].x << " " << info.walls[i]->coord[1].y << std::endl;
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

    RayLine::RayLine() {}

    RayLine::RayLine(Coord a, Coord b)
    {
        coord[0].x = a.x;
        coord[0].y = a.y;
        coord[1].x = b.x;
        coord[1].y = b.y;
    }

    VisibleWall::VisibleWall(Coord a, Coord b) {
        coord[0].x = a.x;
        coord[0].y = a.y;
        coord[1].x = b.x;
        coord[1].y = b.y;
    }

    VisibleWall::VisibleWall() {}

    CrossingRayLineInfo::CrossingRayLineInfo() {
        this->distance = 0;
        this->isCrossing = false;
    }
}

