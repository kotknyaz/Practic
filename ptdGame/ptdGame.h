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

	// ----------------------------------------- Основные классы игры -----------------------------------------
	class MainMenu // при начале игры создает GameManager (давая ему данные об уровне и т.п.) и 
		//передает управление над окном к Interpreter`у (вместе с ссылкой на GameManager)
	{
		sf::RenderWindow* window;

	public:
		MainMenu();
		int Menu();
		~MainMenu();
	};


	class GameManager // 2D движок в котором выполняется игра (ходьба, стрельба, подгрузка комнат и т.п.)
	{
		sf::Clock* clock;

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


	class Interpreter // каждый кадр передает действия игрока в GameManager и отрисовывает полученные от него данные
	{
		sf::RenderWindow* window;
		GameManager* gm;
	public:
		Interpreter(sf::RenderWindow*, GameManager*);
		int GameProcess();
		int Print(PrintInfo2D&);
		int Print(PrintInfo3D&);
	};



	// ----------------------------------------- Еще какието классы -----------------------------------------

	class Drawble
	{
	protected:
		double size;
		Coord coord;
		sf::Shape* shape;

	public:
		Coord GetCoord();
		double GetSize();
		int Draw(double x, double width, sf::RenderWindow* window);
		Drawble(double width, Coord c, sf::Shape* usingShape);
		~Drawble();
	};

	class Object : public Drawble
	{

	};
}

#endif //PTDGAME_H
