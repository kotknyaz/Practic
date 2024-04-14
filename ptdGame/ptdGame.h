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
	* ����� ��� ������ ������� ��� ������� - 2D � ����� ������; �� � ������-3D
	* ��� ������ ��� ���� �������� 2D - ������ ��� �������, ����� ����� ������� �������
	*/


	// ----------------------------------------- ��������� ���� ������ -----------------------------------------
	struct Coord
	{
		double x = 0;
		double y = 0;
		Coord(double newX, double newY);
		Coord();
	};
	struct UpdateInfo //���������� �� Interpreter � GameManager (�������� ������ ����� �������)
	{
		double view; // temp ���������� ������ �������
	};
	struct Wall // ����� � 2D
	{
		Coord coord[2];
	public:
		Wall(Coord, Coord);
		Wall();
	};
	struct VisibleWall // ����� ��� ��������� ����  ������-3D; ���� ��������� ����� (������), �� 0 �� FOV
	{
		double leftBound_angle;
		double leftBound_distance;
		double rightBound_angle;
		double rightBound_distance;
		Wall* wall;
	public:
		VisibleWall();
	};
	struct PrintInfo2D // ���������� �� GameManager � Interpreter (��� ������ ����� ��������; � ����� �� UpdateInfo)
	{
		//temp
		Coord playerPos;
		std::vector<Wall*> walls;
		std::vector<VisibleWall> visibleWalls;
		double viewLeft;
		double viewRight;

		PrintInfo2D();
	};


	// ----------------------------------------- �������� ������ ���� -----------------------------------------
	class MainMenu // ��� ������ ���� ������� GameManager (����� ��� ������ �� ������ � �.�.) � 
		//�������� ���������� ��� ����� � Interpreter`� (������ � ������� �� GameManager)
	{
		sf::RenderWindow *window;

	public:
		MainMenu();
		int Menu();
	};


	class GameManager // 2D ������ � ������� ����������� ���� (������, ��������, ��������� ������ � �.�.)
	{

		std::vector<Wall*> walls; // temp
		Coord playerPos;
		double view;

		std::vector<VisibleWall> GetVisibleWalls();

	public:
		GameManager(); // ���� �����
		PrintInfo2D Update2D(UpdateInfo&);
	};


	class Interpreter // ������ ���� �������� �������� ������ � GameManager � ������������ ���������� �� ���� ������
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
