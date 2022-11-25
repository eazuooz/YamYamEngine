#pragma once
#include "CommonInclude.h"

#include <memory>

namespace ya::graphics
{

	struct GraphicsDeviceChild
	{
		std::shared_ptr<void> internal_state;
		inline bool IsValid() const { return internal_state.get() != nullptr; }
	};


}
