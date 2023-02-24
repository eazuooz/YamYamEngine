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
	bool CollisionManager::Intersect(Collider* left, Collider* right)
	{
		// 0 --- 1
		// |  \  | 
		// 3 --- 2	
		static const Vector3 arrLocalPos[4] =
		{
			Vector3(-0.5f, 0.5f, 0.f),
			Vector3(0.5f, 0.5f, 0.f),
			Vector3(0.5f, -0.5f, 0.f),
			Vector3(-0.5f, -0.5f, 0.f)
		};

		Transform* leftTr = left->GetOwner()->GetComponent<Transform>();
		Transform* rightTr = right->GetOwner()->GetComponent<Transform>();


		// 분리축 구하기
		Vector3 vAxis[4] = {};

		const Matrix& matLeft = leftTr->GetWorldMatrix();
		const Matrix& matRight = rightTr->GetWorldMatrix();

		// 분리축 벡터 == 투영벡터
		vAxis[0] = Vector3::Transform(arrLocalPos[1], matLeft) - Vector3::Transform(arrLocalPos[0], matLeft);
		vAxis[1] = Vector3::Transform(arrLocalPos[3], matLeft) - Vector3::Transform(arrLocalPos[0], matLeft);
		vAxis[2] = Vector3::Transform(arrLocalPos[1], matRight) - Vector3::Transform(arrLocalPos[0], matRight);
		vAxis[3] = Vector3::Transform(arrLocalPos[3], matRight) - Vector3::Transform(arrLocalPos[0], matRight);

		for (int i = 0; i < 4; ++i)
			vAxis[i].z = 0.f;


		Vector2 vC = Vector2(left->GetColliderPos().x - right->GetColliderPos().x
							, left->GetColliderPos().y - right->GetColliderPos().y);
		Vector3 vCenterDir = Vector3(vC.x, vC.y, 0.f);


		for (int i = 0; i < 4; ++i)
		{
			Vector3 vA = vAxis[i];
			vA.Normalize();

			float fProjDist = 0.f;
			for (int j = 0; j < 4; ++j)
			{
				fProjDist += fabsf(vAxis[j].Dot(vA)) / 2.f;
			}

			if (fProjDist < fabsf(vCenterDir.Dot(vA)))
			{
				return false;
			}
		}

		return true;
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

			if (left->GetComponent<Collider>() == nullptr)
				continue;

			for (GameObject* right : rights)
			{
				GameObject::eState state = right->GetActive();
				if (state == GameObject::eState::Dead
					|| state == GameObject::eState::Paused)
					continue;

				if (right->GetComponent<Collider>() == nullptr)
					continue;
				if (right == left)
					continue;

				ColliderCollision(left->GetComponent<Collider>(), right->GetComponent<Collider>());

			}
			
			// 넘어가면 break 최적화
			/*if (leftLayer == rightLayer)
				break;*/
		}
	}

	void CollisionManager::ColliderCollision(Collider* left, Collider* right)
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

		if (Intersect(left, right))
		{
			// 충돌 중
			if (iter->second == false)
			{
				//최초 충돌했을때
				if (left->IsTrigger())
					left->OnTriigerEnter(right);
				else
					left->OnCollisionEnter(right);

				if (right->IsTrigger())
					right->OnTriigerEnter(left);
				else
					right->OnCollisionEnter(left);
				
				iter->second = true;
			}
			else
			{
				if (left->IsTrigger())
					left->OnTriigerStay(right);
				else
					left->OnCollisionStay(right);

				if (right->IsTrigger())
					right->OnTriigerStay(left);
				else
					right->OnCollisionStay(left);
				//충돌 중일때
			}
		}
		else
		{
			// 충돌 X
			if (iter->second)
			{
				if (left->IsTrigger())
					left->OnTriigerExit(right);
				else
					left->OnCollisionExit(right);

				if (right->IsTrigger())
					right->OnTriigerExit(left);
				else
					right->OnCollisionExit(left);

				iter->second = false;
			}

		}
	}
}
