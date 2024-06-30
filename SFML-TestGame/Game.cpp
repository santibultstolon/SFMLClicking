#include "Game.h"

//Private Functions
void Game::InitializeVariables()
{
	this->window = nullptr;
	//Game Logic

	this->points = 0;
	this->enemySpawnTimerMax = 20.f;
	this->enemySpawnTimer = this->enemySpawnTimerMax;
	this->maxEnemies = 5;
	this->mouseHeld = false;
	this->health = 20;
	this->endGame = false;
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
	this->InitFonts();
	this->InitText();
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
void Game::InitFonts()
{
	if (!this->font.loadFromFile("Fonts/Monocraft.ttf")) 
	{
		std::cout << "ERROR::GAME::Failed to load font!\n" << std::endl;
	};
}

void Game::InitText()
{
	this->uiText.setFont(this->font);
	this->uiText.setCharacterSize(24);
	this->uiText.setFillColor(sf::Color::White);
	this->uiText.setString("NONE");
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

	if (this->endGame == false) 
	{
		this->UpdateMousePositions();

		this->UpdateText();

		this->UpdateEnemies();
	}

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

	//Randomize enemy type
	int type = rand() % 5;

	switch (type) 
	{
	case 0:
		this->enemy.setSize(sf::Vector2f(10.f, 10.f));
		this->enemy.setFillColor(sf::Color::Red);
		break;
	case 1:
		this->enemy.setSize(sf::Vector2f(30.f, 30.f));
		this->enemy.setFillColor(sf::Color::Magenta);
		break;
	case 2:
		this->enemy.setSize(sf::Vector2f(50.f, 50.f));
		this->enemy.setFillColor(sf::Color::Cyan);
		break;
	case 3:
		this->enemy.setSize(sf::Vector2f(70.f, 70.f));
		this->enemy.setFillColor(sf::Color::Blue);
		break;
	case 4:
		this->enemy.setSize(sf::Vector2f(100.f, 100.f));
		this->enemy.setFillColor(sf::Color::Green);
		break;
	default:
		this->enemy.setSize(sf::Vector2f(100.f, 100.f));
		this->enemy.setFillColor(sf::Color::Yellow);
		break;
	}

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

		this->enemies[i].move(0.f, 2.f);

		if (this->enemies[i].getPosition().y > this->window->getSize().y) 
		{
			this->enemies.erase(this->enemies.begin() + i);
			this->health -= 1;
			//End game condition
			if (this->health <= 0)
				this->endGame = true;
		}
	}

	//Check if clicked upon
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		if (this->mouseHeld == false) 
		{
			this->mouseHeld = true;
			bool deleted = false;

			for (size_t i = 0; i < this->enemies.size() && deleted == false; i++)
			{
				if (this->enemies[i].getGlobalBounds().contains(this->mousePosView))
				{
					deleted = true;
					if (this->enemies[i].getFillColor() == sf::Color::Magenta)
						this->points += 20;
					else if(this->enemies[i].getFillColor() == sf::Color::Cyan)
						this->points += 15;
					else if (this->enemies[i].getFillColor() == sf::Color::Blue)
						this->points += 10;
					else if (this->enemies[i].getFillColor() == sf::Color::Green)
						this->points += 5;
					else if (this->enemies[i].getFillColor() == sf::Color::Yellow)
						this->points += 5;
					else if (this->enemies[i].getFillColor() == sf::Color::Red)
						this->points += 25;

					this->enemies.erase(this->enemies.begin() + i);
				}
			}
		}
	}
	else
	{
		this->mouseHeld = false;
	}

}

void Game::RenderEnemies(sf::RenderTarget& target)
{
	for (auto& e : this->enemies)
	{
		target.draw(e);
	}
}


const bool Game::getEndGame() const
{
	return this->endGame;
}

void Game::RenderText(sf::RenderTarget& target)
{
	target.draw(this->uiText);
}

void Game::UpdateText()
{
	std::stringstream ss;

	ss << "Points: " << this->points << "\n"
		<< "Health: " << this->health << "\n";

	this->uiText.setString(ss.str());
}

void Game::Render()
{
	this->window->clear();

	this->RenderEnemies(*this->window);

	this->RenderText(*this->window);

	this->window->display();
}
