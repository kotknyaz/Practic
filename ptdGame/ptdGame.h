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



	struct Coord
	{
		double x = 0;
		double y = 0;
		Coord(double newX, double newY);
		Coord();
	};
	struct UpdateInfo
	{
		double view; // temp напрвление центра взгляда
	};
	struct PrintInfo
	{
		//temp
		Coord playerPos;
		std::vector<Coord*> walls;
		std::vector<Coord> visibleWalls;
		double viewLeft;
		double viewRight;

		PrintInfo();
	};



	class MainMenu {
		sf::RenderWindow *window;

	public:
		MainMenu();
		int Menu();
	};


	class GameManager {

		std::vector<Coord*> walls; // temp
		Coord playerPos;
		double view;

		std::vector<Coord> GetVisibleWalls();

	public:
		GameManager(); // пока пусто
		PrintInfo Update(UpdateInfo&);
	};


	class Interpreter {
		sf::RenderWindow* window;
		GameManager* gm;
	public:
		Interpreter(sf::RenderWindow*, GameManager*);
		int GameProcess();
		int Print(PrintInfo&);

	};

}

#endif //PTDGAME_H
