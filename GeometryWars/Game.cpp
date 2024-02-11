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

		if (!m_paused)
		{
			sEnemySpawner();
			sMovement();
			sCollision();			
		}

		sUserInput();
		sRender();

		if (!m_entities.getEntities("player").size())
		{
			spawnPlayer();
		}	

		m_currentFrame++;
	}	
}

void Game::init(const std::string& config)
{
	//TODO: we read config.txt and asssign structs
	// Import font 
	if (!m_font.loadFromFile("D:/SFML_GameEngine/GeometryWars/Dosis.ttf"))
	{
		std::cerr << "Error opening the font file." << std::endl;
	}

	
	m_window.create(sf::VideoMode(1280, 720), "Geometry Wars");
	m_window.setFramerateLimit(60);

	spawnPlayer();

	
}

void Game::setPaused(bool paused)
{
	if (!paused)
	{
		m_paused = true;
	}
	else
	{
		m_paused = false;
	}
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

	// Small enemy movement
	for (auto entity : m_entities.getEntities("smallEnemy"))
	{
		sLifespan();
		entity->cTransform->pos += entity->cTransform->velocity;
	}

	// Bullet movement
	for (auto& entity : m_entities.getEntities("bullet"))
	{
		if (entity->cTransform->pos.x + entity->cCollision->radius > winX || entity->cTransform->pos.x - entity->cCollision->radius < 0.f)
		{			
			entity->destroy();
		}
		else if (entity->cTransform->pos.y + entity->cCollision->radius > winY || entity->cTransform->pos.y - entity->cCollision->radius < 0.f)
		{
			entity->destroy();
		}

		sLifespan();
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
				spawnSpecialWeapon(m_player);
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
				setPaused(m_paused);
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
				spawnSmallEnemies(e);
				e->destroy();
				b->destroy();
				m_player->cScore->score += e->cShape->shape.getPointCount();
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
				for (auto& e : m_entities.getEntities())
				{
					e->destroy();
				}
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

	// Score text	
	std::string scoreTxt = "SCORE: " + std::to_string(m_player->cScore->score);

	m_text.setFont(m_font);
	m_text.setPosition(sf::Vector2f(0.f, 0.f));
	m_text.setString(scoreTxt);
	m_text.setCharacterSize(32);
	m_text.setFillColor(sf::Color::White);
	m_window.draw(m_text);



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

	// Set small enemies	
	for (auto& smallEnemy : m_entities.getEntities("smallEnemy"))
	{
		smallEnemy->cShape->shape.setPosition(smallEnemy->cTransform->pos.x, smallEnemy->cTransform->pos.y);
		smallEnemy->cTransform->angle += 1.5f;
		smallEnemy->cShape->shape.setRotation(smallEnemy->cTransform->angle);
		m_window.draw(smallEnemy->cShape->shape);
	}

	m_window.display();	
}

void Game::sLifespan()
{
	
	for (auto& b : m_entities.getEntities("bullet"))
	{
		int fade = (b->cLifespan->remaining * 255) / b->cLifespan->total;
		//int newFade = (b->cLifespan->remaining / b->cLifespan->total) * 255;
		b->cShape->shape.setFillColor(sf::Color(255, 255, 255, fade));
		b->cShape->shape.setOutlineColor(sf::Color(255, 255, 255, fade));

		--b->cLifespan->remaining;

		//std::cout << fade << "\n";

		if (!fade)
		{
			b->destroy();
		}
	}

	// small enemies
	for (auto& b : m_entities.getEntities("smallEnemy"))
	{
		int fade = (b->cLifespan->remaining * 255) / b->cLifespan->total;

		b->cShape->shape.setFillColor(sf::Color(b->cShape->shape.getFillColor().r,
			b->cShape->shape.getFillColor().g, 
			b->cShape->shape.getFillColor().b, 
			fade));

		b->cShape->shape.setOutlineColor(sf::Color(255, 255, 255, fade));

		--b->cLifespan->remaining;

		std::cout << fade << "\n";

		if (!fade)
		{
			b->destroy();
		}
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
	
	int edgeNum = entity->cShape->shape.getPointCount();
	float divAngle = 360.f / edgeNum;

	for (int i = 0; i < edgeNum; i++)
	{
		auto smallEnemy = m_entities.addEntity("smallEnemy");
		float angleRadians = PI / 180.f * divAngle * i;

		smallEnemy->cShape = new CShape(entity->cShape->shape.getRadius(),
			edgeNum,
			entity->cShape->shape.getFillColor(),
			entity->cShape->shape.getOutlineColor(),
			entity->cShape->shape.getOutlineThickness());

		smallEnemy->cTransform = new CTransfrom(entity->cTransform->pos,
			Vec2(entity->cTransform->velocity.x * std::cos(angleRadians), 
				entity->cTransform->velocity.y * std::sin(angleRadians)),
			entity->cTransform->angle);		

		smallEnemy->cLifespan = new CLifespan(500, 500);

	}
	
	
}

void Game::spawnBullet(Entity* entity, const Vec2& mousePos)
{
	auto bullet = m_entities.addEntity("bullet");
	
	Vec2 playerOrigin = {entity->cTransform->pos.x, entity->cTransform->pos.y };
	Vec2 bulletTargetLoc = mousePos;
	Vec2 bulletDirect = (mousePos - playerOrigin);
	bulletDirect.normalize();

	bullet->cShape = new CShape(5.f, 32, sf::Color(255, 255, 255), sf::Color(255, 255, 255), 0.f);
	bullet->cShape->shape.setOrigin(5.f, 5.f);
	bullet->cCollision = new CCollision(8.f);
	bullet->cTransform = new CTransfrom(playerOrigin, bulletDirect * 5.f, 0.f);
	bullet->cLifespan = new CLifespan(50,50);

	

}

void Game::spawnSpecialWeapon(Entity* entity)
{
	int edgeNum = m_player->cShape->shape.getPointCount();
	float divAngle = 360.f / edgeNum;

	// TODO
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
