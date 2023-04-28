#pragma once
#include "guiWidget.h"


namespace gui
{
	class TreeWidget : public Widget
	{
	public:
		struct Node
		{
			void Update();

			Node* mParent;
			std::vector<Node*> mChilds;
			std::string mName;
		};

		TreeWidget();
		~TreeWidget();

		virtual void FixedUpdate() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Close() override;

	private:
		Node* mRoot;
	};
}
