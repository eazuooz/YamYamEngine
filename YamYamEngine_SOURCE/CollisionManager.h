#pragma once
#include "YamYamEngine.h"
#include "yaCollider.h"

namespace ya
{
	using namespace ya::enums;
	union ColliderID
	{
		struct 
		{
			UINT left;
			UINT right;
		};
		UINT64 id;
	};

	class CollisionManager
	{
	public:
		static void Initialize();
		static void Update();
		static void Render();
		static void Release();
		
		static void Clear();
		static void CollisionLayerCheck(UINT left, UINT right, bool value);
		static bool GetCollisionID(Collider* left, Collider* right);
		static void SetCollisionID(Collider* left, Collider* right, bool enable);
		static bool Intersect(Collider* left, Collider* right);
		static void LayerCollision(class Scene* scene, eLayerType leftLayer, eLayerType rightLayer);
		static void ColliderCollision(Collider* left, Collider* right);

		static bool RectToRect(Collider* left, Collider* right);

		static void CollisionEnter(Collider* left, Collider* right);
		static void CollisionStay(Collider* left, Collider* right);
		static void CollisionExit(Collider* left, Collider* right);

	private:
		static std::bitset<(UINT)eLayerType::Max> mLayerCollisionMatrix[(UINT)eLayerType::Max];
		static std::map<UINT64, bool> mCollisionMap;
	};
}
