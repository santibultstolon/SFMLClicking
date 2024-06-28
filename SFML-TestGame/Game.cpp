#include "Game.h"

//Private Functions
void Game::InitializeVariables()
{
	this->window = nullptr;
	//Game Logic

	this->points = 0;
	this->enemySpawnTimerMax = 10.f;
	this->enemySpawnTimer = this->enemySpawnTimerMax;
	this->maxEnemies = 5;
}

void Game::InitWindow()
{
	this->videoMode.height = 600;
	this->videoMode.width = 800;

	this->window = new sf::RenderWindow(this->videoMode, "Click destroy");

	this->window->setFramerateLimit(144);
}

//Constructor
Game::Game()
{
	this->InitializeVariables();
	this->InitWindow();
	this->InitEnemies();
}
//Destructor
Game::~Game()
{
	delete this->window;
}

void Game::InitEnemies()
{
	this->enemy.setPosition(200.f,200.f);
	this->enemy.setSize(sf::Vector2f(50.f, 50.f));
	this->enemy.setFillColor(sf::Color::Cyan);
	//this->enemy.setOutlineColor(sf::Color::Green);
	//this->enemy.setOutlineThickness(1.f);
}

//Accessors

const bool Game::isRunning() const
{
	return this->window->isOpen();
}

//Functions

void Game::PollEvents()
{
	//Event polling
	while (this->window->pollEvent(this->event))
	{
		switch (this->event.type)
		{
		case sf::Event::Closed:
			this->window->close();
			break;
		case sf::Event::KeyPressed:
			if(this->event.key.code == sf::Keyboard::Escape)
				this->window->close();
			break;
		}
	}
}


void Game::Update()
{
	this->PollEvents();

	this->UpdateMousePositions();

	this->UpdateEnemies();

	std::cout << "Enemy count : " << enemies.size() << std::endl;
}



void Game::Render()
{
	this->window->clear();

	this->RenderEnemies();

	this->window->display();
}

void Game::UpdateMousePositions()
{
	/*
	@return void

	Updates the mouse position :
	 - Mouse position relative to window,
	*/

	this->mousePosWindow = sf::Mouse::getPosition(*this->window);
	this->mousePosView = this->window->mapPixelToCoords(this->mousePosWindow);
}

void Game::SpawnEnemy()
{
	/*
		@return void 

		Spawns enemies and sets their color and positions.
		-Sets a random position.
		-Sets a random color.
		-Adds enemy to the vector.
	*/

	this->enemy.setPosition(
		static_cast<float>(rand() % static_cast<int>(this->window->getSize().x - this->enemy.getSize().x)),
		0.f
	);


	this->enemy.setFillColor(sf::Color::Green);

	//Spawns the enemy
	this->enemies.push_back(this->enemy);

	//Remove enemies at the end of the screen
	
}

void Game::UpdateEnemies()
{
	/**
		@return void

		Updates the enemy spawn timer and spawns enemies
		when the total amount of enemies is smaller than the maximum.
		Moves the enemies downwards.
		Removes the enemies at the edge of the screen. //TODO
	*/


	//Updating timer for enemy spawning
	if (this->enemies.size() < this->maxEnemies) 
	{
		if (this->enemySpawnTimer >= this->enemySpawnTimerMax) 
		{
			//Spawn enemy and reset the timer
			this->SpawnEnemy();
			this->enemySpawnTimer = 0.f;
		}
		else
			this->enemySpawnTimer += 1.f;
	}

	//Move the enemies
	for (int i =0; i < enemies.size(); i++)
	{
		bool deleted = false;
		this->enemies[i].move(0.f, 1.f);

		//Check if clicked upon
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) 
		{
			if (this->enemies[i].getGlobalBounds().contains(this->mousePosView)) 
			{
				deleted = true;

				this->points += 10.f;
			}
		}

		if (this->enemies[i].getPosition().y > this->window->getSize().y)
			deleted = true;

		//Delete
		if(deleted)
			this->enemies.erase(this->enemies.begin() + i);
	}
}

void Game::RenderEnemies()
{
	for (auto& e : this->enemies)
	{
		this->window->draw(e);
	}
}
