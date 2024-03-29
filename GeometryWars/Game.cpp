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
			sLifespan();
		}

		sUserInput();
		sRender();

		if (!m_entities.getEntities("player").size())
		{
			spawnPlayer();
			m_lastSpecialWeaponSpawnTime = 0;
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

	// Boss movement
	for (auto& entity : m_entities.getEntities("boss"))
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
			entity->cTransform->velocity.x = -entity->cTransform->velocity.x;
		}
		else if (entity->cTransform->pos.y + entity->cCollision->radius > winY || entity->cTransform->pos.y - entity->cCollision->radius < 0.f)
		{
			entity->cTransform->velocity.y = -entity->cTransform->velocity.y;
		}

		entity->cTransform->pos += entity->cTransform->velocity;		
	}

	// Special bullet movement
	for (auto entity : m_entities.getEntities("specialBullet"))
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
}

void Game::sUserInput()
{
	sf::Event event;
	while (m_window.pollEvent(event))
	{
		// this event triggers when the window is closed
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
				//std::cout << "W key Pressed\n";
				m_player->cInput->up = true;
				break;
			case sf::Keyboard::S:
				//std::cout << "S key Pressed\n";
				m_player->cInput->down = true;
				break;
			case sf::Keyboard::A:
				//std::cout << "A key Pressed\n";
				m_player->cInput->left = true;
				break;
			case sf::Keyboard::D:
				//std::cout << "D key Pressed\n";
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
				if (m_currentFrame - m_lastSpecialWeaponSpawnTime >= 100)
				{					
					m_lastSpecialWeaponSpawnTime = m_currentFrame;
					spawnSpecialWeapon(m_player);				
					
				}					
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
				m_bossScore++;
			}
		}
	}

	// Bullet boss collision
	for (auto& b : m_entities.getEntities("bullet"))
	{
		for (auto& e : m_entities.getEntities("boss"))
		{
			Vec2 bulletLoc = { b->cTransform->pos.x, b->cTransform->pos.y };
			Vec2 enemyLoc = { e->cTransform->pos.x, e->cTransform->pos.y };
			Vec2 D = enemyLoc - bulletLoc;
			float bothColl = (b->cCollision->radius + e->cCollision->radius);

			if ((D.x * D.x + D.y * D.y) < (bothColl * bothColl))
			{
				b->destroy();
				e->cLifespan->total -= e->cLifespan->remaining;

				if (e->cLifespan->total <= 0)
				{
					spawnSmallEnemies(e);
					e->destroy();
					m_player->cScore->score += (e->cShape->shape.getPointCount() * 2);					
				}				
			}
		}
	}

	// Special bullet enemy collision
	for (auto& b : m_entities.getEntities("specialBullet"))
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

	// Player enemy collision
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

	// Player boss collision
	for (auto& p : m_entities.getEntities("player"))
	{
		for (auto& e : m_entities.getEntities("boss"))
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
	int timeForSpawn = 100;	

	if (m_currentFrame - m_lastEnemySpawnTime >= timeForSpawn)
	{
		spawnEnemy();		
	}
	if (m_bossScore > 5)
	{
		spawnBoss();
		m_bossScore = 0;
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

	// Cooldown text
	sf::Color markColor;
	if (m_currentFrame - m_lastSpecialWeaponSpawnTime < 100.f)
	{		
		markColor = sf::Color::Red;
	}
	else
	{		
		markColor = sf::Color::Green;
	}

	sf::CircleShape cooldwnMark;
	cooldwnMark.setPointCount(32);
	cooldwnMark.setFillColor(markColor);
	cooldwnMark.setPosition(110.f, 40.f);
	cooldwnMark.setRadius(5.f);
	m_window.draw(cooldwnMark);

	sf::Text specTxt;
	specTxt.setFont(m_font);
	specTxt.setPosition(0.f, 35.f);
	specTxt.setString("SPECIAL WEAPON: ");
	specTxt.setCharacterSize(15);
	specTxt.setFillColor(sf::Color::White);
	m_window.draw(specTxt);

	// Info text
	sf::Text escTxt;
	escTxt.setFont(m_font);
	escTxt.setPosition(sf::Vector2f(0.f, m_window.getSize().y - 2 * escTxt.getCharacterSize()));
	escTxt.setString("'ESC' to exit \n 'P' to pause");
	escTxt.setCharacterSize(20);
	escTxt.setFillColor(sf::Color::White);
	m_window.draw(escTxt);

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

	// Set boss
	for (auto boss : m_entities.getEntities("boss"))
	{
		boss->cShape->shape.setPosition(boss->cTransform->pos.x, boss->cTransform->pos.y);
		boss->cTransform->angle += 1.f;
		boss->cShape->shape.setRotation(boss->cTransform->angle);
		m_window.draw(boss->cShape->shape);
	}

	// Set bullet
	for (auto& bullet : m_entities.getEntities("bullet"))
	{
		bullet->cShape->shape.setPosition(bullet->cTransform->pos.x, bullet->cTransform->pos.y);
		m_window.draw(bullet->cShape->shape);
	}

	// set special bullet
	for (auto specialBullet : m_entities.getEntities("specialBullet"))
	{
		specialBullet->cShape->shape.setPosition(specialBullet->cTransform->pos.x, specialBullet->cTransform->pos.y);
		m_window.draw(specialBullet->cShape->shape);
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
	// bullet lifespan
	for (auto& b : m_entities.getEntities("bullet"))
	{
		int fade = (b->cLifespan->remaining * 255) / b->cLifespan->total;
		b->cShape->shape.setFillColor(sf::Color(255, 255, 255, fade));
		b->cShape->shape.setOutlineColor(sf::Color(255, 255, 255, fade));

		--b->cLifespan->remaining;

		if (!fade)
		{
			b->destroy();
		}
	}

	// special bullet lifespan
	for (auto& b : m_entities.getEntities("specialBullet"))
	{
		int fade = (b->cLifespan->remaining * 255) / b->cLifespan->total;
		b->cShape->shape.setFillColor(sf::Color(255, 255, 0, fade));
		b->cShape->shape.setOutlineColor(sf::Color(255, 255, 255, fade));

		--b->cLifespan->remaining;

		if (!fade)
		{
			b->destroy();
		}
	}

	// boss lifespan
	for (auto e : m_entities.getEntities("boss"))
	{		
		e->cShape->shape.setOutlineThickness(e->cLifespan->total/10.f);
	}

	// small enemy lifespan
	for (auto& b : m_entities.getEntities("smallEnemy"))
	{
		int fade = (b->cLifespan->remaining * 255) / b->cLifespan->total;

		b->cShape->shape.setFillColor(sf::Color(b->cShape->shape.getFillColor().r,
			b->cShape->shape.getFillColor().g, 
			b->cShape->shape.getFillColor().b, 
			fade));

		b->cShape->shape.setOutlineColor(sf::Color(255, 255, 255, fade));

		--b->cLifespan->remaining;		

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

	enemy->cShape = new CShape(
		22.f,
		getRandom(3.f, 9.f),
		sf::Color(getRandom(0.f, 255.f), getRandom(0.f, 255.f), getRandom(0.f, 255.f)),
		sf::Color(255, 255, 255),
		3.f);

	enemy->cCollision = new CCollision(enemy->cShape->shape.getRadius() + 4.f);
	float posX = getRandom(enemy->cCollision->radius, m_window.getSize().x - enemy->cCollision->radius);
	float posY = getRandom(enemy->cCollision->radius, m_window.getSize().y - enemy->cCollision->radius);
	
	
	enemy->cTransform = new CTransfrom(
		Vec2(posX, posY), 
		Vec2(getRandom(-3.f,3.f), getRandom(-3.f, 3.f)),
		0.f);

	enemy->cShape->shape.setOrigin(enemy->cShape->shape.getRadius(), enemy->cShape->shape.getRadius());

	// Record when the most recent enemy was spawned
	m_lastEnemySpawnTime = m_currentFrame;
}

void Game::spawnBoss()
{
	auto boss = m_entities.addEntity("boss");
	float grayRnd = getRandom(0.f, 255.f);

	boss->cShape = new CShape(
		40.f,
		getRandom(3.f, 9.f),
		sf::Color(0, 0, 0),
		sf::Color(255, 0, 0),
		10.f);

	boss->cCollision = new CCollision(boss->cShape->shape.getRadius() + 4.f);
	float posX = getRandom(boss->cCollision->radius, m_window.getSize().x - boss->cCollision->radius);
	float posY = getRandom(boss->cCollision->radius, m_window.getSize().y - boss->cCollision->radius);

	boss->cTransform = new CTransfrom(
		Vec2(posX, posY),
		Vec2(getRandom(-3.f, 3.f), getRandom(-3.f, 3.f)),
		0.f);

	boss->cLifespan = new CLifespan(10, 100);		
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
			Vec2(1.f * std::cos(angleRadians), 
				 1.f * std::sin(angleRadians)),
			entity->cTransform->angle);	

		smallEnemy->cCollision = new CCollision(entity->cCollision->radius);

		smallEnemy->cLifespan = new CLifespan(60, 60);
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
	bullet->cTransform = new CTransfrom(playerOrigin, bulletDirect * 10.f, 0.f);
	bullet->cLifespan = new CLifespan(60,60);
}

void Game::spawnSpecialWeapon(Entity* entity)
{
	// arg entity is player
	int edgeNum = entity->cShape->shape.getPointCount();
	float divAngle = 360.f / edgeNum;	
	
	for (int i = 0; i < edgeNum; i++)
	{
		auto specialBullet = m_entities.addEntity("specialBullet");
		float angleRadians = PI / 180.f * divAngle * i;

		specialBullet->cShape = new CShape(5.f, 32, sf::Color(255, 255, 0), sf::Color(255, 255, 255), 0.f);
		specialBullet->cShape->shape.setOrigin(5.f, 5.f);
		specialBullet->cCollision = new CCollision(8.f);

		specialBullet->cTransform = new CTransfrom(
			entity->cTransform->pos,
			Vec2(5.f * std::cos(angleRadians),
				5.f * std::sin(angleRadians)),
			0.f);

		specialBullet->cLifespan = new CLifespan(100, 100);
	}

	m_lastSpecialWeaponSpawnTime = m_currentFrame;
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
