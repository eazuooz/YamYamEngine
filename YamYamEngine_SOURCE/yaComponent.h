#pragma once
#include "yaEntity.h"

namespace ya
{
	class Component : public Entity
	{
	public:
		enum class eType
		{
			Component = 0,
			Transform,
			Camera,

			//render
			Mesh,
			Tile,
			Particle,
			SkyBox,
			Decal,
			
			Script,
			End,
		};

		Component();
		virtual ~Component();

		virtual void Initialize();
		virtual void Update();
		virtual void FixedUpdate();
		virtual void Render();

	private:
		const eType mType;


	};
}
