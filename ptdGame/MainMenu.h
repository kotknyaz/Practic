#ifndef MAINMENU_H
#define MAINMENU_H

#define _USE_MATH_DEFINES
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

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

#endif //MAINMENU_H