#pragma once
#include "Entity.h"
#include "EntityManager.h"
#include "SFML/Graphics.hpp"

struct PlayerConfig { int SR, CR, FR, FG, FB, OR, OG, OB, OT, V; float S; };
struct EnemyConfig { int SR, CR, OR, OG, OB, OT, VMIN, VMAX, L, SI; float SMIN, SMAX; };
struct BulletConfig { int SR, CR, FR, FG, FB, OR, OG, OB, OT, V, L; float S; };

class Game
{
	sf::RenderWindow m_window;
	EntityManager m_entities;
	sf::Text m_text;
	sf::Font m_font;
	PlayerConfig m_playerConfig;
	EnemyConfig m_enemyConfig;
	BulletConfig m_bulletConfig;
	int m_score = 0;
	int m_currentFrame = 0;
	int m_lastEnemySpawnTime = 0;
	bool m_paused = false;
	bool m_running = true;

	Entity* player = nullptr;

	// Initialize the GameState
	void init(const std::string& config);
	void setPaused(bool paused);

	// Systems
	void sMovement();
	void sUserInput();
	void sCollision();
	void sEnemySpawner();
	void sRender();
	void sLifespan();

	// Functions
	void spawnPlayer();
	void spawnEnemy();
	void spawnSmallEnemies(Entity* entity);
	void spawnBullet(Entity* entity, const Vec2& mousePos);
	void spawnSpecialWeapon(Entity* entity);

public:

	Game(const std::string& config);

	void run();

};