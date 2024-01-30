#include "Game.h"
#include <iostream>

Game::Game(const std::string& config)
{
	init(config);
}

void Game::run()
{
	auto player1 = m_entities.addEntity("player");
	auto enemy1 = m_entities.addEntity("enemy");
	m_entities.addEntity("enemy");
	m_entities.addEntity("enemy");
	m_entities.addEntity("enemy");
	m_entities.addEntity("enemy");
	m_entities.update();
	
	/*
	while (m_running)
	{
		m_entities.update();

		sEnemySpawner();
		sMovement();
		sCollision();
		sUserInput();
		sRender();

		m_currentFrame++;
	}*/
	

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
}

void Game::sLifespan()
{
}

void Game::spawnPlayer()
{
	
}

void Game::spawnEnemy()
{
	auto enemy = m_entities.addEntity("enemy");
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
