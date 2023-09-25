#include "yaLight.h"
#include "yaGameObject.h"
#include "yaTransform.h"
#include "yaRenderer.h"

namespace ya
{
	Light::Light()
		: Component(eComponentType::Light)
		, mAttribute{}
	{

	}

	Light::~Light()
	{
	}

	void Light::LateUpdate()
	{
		Transform* tr = GetOwner()->GetComponent<Transform>();
		Vector3 position = tr->getPosition();
		mAttribute.position = Vector4(position.x, position.y, position.z, 1.0f);
		mAttribute.direction = Vector4(tr->Foward().x, tr->Foward().y, tr->Foward().z, 0.0f);

		renderer::lights.push_back(mAttribute);
	}
}
