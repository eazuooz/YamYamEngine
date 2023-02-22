#include "CollisionManager.h"
#include "yaScene.h"
#include "yaSceneManager.h"

namespace ya
{
	std::bitset<(UINT)eLayer::Max> CollisionManager::mLayerCollisionMatrix[(UINT)eLayer::Max] = {};
	std::map<UINT64, bool> CollisionManager::mCollisionMap;

	void CollisionManager::Initialize()
	{
	}

	void CollisionManager::Update()
	{
		Scene* scene = SceneManager::GetActiveScene();
		for (UINT row = 0; row < (UINT)eLayer::Max; row++)
		{
			for (UINT col = row; col < (UINT)eLayer::Max; col++)
			{
				if (mLayerCollisionMatrix[row][col])
				{
					LayerCollision(scene, (eLayer)row, (eLayer)col);
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


		// �и��� ���ϱ�
		Vector3 vAxis[4] = {};

		const Matrix& matLeft = leftTr->GetWorldMatrix();
		const Matrix& matRight = rightTr->GetWorldMatrix();

		// �и��� ���� == ��������
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
	void CollisionManager::LayerCollision(Scene* scene, eLayer leftLayer, eLayer rightLayer)
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
			
			// �Ѿ�� break ����ȭ
			/*if (leftLayer == rightLayer)
				break;*/
		}
	}

	void CollisionManager::ColliderCollision(Collider* left, Collider* right)
	{
		// �� �浹ü��  ID Ȯ��
		ColliderID colliderID;
		colliderID.left = left->GetID();
		colliderID.right = right->GetID();

		// ���� �浹������ �˻��Ѵ�.
		std::map<UINT64, bool>::iterator iter
			= mCollisionMap.find(colliderID.id);

		// �浹������ ���ٸ� �浹������ ������ش�.
		if (iter == mCollisionMap.end())
		{
			mCollisionMap.insert(std::make_pair(colliderID.id, false));
			iter = mCollisionMap.find(colliderID.id);
		}

		if (Intersect(left, right))
		{
			// �浹 ��
			if (iter->second == false)
			{
				//���� �浹������
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
				//�浹 ���϶�
			}
		}
		else
		{
			// �浹 X
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
