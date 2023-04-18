#pragma once

namespace ya::enums
{
	enum class eSceneType
	{
		Tilte,
		Play,
		End,
	};

	enum class eLayerType
	{
		None = 0,

		Max = 16,
	};

	enum class eComponentType
	{
		//component
		Component = 0,
		Transform,
		Camera,
		Collider,
		Animator,
		Light,
		
		//render
		MeshRenderer,
		SpriteRenderer,
		ParticleSystem,
		SkyBox,
		Decal,

		//etc
		Script,
		End,
	};
	extern const char* charComponentType[(int)eComponentType::End];
	extern const wchar_t* wcharComponentType[(int)eComponentType::End];

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

	enum class eColliderType
	{
		Rect,
		Circle,

		Cube,
		Sphere,

		End,
	};


}
