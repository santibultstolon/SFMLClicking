#pragma once

#include <iostream>
#include <vector>
#include <ctime>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

/*
	Class which tries to act like the game engine.
*/
class Game
{
private:
	//Window
	sf::RenderWindow* window;
	sf::VideoMode videoMode;
	sf::Event event;

	//Mouse Positions
	sf::Vector2i mousePosWindow;
	sf::Vector2f mousePosView;

	//Game Logic
	int points;
	float enemySpawnTimer;
	float enemySpawnTimerMax;
	int maxEnemies;



	//Game Objects
	std::vector<sf::RectangleShape> enemies;
	sf::RectangleShape enemy;

private:
	void InitializeVariables();
	void InitWindow();
	void InitEnemies();

public:
	//Constructor and Destructor
	Game();
	virtual ~Game();

	//Accessors
	const bool isRunning() const;

	//Functions
	void SpawnEnemy();

	void PollEvents();
	void UpdateMousePositions();
	void UpdateEnemies();
	void Update();
	void RenderEnemies();
	void Render();
};

