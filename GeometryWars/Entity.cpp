#include "Entity.h"

bool Entity::IsActive() const
{
   return m_active;
}

const size_t Entity::getId() const
{
   return m_id;
}

const std::string& Entity::getTag() const
{
   return m_tag;
}


void Entity::destroy()
{
	m_active = false;
}
