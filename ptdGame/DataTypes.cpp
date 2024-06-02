#include "DataTypes.h"

namespace ptd {
    Coord::Coord(double newX, double newY) : x(newX), y(newY) {}

    Coord::Coord() {}


    PrintInfo2D::PrintInfo2D()
    {
        playerPos = Coord(0.f, 0.f);
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



    VisibleWall::VisibleWall() {}

    VisibleWall::VisibleWall(double a, double b) {
        angle = a;
        distance = b;
    }

    CrossingRayLineInfo::CrossingRayLineInfo() {
        this->distance = 0;
        this->isCrossing = false;
    }
}