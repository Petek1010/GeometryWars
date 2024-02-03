#include "Game.h"
#include <iostream>

Game::Game(const std::string& config)
{
	init(config);
}

void Game::run()
{
	
	while (m_running)
	{
		m_entities.update();

		sEnemySpawner();
		sMovement();
		sCollision();
		sUserInput();
		sRender();

		m_currentFrame++;
	}
	
	//delete player1; TODO
	delete m_player->cTransform;
}

void Game::init(const std::string& config)
{
	//TODO: we read config.txt and asssign structs

	m_window.create(sf::VideoMode(1280, 720), "Geometry Wars");
	m_window.setFramerateLimit(60);

	spawnPlayer();

	
}

void Game::setPaused(bool paused)
{
	// TODO
}

void Game::sMovement()
{
	m_player->cTransform->velocity = { 0,0 };

	if (m_player->cInput->up)
	{
		m_player->cTransform->velocity.y = -5.f;
	}
	else if (m_player->cInput->down)
	{
		m_player->cTransform->velocity.y = 5.f;
	}

	if (m_player->cInput->left)
	{
		m_player->cTransform->velocity.x = -5.f;
	}
	else if (m_player->cInput->right)
	{
		m_player->cTransform->velocity.x = 5.f;
	}


	// movement speed update
	m_player->cTransform->pos.x += m_player->cTransform->velocity.x;
	m_player->cTransform->pos.y += m_player->cTransform->velocity.y;

	// TODO for every entity

}

void Game::sUserInput()
{
	sf::Event event;
	while (m_window.pollEvent(event))
	{
		// this event riggers when the window is closed
		if (event.type == sf::Event::Closed)
		{
			m_running = false;
		}

		// this event is triggered when a key is pressed
		if (event.type == sf::Event::KeyPressed)
		{
			switch (event.key.code)
			{
			case sf::Keyboard::W:
				std::cout << "W key Pressed\n";
				m_player->cInput->up = true;
				break;
			case sf::Keyboard::S:
				std::cout << "S key Pressed\n";
				m_player->cInput->down = true;
				break;
			case sf::Keyboard::A:
				std::cout << "A key Pressed\n";
				m_player->cInput->left = true;
				break;
			case sf::Keyboard::D:
				std::cout << "D key Pressed\n";
				m_player->cInput->right = true;
				break;
			default: 
				break;	
			}
		}

		// this event is triggered when a key is released
		if (event.type == sf::Event::KeyReleased)
		{
			switch (event.key.code)
			{
			case sf::Keyboard::W:
				std::cout << "W key Released\n";
				m_player->cInput->up = false;
				break;
			case sf::Keyboard::S:
				std::cout << "S key Released\n";
				m_player->cInput->down = false;
				break;
			case sf::Keyboard::A:
				std::cout << "A key Released\n";
				m_player->cInput->left = false;
				break;
			case sf::Keyboard::D:
				std::cout << "D key Released\n";
				m_player->cInput->right = false;
				break;
			default:
				break;
			}
		}

		if (event.type == sf::Event::MouseButtonPressed)
		{
			if (event.mouseButton.button == sf::Mouse::Left)
			{
				std::cout << "Shoot\n";
				std::cout << event.mouseButton.x << " " << event.mouseButton.y << "\n";


				// call spawn bullet
			}

			if (event.mouseButton.button == sf::Mouse::Right)
			{
				std::cout << "Spawn special\n";
				std::cout << event.mouseButton.x << " " << event.mouseButton.y << "\n";
			}


		}


		// Window exit and pause
		if (event.type == sf::Event::KeyPressed)
		{
			if (event.key.code == sf::Keyboard::Escape)
			{
				m_running = false;
			}

			if (event.key.code == sf::Keyboard::P)
			{
				// Pausing the game TODO
			}
		}


	}
}

void Game::sCollision()
{
	// window border collision
	float winX = m_window.getSize().x;
	float winY = m_window.getSize().y;
	float playerColl = m_player->cCollision->radius;
	
	if ((m_player->cTransform->pos.y + playerColl) > winY)
	{
		std::cout << "COLLIDING\n";
		m_player->cInput->down = false;
	}


	for (auto& e : m_entities.getEntities())
	{

	}
}

void Game::sEnemySpawner()
{	
	int timeForSpawn = 250;
	if (m_currentFrame - m_lastEnemySpawnTime == timeForSpawn)
	{
		spawnEnemy();		
	}
}

void Game::sRender()
{
	m_window.clear();

	// Set player
	m_player->cShape->shape.setPosition(m_player->cTransform->pos.x, m_player->cTransform->pos.y);
	m_player->cTransform->angle += 1.0f;
	m_player->cShape->shape.setRotation(m_player->cTransform->angle);

	m_window.draw(m_player->cShape->shape);
	
	// Set enemy
	for (auto& enemy : m_entities.getEntities("enemy"))
	{
		enemy->cShape->shape.setPosition(enemy->cTransform->pos.x, enemy->cTransform->pos.y);
		enemy->cTransform->angle += 1.5f;
		enemy->cShape->shape.setRotation(enemy->cTransform->angle);

		m_window.draw(enemy->cShape->shape);
	}



	m_window.display();	
}

void Game::sLifespan()
{
}

void Game::spawnPlayer()
{
	auto player = m_entities.addEntity("player");

	float midX = m_window.getSize().x;
	float midY = m_window.getSize().y;


	player->cTransform = new CTransfrom(Vec2(midX/2, midY/2), Vec2(1.0f, 1.0f), 0.f);
	player->cShape = new CShape(32.f, 8, sf::Color(10, 10, 10), sf::Color(255, 0, 0), 4.f);
	player->cInput = new CInput();
	player->cCollision = new CCollision(32.f);
	player->cScore = new CScore(0.f);

	player->cShape->shape.setOrigin(32.f,32.f);
	
	
	m_player = player;

	 

	
}

void Game::spawnEnemy()
{
	auto enemy = m_entities.addEntity("enemy");

	float posX = getRandom(0.f, m_window.getSize().x);
	float posY = getRandom(0.f, m_window.getSize().y);

	enemy->cTransform = new CTransfrom(Vec2(posX, posY), Vec2(1.0f, 1.0f), 0.f);
	enemy->cShape = new CShape(16.f, 3, sf::Color(0, 0, 255), sf::Color(255, 255, 255), 4.f);
	enemy->cCollision = new CCollision(16.f);


	// Record when the most recent enemy was spawned
	m_lastEnemySpawnTime = m_currentFrame;
}

void Game::spawnSmallEnemies(Entity* entity)
{
}

void Game::spawnBullet(Entity* entity, const Vec2& mousePos)
{
	auto bullet = m_entities.addEntity("bullet");
}

void Game::spawnSpecialWeapon(Entity* entity)
{
}

float Game::getRandom(const float first, const float second) const
{
	// Create a Mersenne Twister engine with a random seed
	std::random_device rd;
	std::mt19937 mt(rd());

	// Define a uniform distribution for integers between 1 and 100
	std::uniform_real_distribution<float> distribution(first, second);

	// Generate a random number
	float random_number = distribution(mt);

	return random_number;
}
