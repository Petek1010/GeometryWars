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

	//DEBUG///////////////////////////////////////////////////
	std::cout << "m_toAdd size: " << m_toAdd.size() << "\n";
	for (auto& i : m_toAdd)
	{
		//std::cout << "element of m_toAdd TAG: " << i->getTag() << "\n";
		//std::cout << "element of m_toAdd ID: " << i->getId() << "\n";
	}
	////////////////////////////////////////////////////////////
	
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
	// DEBUG////////////////////////////////////////////////////////////////////
	//std::cout << "------------" << "UPDATE" << "----------------" << "\n";
	//std::cout << "m_entities size: " << getEntities().size() << "\n";
	for (auto& i : m_entities)
	{
		//std::cout << "element of m_entities TAG: " << i->getTag() << "\n";
		//std::cout << "element of m_entities ID: " << i->getId() << "\n";
	}
	
	for (auto& m : m_entityMap)
	{
		//std::cout << "element of m_entityMap first: " << m.first << " " << m.second.size() << "\n";
	}

	//////////////////////////////////////////////////////////////////////////////////////////////




	auto newEndVec = std::remove_if(m_entities.begin(), m_entities.end(), [](Entity* entity) 
		{
			return !entity->IsActive();
		});
	
	m_entities.erase(newEndVec, m_entities.end());

	for (auto ent : m_entityMap)
	{
		/*auto newEndMap = std::remove_if(m_entityMap[ent.].begin(), m_entityMap[ent->getTag()].end(), [](Entity* entity)
			{
				return !entity->IsActive();
			});*/
		
	}

	

	//auto newEnd = std::remove_if(m_entities.begin(), m_entities.end(), readyToRemove);
	//m_entities.erase(newEnd, m_entities.end());


	///////////////////// NEW IMPLEMENTATION ////////////////////////////////

	// remove dead entities from the vector of all entities
	removeDeadEntities(m_entities);

	// remove entities from each vector in the entity map
	/*for (auto& [tag, entityVec] : m_entityMap)
	{

	}*/

	
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
}


