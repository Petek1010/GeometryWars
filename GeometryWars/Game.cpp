#include "Game.h"
#include <iostream>

Game::Game(const std::string& config)
{
	init(config);
}

void Game::run()
{
	/*auto player1 = m_entities.addEntity("player");
	auto enemy1 = m_entities.addEntity("enemy");
	m_entities.addEntity("enemy");
	m_entities.addEntity("enemy");
	m_entities.addEntity("enemy");
	m_entities.addEntity("enemy");
	m_entities.update();*/
	
	
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
	
	//delete player1;
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
}

void Game::sUserInput()
{
}

void Game::sCollision()
{
}

void Game::sEnemySpawner()
{
}

void Game::sRender()
{
	m_window.clear();

	m_player->cShape->shape.setPosition(m_player->cTransform->pos.x, m_player->cTransform->pos.y);
	m_player->cTransform->angle += 1.0f;
	m_player->cShape->shape.setRotation(m_player->cTransform->angle);


	m_window.draw(m_player->cShape->shape);

	m_window.display();
	
}

void Game::sLifespan()
{
}

void Game::spawnPlayer()
{
	auto player = m_entities.addEntity("player");

	player->cTransform = new CTransfrom(Vec2(200.f, 200.f), Vec2(1.0f, 1.0f), 0.f);
	player->cShape = new CShape(32.f, 8, sf::Color(10, 10, 10), sf::Color(255, 0, 0), 4.f);
	player->cInput = new CInput();

	m_player = player;


	
}

void Game::spawnEnemy()
{
	auto enemy = m_entities.addEntity("enemy");

	//enemy->cTransform = new CTransfrom(Vec2(400.f, 400.f), Vec2(1.0f, 1.0f), 0.f);
}

void Game::spawnSmallEnemies(Entity* entity)
{
}

void Game::spawnBullet(Entity* entity, const Vec2& mousePos)
{
}

void Game::spawnSpecialWeapon(Entity* entity)
{
}
