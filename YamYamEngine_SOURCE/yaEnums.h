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
		Player,
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

		//Audio
		AudioListener,
		AudioSource,

		//etc
		Script,
		End,
	};

	enum class eResourceType 
	{
		Mesh,
		Texture,
		Material,
		AudioClip,
		Prefab,
		Shader,
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

	extern const char* charComponentType[(int)eComponentType::End];
	extern const wchar_t* wcharComponentType[(int)eComponentType::End];
	extern const char* charResourceType[(int)eResourceType::End];
	extern const wchar_t* wcharResourceType[(int)eResourceType::End];
}
