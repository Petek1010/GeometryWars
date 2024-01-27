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

	// constructor
	Entity(const size_t id, const std::string tag) {}

public:

	// pointers to components
	CTransfrom* cTransform;
	CShape* cShape;
	CCollision* cCollision;
	CInput* cInput;
	CScore* cScore;
	CLifespan* cLifespan;


	bool IsActive() const;
	const size_t getId() const;
	const std::string & getTag() const;
	void destroy();
};