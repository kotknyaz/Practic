#ifndef INTERPRETER_H
#define INTERPRETER_H

#define _USE_MATH_DEFINES
#include <cmath>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

#include "GameManager.h"

namespace ptd
{


	class Interpreter // каждый кадр передает действия игрока в GameManager и отрисовывает полученные от него данные
	{
		sf::Clock* clock;

		sf::RenderWindow* window;
		GameManager* gm;
	public:
		Interpreter(sf::RenderWindow*, GameManager*);
		~Interpreter();
		int GameProcess();
		int Print(PrintInfo2D&);
		int Print(PrintInfo3D&);
	};

}

#endif //INTERPRETER_H