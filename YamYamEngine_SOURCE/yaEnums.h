#pragma once

namespace ya::enums
{
	enum class eLayer
	{
		None = 0,

		Max = 16,
	};

	enum class eComponentType
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

		//etc
		Script,
		End,
	};

	enum class eResourceType 
	{
		Mesh,
		Texture,
		Material,
		Sound,
		Prefab,
		MeshData,
		GraphicsShader,
		ComputeShader,
		End,
	};
}
