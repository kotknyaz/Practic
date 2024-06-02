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
	class MainMenu // ��� ������ ���� ������� GameManager (����� ��� ������ �� ������ � �.�.) � 
		//�������� ���������� ��� ����� � Interpreter`� (������ � ������� �� GameManager)
	{
		sf::RenderWindow* window;

	public:
		MainMenu();
		int Menu();
		~MainMenu();
	};
}