#include "Collider2D.h"
#include "yaGameObject.h"
#include "yaRenderer.h"

namespace ya
{
	Collider2D::Collider2D()
		: Component(eComponentType::Collider2D)
		, mCenter(Vector2::Zero)
		, mSize(Vector2::One)
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

	void Collider2D::FixedUpdate()
	{
		Vector3 scale = mTransform->GetScale();
		scale *= Vector3(mSize.x, mSize.y, 1.0f);

		Vector3 rotation = mTransform->GetRotation();
		
		Vector3 position = mTransform->GetPosition();
		Vector3 colliderPos = position + Vector3(mCenter.x, mCenter.y, 0.0f);

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
}
