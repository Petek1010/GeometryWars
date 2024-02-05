#include "EntityManager.h"
#include <iostream>

EntityManager::EntityManager()
{
}

EntityManager::~EntityManager()
{
	
}

Entity* EntityManager::addEntity(const std::string& tag)
{
	// Creates new Entity() and gives each entity an increasing integer id
	Entity* ent = new Entity(++m_totalEntities, tag); 
	m_toAdd.push_back(ent);
	
	return ent;
}

void EntityManager::update()
{
	
	// Iterator invalidation solution
	for (auto& ent : m_toAdd)
	{
		m_entities.push_back(ent);
		m_entityMap[ent->getTag()].push_back(ent);
	}
	m_toAdd.clear();

	// remove dead entities from the vector of all entities
	removeDeadEntities(m_entities);

	// remove entities from each vector in the entity map
	for (auto& [tag, entityVec] : m_entityMap)
	{
		removeDeadEntities(entityVec);
	}	
}

EntityVector& EntityManager::getEntities()
{
	return m_entities;
}

EntityVector& EntityManager::getEntities(const std::string& tag)
{
	return m_entityMap[tag];
}

void EntityManager::removeDeadEntities(EntityVector& vector)
{
	auto EndVec = std::remove_if(vector.begin(), vector.end(), [](Entity* entity) {return !entity->IsActive(); });
	vector.erase(EndVec, vector.end());

}


