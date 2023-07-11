#include "yaCollider2D.h"
#include "yaGameObject.h"
#include "yaRenderer.h"

namespace ya
{
	Collider2D::Collider2D()
	{
	}

	Collider2D::~Collider2D()
	{
	}

	void Collider2D::Initialize()
	{
		mTransform = GetOwner()->GetComponent<Transform>();

	}

	void Collider2D::Update()
	{
	}

	void Collider2D::LateUpdate()
	{
		Vector3 scale = mTransform->GetScale();
		scale *= Vector3(mSize.x, mSize.y, 1.0f);

		Vector3 rotation = mTransform->GetRotation();
		
		Vector3 position = mTransform->GetPosition();
		Vector3 colliderPos = position + Vector3(mCenter.x, mCenter.y, 0.0f);
		mColliderPosition = colliderPos;

		Matrix scaleMatrix;
		scaleMatrix = Matrix::CreateScale(scale);

		Matrix rotationMatrix;
		rotationMatrix = Matrix::CreateRotationX(rotation.x);
		rotationMatrix *= Matrix::CreateRotationY(rotation.y);
		rotationMatrix *= Matrix::CreateRotationZ(rotation.z);

		Matrix positionMatrix;
		positionMatrix.Translation(Vector3(colliderPos.x , colliderPos.y, 1.0f));

		Matrix worldMatrix = scaleMatrix * rotationMatrix * positionMatrix;

		DebugMesh debugMesh;
		debugMesh.position = Vector3(colliderPos.x, colliderPos.y, colliderPos.z);
		debugMesh.scale = scale;
		debugMesh.radius = 1.0f;
		debugMesh.rotation = rotation;
		debugMesh.type = eColliderType::Rect;

		renderer::PushDebugMesh(debugMesh);
	}

	void Collider2D::Render()
	{
		
	}
	void Collider2D::OnCollisionEnter(Collider* collider)
	{
		Collider::OnCollisionEnter(collider);
	}
	void Collider2D::OnCollisionStay(Collider* collider)
	{
		Collider::OnCollisionStay(collider);
	}
	void Collider2D::OnCollisionExit(Collider* collider)
	{
		Collider::OnCollisionExit(collider);
	}
	void Collider2D::OnTriigerEnter(Collider* collider)
	{
		Collider::OnTriigerEnter(collider);
	}
	void Collider2D::OnTriigerStay(Collider* collider)
	{
		Collider::OnTriigerStay(collider);
	}
	void Collider2D::OnTriigerExit(Collider* collider)
	{
		Collider::OnTriigerExit(collider);
	}
}
