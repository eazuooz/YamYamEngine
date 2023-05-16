#pragma once
#include "guiResource.h"
#include "yaResource.h"

namespace gui
{
	class Mesh : public Resource
	{
	public:
		Mesh();
		~Mesh();

		virtual void Update() override;
		//virtual void LateUpdate() override;

	private:

	};
}