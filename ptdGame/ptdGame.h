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
	};
	struct Wall // стена в 2D
	{
		Coord coord[2];
	public:
		Wall(Coord, Coord);
		Wall();
	};
	struct CrossingRayLineInfo 
	{
		bool isCrossing;
		double angle;
		double distance;
	};
	struct VisibleWall // класс для отрисовки стен  псевдо-3D; углы считаются слева (экрана), от 0 до FOV
	{
		double leftBound_angle;
		double leftBound_distance;
		double rightBound_angle;
		double rightBound_distance;
		Wall* wall;
	public:
		VisibleWall();
	};
	struct PrintInfo2D // передается от GameManager к Interpreter (что теперь нужно рисовать; в ответ на UpdateInfo)
	{
		//temp
		Coord playerPos;
		std::vector<Wall*> walls;
		std::vector<VisibleWall> visibleWalls;
		double viewLeft;
		double viewRight;

		PrintInfo2D();
	};


	// ----------------------------------------- Основные классы игры -----------------------------------------
	class MainMenu // при начале игры создает GameManager (давая ему данные об уровне и т.п.) и 
		//передает управление над окном к Interpreter`у (вместе с ссылкой на GameManager)
	{
		sf::RenderWindow *window;

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

		std::vector<VisibleWall> GetVisibleWalls();
		CrossingRayLineInfo CrossingRayLine(const Wall&, double angle);

	public:
		GameManager(); // пока пусто
		PrintInfo2D Update2D(UpdateInfo&);
	};


	class Interpreter // каждый кадр передает действия игрока в GameManager и отрисовывает полученные от него данные
	{
		sf::RenderWindow* window;
		GameManager* gm;
	public:
		Interpreter(sf::RenderWindow*, GameManager*);
		int GameProcess();
		int Print2D(PrintInfo2D&);

	};

}

#endif //PTDGAME_H
