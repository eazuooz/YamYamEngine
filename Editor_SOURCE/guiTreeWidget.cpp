#include "guiTreeWidget.h"

namespace gui
{
	// Node
	void TreeWidget::Node::Update()
	{
		if (ImGui::TreeNode(mName.c_str()))
		{
			for (Node* node : mChilds)
				node->Update();
			
			ImGui::TreePop();
		}
	}

	// Tree
	TreeWidget::TreeWidget()
		: mRoot(nullptr)
	{

	}

	TreeWidget::~TreeWidget()
	{

	}

	void TreeWidget::FixedUpdate()
	{
	}

	void TreeWidget::Update()
	{
		if (mRoot == nullptr)
			return;

		mRoot->Update();
	}

	void TreeWidget::LateUpdate()
	{
	}

	void TreeWidget::Close()
	{
	}
}
