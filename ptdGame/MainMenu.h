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
#include "Interpreter.h"

namespace ptd
{
	class MainMenu // при начале игры создает GameManager (давая ему данные об уровне и т.п.) и 
		//передает управление над окном к Interpreter`у (вместе с ссылкой на GameManager)
	{
		sf::RenderWindow* window;

	public:
		MainMenu();
		int Menu();
		~MainMenu();
	};
}