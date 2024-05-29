#ifndef PTDGAME_H
#define PTDGAME_H

#define _USE_MATH_DEFINES
#include <cmath>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

namespace ptd {
	/*
	* Будет два режима запуска для отладки - 2D с видом сверху; ну и псевдо-3D
	* все классы где есть приписка 2D - классы для отладки, потом будут созданы аналоги
	*/


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
	public:
		Wall(Coord, Coord);
		Wall();
	};
	struct RayLine // луч
	{
		Coord coord[2];
		double length;
	public:
		RayLine(Coord, Coord);
		RayLine();
	};
	struct CrossingRayLineInfo 
	{
		bool isCrossing;
		double distance;
	public:
		CrossingRayLineInfo();
	};
	struct VisibleWall
	{
		Coord coord[2];
	public:
		VisibleWall(Coord, Coord);
		VisibleWall();
	};
	struct VisibleWall3D
	{
		double angle;
		double distance;
	public:
		VisibleWall3D(double, double);
		VisibleWall3D();
	};
	struct PrintInfo2D // передается от GameManager к Interpreter (что теперь нужно рисовать; в ответ на UpdateInfo)
	{
		Coord playerPos;
		std::vector<Wall*> walls;
		std::vector<RayLine> VRL;
		std::vector<VisibleWall> visibleWalls;
		double viewLeft;
		double viewRight;

		PrintInfo2D();
	};
	struct PrintInfo3D
	{
		std::vector<VisibleWall3D> walls;
	};

	// ----------------------------------------- Основные классы игры -----------------------------------------
	class MainMenu // при начале игры создает GameManager (давая ему данные об уровне и т.п.) и 
		//передает управление над окном к Interpreter`у (вместе с ссылкой на GameManager)
	{
		sf::RenderWindow* window;

	public:
		MainMenu();
		int Menu();
	};


	class GameManager // 2D движок в котором выполняется игра (ходьба, стрельба, подгрузка комнат и т.п.)
	{
		sf::Clock* clock;

		std::vector<Wall*> walls; // temp
		Coord playerPos;
		double view;

		bool ispointin(Coord, VisibleWall);
		std::vector<VisibleWall> SortVisibleWall(std::vector<VisibleWall>);
		std::vector<VisibleWall> GetVisibleWalls(); // определяет видимые стены
		std::vector<RayLine> GetCrossingRayLines(); // определяет длину каждого луча в зависимости от расстояния до стены
		std::vector<VisibleWall3D> GetAngleDistance(std::vector<VisibleWall>);
		//RayLine GetCollisionRayLines();
		CrossingRayLineInfo CrossingRayLine(const Wall&);

	public:
		GameManager(); // пока пусто
		~GameManager();
		PrintInfo2D Update2D(UpdateInfo&);
		PrintInfo3D Update3D(UpdateInfo&);
	};


	class Interpreter // каждый кадр передает действия игрока в GameManager и отрисовывает полученные от него данные
	{
		sf::RenderWindow* window;
		GameManager* gm;
	public:
		Interpreter(sf::RenderWindow*, GameManager*);
		int GameProcess();
		int Print2D(PrintInfo2D&);
		int Print3D(PrintInfo3D&);
	};

}

#endif //PTDGAME_H
