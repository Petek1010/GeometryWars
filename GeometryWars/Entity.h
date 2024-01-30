#pragma once

#include "Components.h"
#include <string>
#include <memory>

class Entity
{
	friend class EntityManager;

	bool m_active = true;
	size_t m_id = 0;
	std::string m_tag = "default";

private:
	// constructor
	Entity(const size_t id, const std::string tag) : m_id(id), m_tag(tag) {}

public:
	
	// pointers to components
	CTransfrom* cTransform = nullptr;
	CShape* cShape = nullptr;
	CCollision* cCollision = nullptr;
	CInput* cInput = nullptr;
	CScore* cScore = nullptr;
	CLifespan* cLifespan = nullptr;


	bool IsActive() const;
	const size_t getId() const;
	const std::string & getTag() const;
	void destroy();
};