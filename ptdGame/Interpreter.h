#pragma once

#define _USE_MATH_DEFINES
#include <cmath>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>


#include "DataTypes.h"
#include "GameManager.h"
#include "MainMenu.h"

namespace ptd
{

	class Interpreter // ������ ���� �������� �������� ������ � GameManager � ������������ ���������� �� ���� ������
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