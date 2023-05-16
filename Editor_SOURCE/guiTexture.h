#pragma once
#include "guiResource.h"
#include "yaResource.h"

namespace gui
{
	class Texture : public Resource
	{
	public:
		Texture();
		~Texture();

		virtual void Update() override;
		virtual void LateUpdate() override;

	private:

	};
}


