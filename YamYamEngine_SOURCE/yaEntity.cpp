#include "yaEntity.h"

namespace ya
{
	Entity::Entity()
		: mID(reinterpret_cast<UINT32>(this))
	{
	}
	Entity::Entity(const Entity& other)
		: mID(reinterpret_cast<UINT32>(this))
		, mName(other.mName)
	{
	}
	Entity::~Entity()
	{
	}
}

namespace gui
{
	Entity::Entity()
		: mID(reinterpret_cast<UINT32>(this))
	{
	}
	Entity::Entity(const Entity& other)
		: mID(reinterpret_cast<UINT32>(this))
		, mName(other.mName)
	{
	}
	Entity::~Entity()
	{
	}
}
