#pragma once
#include <vector>
#include <map>
#include <string>
#include "Entity.h"

typedef std::vector<Entity*> EntityVector;
typedef std::map<std::string, EntityVector> EntityMap;

class EntityManager
{
	EntityVector m_entities;
	EntityVector m_toAdd;
	EntityMap m_entityMap;
	size_t m_totalEntities = 0;

public:

	EntityManager();
	~EntityManager();

	Entity* addEntity(const std::string& tag);
	void update();
	EntityVector& getEntities();
	EntityVector& getEntities(const std::string& tag);
	
};