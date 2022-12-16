#include "yaGameObject.h"


namespace ya
{
	void GameObject::AddComponent(Component* component)
	{
		//// Find the insertion point in the sorted vector
		//// (The first element with a order higher than me)
		//int myOrder = component->GetUpdateOrder();
		//auto iter = mComponents.begin();
		//for ( ;iter != mComponents.end(); ++iter)
		//{
		//	if (myOrder < (*iter)->GetUpdateOrder())
		//	{
		//		break;
		//	}
		//}

		//// Inserts element before position of iterator
		//mComponents.insert(iter, component);
	}
}
