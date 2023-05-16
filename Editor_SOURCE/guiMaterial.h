#pragma once
#include "guiResource.h"
#include "yaResource.h"

namespace gui
{
	class Material : public Resource
	{
	public:
		Material();
		~Material();

		virtual void Update() override;
		//virtual void LateUpdate() override;

	private:

	};
}