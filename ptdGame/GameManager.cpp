#include "GameManager.h"

namespace ptd {
    const double FOV = (M_PI / 3);
    const double FOV_DIVIDE_BY2 = FOV / 2;
    const double CAMERA_ROTATING_KOEF = 1.f / 300.f;
    const double PLAYER_SPEED = 0.004;


    std::vector<Wall> GameManager::GetVisibleWalls()
    {
        std::vector<RayLine> VRL = GameManager::GetCrossingRayLines();
        std::vector<Wall> t;
        Wall VW;
        bool isbeginwall = true;
        bool isotherwall = false;
        bool isin = false;
        for (int i = 0; i < walls.size(); i++) {
            for (int j = 0; j < VRL.size(); j++) {
                if (ispointin(Coord(VRL[j].coord[1].x, VRL[j].coord[1].y), Wall(Coord(walls[i]->coord[0].x, walls[i]->coord[0].y), Coord(walls[i]->coord[1].x, walls[i]->coord[1].y)))) {
                    isin = true;
                    if (isbeginwall) {
                        VW.coord[0].x = VRL[j].coord[1].x;
                        VW.coord[0].y = VRL[j].coord[1].y;
                        isbeginwall = false;
                    }
                    else {
                        VW.coord[1].x = VRL[j].coord[1].x;
                        VW.coord[1].y = VRL[j].coord[1].y;
                    }
                }
                else if (!isbeginwall) {
                    VW.coord[1].x = VRL[j - 1].coord[1].x;
                    VW.coord[1].y = VRL[j - 1].coord[1].y;
                    t.push_back(VW);
                    isbeginwall = true;
                    isin = false;
                }
            }
            if (isin) { t.push_back(VW); }
            isbeginwall = true;
            isin = false;
        }
   
        return t;
    }

    std::vector<RayLine> GameManager::GetCrossingRayLines() {
        std::vector<RayLine> VRL;
        RayLine RL;
        double interval = 0.001;
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
        //std::cout << VRL.size() << std::endl;
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

    double GameManager::GetCollisionAngle() {
        std::vector<RayLine> VRL;
        RayLine RL;
        double CollisionAngle = 300;
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
                    CollisionAngle = acos((walls[j]->coord[1].y - walls[j]->coord[0].y) / (sqrt((walls[j]->coord[1].x - walls[j]->coord[0].x) * (walls[j]->coord[1].x - walls[j]->coord[0].x) + (walls[j]->coord[1].y - walls[j]->coord[0].y) * (walls[j]->coord[1].y - walls[j]->coord[0].y))));
                    if ((walls[j]->coord[0].y == walls[j]->coord[1].y) && playerPos.y < walls[j]->coord[0].y) { CollisionAngle *= -1; }
                    return CollisionAngle;
                }
            }
            minlength = DBL_MAX;
            xm = 999;
            ym = 999;
        }

        return DBL_MAX;
    }

    std::vector<VisibleWall> GameManager::GetAngleDistance(std::vector<Wall> t) {
        //t = SortVisibleWall(t);
        double delta = 0.1;
        double temp;
        Coord a;
        RayLine RL;
        RL.coord[1].x = cos(view) + playerPos.x;
        RL.coord[1].y = sin(view) + playerPos.y;
        std::vector<VisibleWall> VW3D;
        VisibleWall VW;
        std::vector<RayLine> VRL = GetCrossingRayLines();
        if (t.size() == 1) { // Если видим сплошную стену
            VW3D.push_back(VisibleWall(-1 * ((RL.coord[1].x - playerPos.x) * (VRL[0].coord[1].x - playerPos.x) + (RL.coord[1].y - playerPos.y) * (VRL[0].coord[1].y - playerPos.y)) / (sqrt((RL.coord[1].x - playerPos.x) * (RL.coord[1].x - playerPos.x) + (RL.coord[1].y - playerPos.y) * (RL.coord[1].y - playerPos.y)) * sqrt((VRL[0].coord[1].x - playerPos.x) * (VRL[0].coord[1].x - playerPos.x) + (VRL[0].coord[1].y - playerPos.y) * (VRL[0].coord[1].y - playerPos.y))), VRL[0].length));
            VW3D.push_back(VisibleWall(((RL.coord[1].x - playerPos.x) * (VRL[VRL.size() - 1].coord[1].x - playerPos.x) + (RL.coord[1].y - playerPos.y) * (VRL[VRL.size() - 1].coord[1].y - playerPos.y)) / (sqrt((RL.coord[1].x - playerPos.x) * (RL.coord[1].x - playerPos.x) + (RL.coord[1].y - playerPos.y) * (RL.coord[1].y - playerPos.y)) * sqrt((VRL[VRL.size() - 1].coord[1].x - playerPos.x) * (VRL[VRL.size() - 1].coord[1].x - playerPos.x) + (VRL[VRL.size() - 1].coord[1].y - playerPos.y) * (VRL[VRL.size() - 1].coord[1].y - playerPos.y))), VRL[VRL.size() - 1].length));
            return VW3D;
        }
        // Если видим углы
        VW3D.push_back(VisibleWall(-1 * ((RL.coord[1].x - playerPos.x) * (VRL[0].coord[1].x - playerPos.x) + (RL.coord[1].y - playerPos.y) * (VRL[0].coord[1].y - playerPos.y)) / (sqrt((RL.coord[1].x - playerPos.x) * (RL.coord[1].x - playerPos.x) + (RL.coord[1].y - playerPos.y) * (RL.coord[1].y - playerPos.y)) * sqrt((VRL[0].coord[1].x - playerPos.x) * (VRL[0].coord[1].x - playerPos.x) + (VRL[0].coord[1].y - playerPos.y) * (VRL[0].coord[1].y - playerPos.y))), VRL[0].length));
        bool isbeginwall = true;
        int k;

        for (int i = 0; i < VRL.size(); i++) {
            for (int j = 0; j < t.size(); j++) {
                if (ispointin(Coord(VRL[i].coord[1].x, VRL[i].coord[1].y), t[j])) {
                    if (!isbeginwall) {
                        if (k == j) {
                            break;
                        }
                        else {
                            k = j;
                        }
                    }
                    else {
                        isbeginwall = false;
                        k = j;
                        break;
                    }
                    // Определяем угол стены, который мы видим
                    if (sqrt((VRL[i].coord[1].x - t[j].coord[0].x) * (VRL[i].coord[1].x - t[j].coord[0].x) + (VRL[i].coord[1].y - t[j].coord[0].y) * (VRL[i].coord[1].y - t[j].coord[0].y)) < sqrt((VRL[i + 1].coord[1].x - t[j].coord[0].x) * (VRL[i + 1].coord[1].x - t[j].coord[0].x) + (VRL[i + 1].coord[1].y - t[j].coord[0].y) * (VRL[i + 1].coord[1].y - t[j].coord[0].y))) {
                        a.x = t[j].coord[0].x;
                        a.y = t[j].coord[0].y;
                    }
                    else if (i == VRL.size() - 1) {
                        if (sqrt((VRL[i - 2].coord[1].x - t[j].coord[0].x) * (VRL[i - 2].coord[1].x - t[j].coord[0].x) + (VRL[i - 2].coord[1].y - t[j].coord[0].y) * (VRL[i - 2].coord[1].y - t[j].coord[0].y)) < sqrt((VRL[i - 1].coord[1].x - t[j].coord[0].x) * (VRL[i - 1].coord[1].x - t[j].coord[0].x) + (VRL[i - 1].coord[1].y - t[j].coord[0].y) * (VRL[i - 1].coord[1].y - t[j].coord[0].y))) {
                            a.x = t[j].coord[0].x;
                            a.y = t[j].coord[0].y;
                        }
                        else {
                            a.x = t[j].coord[1].x;
                            a.y = t[j].coord[1].y;
                        }
                    }
                    else {
                        a.x = t[j].coord[1].x;
                        a.y = t[j].coord[1].y;
                    }
                    //std::cout << a.x << " --- " << a.y << std::endl;
                    VW.distance = sqrt((playerPos.x - a.x) * (playerPos.x - a.x) + (playerPos.y - a.y) * (playerPos.y - a.y));
                    VW.angle = ((RL.coord[1].x - playerPos.x) * (a.x - playerPos.x) + (RL.coord[1].y - playerPos.y) * (a.y - playerPos.y)) / (sqrt((RL.coord[1].x - playerPos.x) * (RL.coord[1].x - playerPos.x) + (RL.coord[1].y - playerPos.y) * (RL.coord[1].y - playerPos.y)) * sqrt((a.x - playerPos.x) * (a.x - playerPos.x) + (a.y - playerPos.y) * (a.y - playerPos.y)));
                    if (VW.angle > 1) { VW.angle = 1; }
                    else if (VW.angle < -1) { VW.angle = -1; }
                    VW.angle = acos(VW.angle);
                    if (i < VRL.size() / 2) { VW.angle *= -1; }
                    VW3D.push_back(VW);
                    if (std::fabs(VRL[i].length - VRL[i - 1].length) > delta) {
                        VW3D.push_back(VisibleWall(VW3D[VW3D.size() - 1].angle, VRL[i - 1].length));
                        temp = VW3D[VW3D.size() - 1].distance;
                        VW3D[VW3D.size() - 1].distance = VW3D[VW3D.size() - 2].distance;
                        VW3D[VW3D.size() - 2].distance = temp;
                    }
                    break;
                }
            }
        }
        VW3D.push_back(VisibleWall(((RL.coord[1].x - playerPos.x) * (VRL[VRL.size() - 1].coord[1].x - playerPos.x) + (RL.coord[1].y - playerPos.y) * (VRL[VRL.size() - 1].coord[1].y - playerPos.y)) / (sqrt((RL.coord[1].x - playerPos.x) * (RL.coord[1].x - playerPos.x) + (RL.coord[1].y - playerPos.y) * (RL.coord[1].y - playerPos.y)) * sqrt((VRL[VRL.size() - 1].coord[1].x - playerPos.x) * (VRL[VRL.size() - 1].coord[1].x - playerPos.x) + (VRL[VRL.size() - 1].coord[1].y - playerPos.y) * (VRL[VRL.size() - 1].coord[1].y - playerPos.y))), VRL[VRL.size() - 1].length));
       // std::cout << " --- " << std::endl;
        return VW3D;
    }

    GameManager::GameManager()
    {
        walls.push_back(new Wall(Coord(3, 2), Coord(3, -2)));
        walls.push_back(new Wall(Coord(3, -2), Coord(1, -2)));
        walls.push_back(new Wall(Coord(1, -2), Coord(1, -5)));
        walls.push_back(new Wall(Coord(1, -5), Coord(-2, -5)));
        walls.push_back(new Wall(Coord(-2, -5), Coord(-2, -2)));
        walls.push_back(new Wall(Coord(-2, -2), Coord(-5, -2)));
        walls.push_back(new Wall(Coord(-5, -2), Coord(-5, 5)));
        walls.push_back(new Wall(Coord(-5, 5), Coord(6, 5)));
        walls.push_back(new Wall(Coord(6, 5), Coord(6, 2)));
        walls.push_back(new Wall(Coord(6, 2), Coord(3, 2)));
        walls.push_back(new Wall(Coord(-2, 3), Coord(-3, 1)));
        walls.push_back(new Wall(Coord(-1, 1), Coord(-2, 3)));
        walls.push_back(new Wall(Coord(-3, 1), Coord(-1, 1)));
    }

    PrintInfo2D GameManager::Update2D(UpdateInfo& updateInfo)
    {
        PrintInfo2D printInfo;
        double CollisionAngle;
        view += updateInfo.viewChange * CAMERA_ROTATING_KOEF;
        


        printInfo.VRL = GetCrossingRayLines(); // получаем вектор лучей: VRL[0].coord[0].x - точка начала по иксу (т.е. координата положения игрока по иксу), и тоже самое с y, 
                                               // VRL[0].coord[1].x - точка конца по иксу (т.е. координата точки пересечения со стеной по иксу), и тоже самое с y,
                                               // VRL[0].lenght - длина луча (расстояние до стены)
                                               // и т.д. (VRL[1]..., VRL[2]...)

        printInfo.visibleWalls = GetVisibleWalls(); // получаем вектор видимых стен: visibleWalls[0].coord[0].x - точка начала по иксу, и тоже самое с y,
                                                    // visibleWalls[0].coord[1].x - точка конца по иксу, и тоже самое по y
                                                    // и т.д. (visibleWalls[1]..., visibleWalls[2]...)
        CollisionAngle = GetCollisionAngle();
        //std::cout << RL.length << std::endl;
 
        // Изменение положения игрока относительно направления взгляда
        if (CollisionAngle == DBL_MAX) {
            playerPos.x += updateInfo.playerPosChange * cos(view) * PLAYER_SPEED;
            playerPos.y += updateInfo.playerPosChange * sin(view) * PLAYER_SPEED;
        }
        else {
            playerPos.x += fabs(updateInfo.playerPosChange) * cos(CollisionAngle) * PLAYER_SPEED;
            playerPos.y += fabs(updateInfo.playerPosChange) * sin(CollisionAngle) * PLAYER_SPEED;
        }

        printInfo.walls = walls;
        printInfo.playerPos.x += playerPos.x;
        printInfo.playerPos.y += playerPos.y;
        printInfo.viewLeft = view + FOV_DIVIDE_BY2;
        printInfo.viewRight = view - FOV_DIVIDE_BY2;

        return printInfo;
    }


    PrintInfo3D GameManager::Update3D(UpdateInfo& updateInfo)
    {
        PrintInfo3D printInfo;
        double CollisionAngle;
        view += updateInfo.viewChange* CAMERA_ROTATING_KOEF;

        CollisionAngle = GetCollisionAngle();

        if (CollisionAngle == DBL_MAX) {
            playerPos.x += updateInfo.playerPosChange * cos(view) * PLAYER_SPEED;
            playerPos.y += updateInfo.playerPosChange * sin(view) * PLAYER_SPEED;
        }
        else {
            playerPos.x += fabs(updateInfo.playerPosChange) * cos(CollisionAngle) * PLAYER_SPEED;
            playerPos.y += fabs(updateInfo.playerPosChange) * sin(CollisionAngle) * PLAYER_SPEED;
        }
      
        printInfo.walls = GetAngleDistance(GetVisibleWalls());

        return printInfo;
    }

    bool GameManager::ispointin(Coord a, Wall VW) { // лежит ли точка на прямой
        if ((a.x - VW.coord[0].x) * (VW.coord[1].y - VW.coord[0].y) == (VW.coord[1].x - VW.coord[0].x) * (a.y - VW.coord[0].y)) {
            if (std::min(VW.coord[0].y, VW.coord[1].y) <= a.y && (std::max(VW.coord[0].y, VW.coord[1].y) >= a.y) && std::min(VW.coord[0].x, VW.coord[1].x) <= a.x && std::max(VW.coord[0].x, VW.coord[1].x) >= a.x) {
                return true;
            }
        }
        return false;
    }

    std::vector<Wall> GameManager::SortVisibleWall(std::vector<Wall> t) {
        double tempx, tempy;
        for (int i = 0; i < t.size(); i++) {
            if ((t[i].coord[0].x > t[i].coord[1].x) && (t[i].coord[0].y > t[i].coord[1].y)) {
                tempx = t[i].coord[0].x;
                tempy = t[i].coord[0].y;
                t[i].coord[0].x = t[i].coord[1].x;
                t[i].coord[0].y = t[i].coord[1].y;
                t[i].coord[1].x = tempx;
                t[i].coord[1].y = tempy;
            }
            else if ((t[i].coord[0].x > t[i].coord[1].x) && (t[i].coord[0].y < t[i].coord[1].y)) {
                tempx = t[i].coord[0].x;
                tempy = t[i].coord[0].y;
                t[i].coord[0].x = t[i].coord[1].x;
                t[i].coord[0].y = t[i].coord[1].y;
                t[i].coord[1].x = tempx;
                t[i].coord[1].y = tempy;
            }
            else if ((t[i].coord[0].x == t[i].coord[1].x) && (t[i].coord[0].y > t[i].coord[1].y)) {
                tempx = t[i].coord[0].x;
                tempy = t[i].coord[0].y;
                t[i].coord[0].x = t[i].coord[1].x;
                t[i].coord[0].y = t[i].coord[1].y;
                t[i].coord[1].x = tempx;
                t[i].coord[1].y = tempy;
            }
            else if ((t[i].coord[0].x > t[i].coord[1].x) && (t[i].coord[0].y == t[i].coord[1].y)) {
                tempx = t[i].coord[0].x;
                tempy = t[i].coord[0].y;
                t[i].coord[0].x = t[i].coord[1].x;
                t[i].coord[0].y = t[i].coord[1].y;
                t[i].coord[1].x = tempx;
                t[i].coord[1].y = tempy;
            }
        }
        return t;
    }

    GameManager::~GameManager() {
        for (int i = 0; i < walls.size(); i++) {
            delete walls[i];
        }
    }
}