#include "guiTreeWidget.h"

namespace gui
{
	// Node
	void TreeWidget::Node::Update()
	{
		//ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_Framed;

		int flag = 0;
		if (mbFrame)
			flag |= ImGuiTreeNodeFlags_Framed;
		if (mbSelected)
			flag |= ImGuiTreeNodeFlags_Selected;
		if (mChilds.empty())
			flag |= ImGuiTreeNodeFlags_Leaf;

		if (mbFrame && mChilds.empty())
			mName = "\t" + mName;

		if (ImGui::TreeNodeEx(mName.c_str(), flag))
		{
			if (!mbFrame && ImGui::IsItemHovered(0) && ImGui::IsMouseClicked(0))
			{
				mTreeWidget->SetSelectedNode(this);
			}

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

		//mRoot->Update();

		if (!mbDummyRootUse)
		{
			mRoot->Update();
		}
		else
		{
			const std::vector<Node*>& childs = mRoot->GetChilds();
			for (Node* child : childs)
			{
				child->Update();
			}
		}
	}

	void TreeWidget::LateUpdate()
	{
	}

	void TreeWidget::Close()
	{
	}

	TreeWidget::Node* TreeWidget::AddNode(Node* parent, const std::string& _strName, DWORD_PTR _data, bool _IsFrame)
	{
		Node* node = new Node;
		node->SetNodeName(_strName);
		node->SetData(_data);
		node->SetFrame(_IsFrame);
		node->mTreeWidget = this;

		if (nullptr == parent)
			mRoot = node;
		else
			parent->AddChild(node);

		return node;
	}

	void TreeWidget::Clear()
	{
		if (nullptr != mRoot)
		{
			delete mRoot;
			mRoot = nullptr;
		}
	}
	void TreeWidget::SetSelectedNode(Node* node)
	{
		if (nullptr != mSelectedNode)
		{
			mSelectedNode->mbSelected = false;
		}

		mSelectedNode = node;
		mSelectedNode->mbSelected = true;

		if (mSelectedNode && mEvent)
		{

		}
	}
}
