#pragma once


#define _USE_MATH_DEFINES
#include <cmath>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>


#include "GameManager.h"
#include "Interpreter.h"
#include "MainMenu.h"

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
		double viewChange; // temp напрвление центра взгляда
		double playerPosChange; // temp координаты игрока
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