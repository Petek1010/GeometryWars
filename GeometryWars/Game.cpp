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

	// window border collision
	float winX = m_window.getSize().x;
	float winY = m_window.getSize().y;
	float playerPosX = m_player->cTransform->pos.x;
	float playerPosY = m_player->cTransform->pos.y;
	float playerColl = m_player->cCollision->radius;

	// Player movement
	if (playerPosY - playerColl > 0 && playerPosY + playerColl < winY)
	{
		if (m_player->cInput->up)
		{
			m_player->cTransform->velocity.y = -5.f;
		}
		else if (m_player->cInput->down)
		{
			m_player->cTransform->velocity.y = 5.f;
		}
		
	}
	else
	{
		if (playerPosY - playerColl < 0 && m_player->cInput->down)
		{
			m_player->cTransform->velocity.y = 5.f;
		}
		if (playerPosY + playerColl > winY && m_player->cInput->up)
		{
			m_player->cTransform->velocity.y = -5.f;
		}
	}

	if (playerPosX - playerColl > 0 && playerPosX + playerColl < winX)
	{
		if (m_player->cInput->left)
		{
			m_player->cTransform->velocity.x = -5.f;
		}
		else if (m_player->cInput->right)
		{
			m_player->cTransform->velocity.x = 5.f;
		}
	}
	else
	{
		if (playerPosX - playerColl < 0 && m_player->cInput->right)
		{
			m_player->cTransform->velocity.x = 5.f;
		}
		if (playerPosX + playerColl > winX && m_player->cInput->left)
		{
			m_player->cTransform->velocity.x = -5.f;
		}
	}
	
	// movement speed update
	m_player->cTransform->pos.x += m_player->cTransform->velocity.x;
	m_player->cTransform->pos.y += m_player->cTransform->velocity.y;

	
	// Enemy movement
	for (auto& entity : m_entities.getEntities("enemy"))
	{
		if (entity->cTransform->pos.x + entity->cCollision->radius > winX || entity->cTransform->pos.x - entity->cCollision->radius < 0.f)
		{
			entity->cTransform->velocity.x = -entity->cTransform->velocity.x;
		}
		else if (entity->cTransform->pos.y + entity->cCollision->radius > winY || entity->cTransform->pos.y - entity->cCollision->radius < 0.f)
		{
			entity->cTransform->velocity.y = -entity->cTransform->velocity.y;
		}

		entity->cTransform->pos += entity->cTransform->velocity;
	}

	// Bullet movement
	for (auto& entity : m_entities.getEntities("bullet"))
	{
		if (entity->cTransform->pos.x + entity->cCollision->radius > winX || entity->cTransform->pos.x - entity->cCollision->radius < 0.f)
		{
			//entity->cTransform->velocity.x = -entity->cTransform->velocity.x;
			entity->destroy();
		}
		else if (entity->cTransform->pos.y + entity->cCollision->radius > winY || entity->cTransform->pos.y - entity->cCollision->radius < 0.f)
		{
			//entity->cTransform->velocity.y = -entity->cTransform->velocity.y;
			entity->destroy();
		}

		entity->cTransform->pos += entity->cTransform->velocity;
	}
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
				m_player->cInput->up = false;
				break;
			case sf::Keyboard::S:
				m_player->cInput->down = false;
				break;
			case sf::Keyboard::A:
				m_player->cInput->left = false;
				break;
			case sf::Keyboard::D:
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
				spawnBullet(m_player, Vec2(event.mouseButton.x, event.mouseButton.y));
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
	// Bullet enemy collision
	for (auto& b : m_entities.getEntities("bullet"))
	{
		for (auto& e : m_entities.getEntities("enemy"))
		{
			Vec2 bulletLoc = { b->cTransform->pos.x, b->cTransform->pos.y };
			Vec2 enemyLoc = { e->cTransform->pos.x, e->cTransform->pos.y };
			Vec2 D = enemyLoc - bulletLoc;
			float bothColl = (b->cCollision->radius + e->cCollision->radius);

			if ((D.x * D.x + D.y * D.y) < (bothColl * bothColl))
			{
				e->destroy();
				b->destroy();
			}
		}
	}

	for (auto& p : m_entities.getEntities("player"))
	{
		for (auto& e : m_entities.getEntities("enemy"))
		{
			Vec2 playerLoc = { p->cTransform->pos.x, p->cTransform->pos.y };
			Vec2 enemyLoc = { e->cTransform->pos.x, e->cTransform->pos.y };
			Vec2 D = enemyLoc - playerLoc;
			float bothColl = (p->cCollision->radius + e->cCollision->radius);

			if ((D.x * D.x + D.y * D.y) < (bothColl * bothColl))
			{
				std::cout << "RESTART GAME\n";
			}
		}
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

	// Set bullet
	for (auto& bullet : m_entities.getEntities("bullet"))
	{
		bullet->cShape->shape.setPosition(bullet->cTransform->pos.x, bullet->cTransform->pos.y);

		m_window.draw(bullet->cShape->shape);
	}

	


	m_window.display();	
}

void Game::sLifespan()
{
	for (auto& b : m_entities.getEntities("bullet"))
	{
		b->cShape->shape.setFillColor(sf::Color(255, 255, 255, 50));
	}
}

void Game::spawnPlayer()
{
	auto player = m_entities.addEntity("player");

	float midX = m_window.getSize().x;
	float midY = m_window.getSize().y;


	player->cTransform = new CTransfrom(Vec2(midX/2, midY/2), Vec2(1.0f, 1.0f), 0.f);
	player->cShape = new CShape(32.f, 8, sf::Color(10, 10, 10), sf::Color(255, 0, 0), 4.f);
	player->cInput = new CInput();
	player->cCollision = new CCollision(36.f);
	player->cScore = new CScore(0.f);

	player->cShape->shape.setOrigin(32.f,32.f);
	
	
	m_player = player;

	 

	
}

void Game::spawnEnemy()
{
	auto enemy = m_entities.addEntity("enemy");

	enemy->cCollision = new CCollision(20.f);
	float posX = getRandom(enemy->cCollision->radius, m_window.getSize().x - enemy->cCollision->radius);
	float posY = getRandom(enemy->cCollision->radius, m_window.getSize().y - enemy->cCollision->radius);
	

	enemy->cTransform = new CTransfrom(Vec2(posX, posY), Vec2(1.0f, 1.0f), 0.f);
	enemy->cShape = new CShape(
		16.f, 
		getRandom(3.f, 9.f), 
		sf::Color(getRandom(0.f, 255.f), getRandom(0.f, 255.f), getRandom(0.f, 255.f)),
		sf::Color(255, 255, 255), 
		4.f);

	enemy->cShape->shape.setOrigin(16.f, 16.f);

	// Record when the most recent enemy was spawned
	m_lastEnemySpawnTime = m_currentFrame;
}

void Game::spawnSmallEnemies(Entity* entity)
{
}

void Game::spawnBullet(Entity* entity, const Vec2& mousePos)
{
	auto bullet = m_entities.addEntity("bullet");
	
	Vec2 playerOrigin = {entity->cTransform->pos.x, entity->cTransform->pos.y };
	Vec2 bulletTargetLoc = mousePos;
	Vec2 bulletDirect = (mousePos - playerOrigin);
	bulletDirect.normalize();

	bullet->cShape = new CShape(5.f, 32, sf::Color(255, 255, 255), sf::Color(255, 255, 255), 1.f);
	bullet->cShape->shape.setOrigin(5.f, 5.f);
	bullet->cCollision = new CCollision(8.f);
	bullet->cTransform = new CTransfrom(playerOrigin, bulletDirect * 5.f, 0.f);
	bullet->cLifespan = new CLifespan(30,30);

	

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
