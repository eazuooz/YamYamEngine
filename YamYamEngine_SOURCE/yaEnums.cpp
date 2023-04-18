#include "yaEnums.h"


namespace ya::enums
{
	const char* charComponentType[(int)eComponentType::End] =
	{
		//component
		"Component",
		"Transform",
		"Camera",
		"Collider",
		"Animator",
		"Light",

		//render
		"MeshRenderer",
		"SpriteRenderer",
		"ParticleSystem",
		"SkyBox",
		"Decal",

		//etc
		"Script",
	};

	const wchar_t* wcharComponentType[(int)eComponentType::End] =
	{
		//component
		L"Component",
		L"Transform",
		L"Camera",
		L"Collider",
		L"Animator",
		L"Light",

		//render
		L"MeshRenderer",
		L"SpriteRenderer",
		L"ParticleSystem",
		L"SkyBox",
		L"Decal",

		//etc
		L"Script",
	};
}