#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#define _USE_MATH_DEFINES
#include <cmath>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

#include "DataTypes.h"

namespace ptd {

	class GameManager // 2D движок в котором выполняется игра (ходьба, стрельба, подгрузка комнат и т.п.)
	{
		std::vector<Wall*> walls;
		Coord playerPos;
		double view;

		bool ispointin(Coord, Wall);
		std::vector<Wall> SortVisibleWall(std::vector<Wall>);
		std::vector<Wall> GetVisibleWalls(); // определяет видимые стены
		std::vector<RayLine> GetCrossingRayLines(); // определяет длину каждого луча в зависимости от расстояния до стены
		std::vector<VisibleWall> GetAngleDistance(std::vector<Wall>);
		double GetCollisionAngle();
		CrossingRayLineInfo CrossingRayLine(const Wall&);

	public:
		GameManager(); // пока пусто
		~GameManager();
		PrintInfo2D Update2D(UpdateInfo&);
		PrintInfo3D Update3D(UpdateInfo&);
	};


}

#endif //GAMEMANAGER_H
