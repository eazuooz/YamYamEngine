#pragma once
#include "yaComponent.h"
#include "yaGraphicsDevice_DX11.h"


namespace ya
{
	using namespace graphics;
	class Transform : public Component
	{
	public:
		Transform();
		~Transform();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render() override;

		void Translate(Vector3 translation);
		void BindConstantBuffer();
		
		Transform* GetParent() { return mParent; }
		void SetParent(Transform* parent) { mParent = parent; }

		Matrix& GetWorldMatrix() { return mWorld; }
		Vector3 GetPosition() { return mPosition; }
		void SetPosition(Vector3 position) { mPosition = position; }
		void SetPosition(float x, float y, float z) { mPosition = Vector3(x,y,z); }
		Vector3 GetRotation() { return mRotation; }
		void SetRotation(Vector3 rotation) { mRotation = rotation; }
		void SetRotation(float x, float y, float z) { mRotation = Vector3(x, y, z); }
		Vector3 GetScale() { return mScale; }
		void SetScale(Vector3 scale) { mScale = scale; }
		void SetScale(float x, float y, float z) { mScale = Vector3(x, y, z); }

		const Vector3 Foward() { return mFoward; }
		const Vector3 Up() { return mUp; }
		const Vector3 Right() { return mRight; }

	private:
		Transform* mParent;
		Vector3 mUp;
		Vector3 mFoward;
		Vector3 mRight;
		Vector3 mPosition;
		Vector3 mRotation;
		Vector3 mScale;
		Matrix mWorld;
	};
}
