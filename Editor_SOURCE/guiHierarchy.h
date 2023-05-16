#pragma once
#include "guiWidget.h"
#include "yaGameObject.h"
#include "guiTreeWidget.h"
#include "guiComponent.h"

namespace gui
{
	class Hierarchy : public Widget
	{
	public:
		Hierarchy();
		~Hierarchy();

		virtual void Update() override;
		virtual void LateUpdate() override;

		void InitializeInspector(void* data);
		void InitializeScene();
		void AddGameObject(TreeWidget::Node* parent, ya::GameObject* gameObject);
		
	private:
		void toInspector(void* data);

	private:
		TreeWidget* mTreeWidget;
	};
}