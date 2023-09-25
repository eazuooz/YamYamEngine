#include "CollisionManager.h"
#include "yaScene.h"
#include "yaSceneManager.h"

namespace ya
{
	std::bitset<(UINT)eLayerType::Max> CollisionManager::mLayerCollisionMatrix[(UINT)eLayerType::Max] = {};
	std::map<UINT64, bool> CollisionManager::mCollisionMap;

	void CollisionManager::Initialize()
	{
	}

	void CollisionManager::Update()
	{
		Scene* scene = SceneManager::GetActiveScene();
		for (UINT row = 0; row < (UINT)eLayerType::Max; row++)
		{
			for (UINT col = row; col < (UINT)eLayerType::Max; col++)
			{
				if (mLayerCollisionMatrix[row][col])
				{
					LayerCollision(scene, (eLayerType)row, (eLayerType)col);
				}
			}
		}
	}

	void CollisionManager::Render()
	{
	}

	void CollisionManager::Release()
	{
	}

	void CollisionManager::Clear()
	{
		mLayerCollisionMatrix->reset();
		mCollisionMap.clear();
	}

	void CollisionManager::CollisionLayerCheck(UINT left, UINT right, bool value)
	{
		int row = 0;
		int column = 0;
		if (left <= right)
		{
			row = left;
			column = right;
		}
		else
		{
			row = right;
			column = left;
		}

		mLayerCollisionMatrix[row][column] = value;
	}

	bool CollisionManager::GetCollisionID(Collider* left, Collider* right)
	{
		// 두 충돌체의  ID 확인
		ColliderID colliderID;
		colliderID.left = left->GetID();
		colliderID.right = right->GetID();

		// 이전 충돌정보를 검색한다.
		std::map<UINT64, bool>::iterator iter
			= mCollisionMap.find(colliderID.id);

		// 충돌정보가 없다면 충돌정보를 만들어준다.
		if (iter == mCollisionMap.end())
		{
			mCollisionMap.insert(std::make_pair(colliderID.id, false));
			iter = mCollisionMap.find(colliderID.id);
		}

		return iter->second;
	}

	void CollisionManager::SetCollisionID(Collider* left, Collider* right, bool enable)
	{
		// 두 충돌체의  ID 확인
		ColliderID colliderID;
		colliderID.left = left->GetID();
		colliderID.right = right->GetID();

		// 이전 충돌정보를 검색한다.
		std::map<UINT64, bool>::iterator iter
			= mCollisionMap.find(colliderID.id);

		iter->second = enable;
	}

	bool CollisionManager::Intersect(Collider* left, Collider* right)
	{
		if (left->GetType() == eColliderType::Rect 
			&& right->GetType() == eColliderType::Rect)
		{
			return RectToRect(left, right);
		}

		return false;
	}
	void CollisionManager::LayerCollision(Scene* scene, eLayerType leftLayer, eLayerType rightLayer)
	{
		const std::vector<GameObject*>& lefts = scene->GetLayer(leftLayer)->GetGameObjects();
		const std::vector<GameObject*>& rights = scene->GetLayer(rightLayer)->GetGameObjects();

		for (GameObject* left : lefts)
		{
			GameObject::eState state = left->GetActive();
			if (state == GameObject::eState::Dead
				|| state == GameObject::eState::Paused)
				continue;

			Collider* leftCollider 
				= left->GetComponent<Collider>();
			if (leftCollider == nullptr)
				continue;

			for (GameObject* right : rights)
			{
				GameObject::eState state = right->GetActive();
				if (state == GameObject::eState::Dead
					|| state == GameObject::eState::Paused)
					continue;

				Collider* rightCollider 
					= right->GetComponent<Collider>();
				if (rightCollider == nullptr)
					continue;
				if (right == left)
					continue;

				ColliderCollision(leftCollider, rightCollider);
			}
		}
	}

	void CollisionManager::ColliderCollision(Collider* left, Collider* right)
	{
		bool check = GetCollisionID(left, right);

		if (Intersect(left, right))
		{
			if (check == false)
			{
				CollisionEnter(left, right);
				SetCollisionID(left, right, true);
			}
			else if (check == true)
			{
				CollisionStay(left, right);
			}
		}
		else
		{
			if (check == true)
			{
				CollisionExit(left, right);
				SetCollisionID(left, right, false);
			}
		}
	}

	bool CollisionManager::RectToRect(Collider* left, Collider* right)
	{
		// Rect vs Rect 
			// 0 --- 1
			// |     |
			// 3 --- 2
		Vector3 arrLocalPos[4] =
		{
		   Vector3{-0.5f, 0.5f, 0.0f}
		   ,Vector3{0.5f, 0.5f, 0.0f}
		   ,Vector3{0.5f, -0.5f, 0.0f}
		   ,Vector3{-0.5f, -0.5f, 0.0f}
		};

		Transform* leftTr = left->GetOwner()->GetComponent<Transform>();
		Transform* rightTr = right->GetOwner()->GetComponent<Transform>();

		Matrix leftMat = leftTr->GetWorldMatrix();
		Matrix rightMat = rightTr->GetWorldMatrix();



		// 분리축 벡터 4개 구하기
		Vector3 Axis[4] = {};

		Vector3 leftScale = Vector3(left->GetSize().x, left->GetSize().y, 1.0f);

		Matrix finalLeft = Matrix::CreateScale(leftScale);
		finalLeft *= leftMat;

		Vector3 rightScale = Vector3(right->GetSize().x, right->GetSize().y, 1.0f);
		Matrix finalRight = Matrix::CreateScale(rightScale);
		finalRight *= rightMat;

		Axis[0] = Vector3::Transform(arrLocalPos[1], finalLeft);
		Axis[1] = Vector3::Transform(arrLocalPos[3], finalLeft);
		Axis[2] = Vector3::Transform(arrLocalPos[1], finalRight);
		Axis[3] = Vector3::Transform(arrLocalPos[3], finalRight);

		Axis[0] -= Vector3::Transform(arrLocalPos[0], finalLeft);
		Axis[1] -= Vector3::Transform(arrLocalPos[0], finalLeft);
		Axis[2] -= Vector3::Transform(arrLocalPos[0], finalRight);
		Axis[3] -= Vector3::Transform(arrLocalPos[0], finalRight);

		for (size_t i = 0; i < 4; i++)
			Axis[i].z = 0.0f;

		Vector3 vc = leftTr->getPosition() - rightTr->getPosition();
		vc.z = 0.0f;

		Vector3 centerDir = vc;
		for (size_t i = 0; i < 4; i++)
		{
			Vector3 vA = Axis[i];
			//vA.Normalize();

			float projDist = 0.0f;
			for (size_t j = 0; j < 4; j++)
			{
				projDist += fabsf(Axis[j].Dot(vA) / 2.0f);
			}

			if (projDist < fabsf(centerDir.Dot(vA)))
			{
				return false;
			}
		}
		// 숙제 Circle vs Cirlce

		return true;
	}
	void CollisionManager::CollisionEnter(Collider* left, Collider* right)
	{
		if (left->IsTrigger())
			left->OnTriigerEnter(right);
		else
			left->OnCollisionEnter(right);

		if (right->IsTrigger())
			right->OnTriigerEnter(left);
		else
			right->OnCollisionEnter(left);
	}
	void CollisionManager::CollisionStay(Collider* left, Collider* right)
	{
		if (left->IsTrigger())
			left->OnTriigerStay(right);
		else
			left->OnCollisionStay(right);

		if (right->IsTrigger())
			right->OnTriigerStay(left);
		else
			right->OnCollisionStay(left);
	}
	void CollisionManager::CollisionExit(Collider* left, Collider* right)
	{
		if (left->IsTrigger())
			left->OnTriigerExit(right);
		else
			left->OnCollisionExit(right);

		if (right->IsTrigger())
			right->OnTriigerExit(left);
		else
			right->OnCollisionExit(left);
	}
}
