#ifndef DATATYPES_H
#define DATATYPES_H

#include <vector>

namespace ptd {

	// ----------------------------------------- Служебные типы данных -----------------------------------------
	struct Coord
	{
		double x = 0;
		double y = 0;
		Coord(double newX, double newY);
		Coord();
	};
	struct UpdateInfo //передается от Interpreter к GameManager (действия игрока между кадрами)
	{
		double viewChange;
		double playerPosChange;
	};
	struct Wall // стена в 2D
	{
		Coord coord[2];
		Wall(Coord, Coord);
		Wall();
	};
	struct RayLine // луч
	{
		Coord coord[2];
		double length;
		RayLine(Coord, Coord);
		RayLine();
	};
	struct CrossingRayLineInfo
	{
		bool isCrossing;
		double distance;
		CrossingRayLineInfo();
	};
	struct VisibleWall
	{
		double angle;
		double distance;
		VisibleWall(double, double);
		VisibleWall();
	};
	struct PrintInfo2D
	{
		Coord playerPos;
		std::vector<Wall*> walls;
		std::vector<RayLine> VRL;
		std::vector<Wall> visibleWalls;
		double viewLeft;
		double viewRight;

		PrintInfo2D();
	};
	struct PrintInfo3D
	{
		std::vector<VisibleWall> walls;
	};

}

#endif //DATATYPES_H