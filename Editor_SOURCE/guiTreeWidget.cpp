#include "guiTreeWidget.h"
#include "guiEditor.h"
#include "guiInspector.h"

extern gui::Editor editor;

namespace gui
{
	TreeWidget::Node::Node()
		: mTreeWidget(nullptr)
		, mData(nullptr)
		, mParent(nullptr)
		, mbSelected(false)
		, mbStem(false)
	{
	}
	TreeWidget::Node::~Node()
	{
		for (Node* child : mChilds)
		{
			delete child;
			child = nullptr;
		}
	}
	// Node
	void TreeWidget::Node::Update()
	{
		int flag = ImGuiTreeNodeFlags_None;
		if (mbStem)
			flag |= ImGuiTreeNodeFlags_Framed;
		if (mbSelected)
			flag |= ImGuiTreeNodeFlags_Selected;
		if (mChilds.empty())
			flag |= ImGuiTreeNodeFlags_Leaf;

		if (mbStem && mChilds.empty())
			SetName("\t" + GetName());

		if (ImGui::TreeNodeEx(GetName().c_str(), flag))
		{
			if (!mbStem && ImGui::IsItemHovered(0) && ImGui::IsMouseClicked(0))
			{
				mTreeWidget->SelectNode(this);
				mTreeWidget->mEvent(mData);
			}

			for (Node* node : mChilds)
				node->Update();
			
			ImGui::TreePop();
		}
	}

	void TreeWidget::Node::AddChildNode(Node* node)
	{
		node->mParent = this;
		mChilds.push_back(node);
	}

	// Tree
	TreeWidget::TreeWidget()
		: mRoot(nullptr)
	{

	}

	TreeWidget::~TreeWidget()
	{
		delete mRoot;
		mRoot = nullptr;
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

	TreeWidget::Node* TreeWidget::AddNode(Node* parent, const std::string& name
										, void* data, bool isFrame)
	{
		Node* node = new Node;
		node->SetName(name);
		node->SetData(data);
		node->SetStem(isFrame);
		node->mTreeWidget = this;

		if (nullptr == parent)
			mRoot = node;
		else
			parent->AddChildNode(node);

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

	void TreeWidget::SelectNode(Node* node)
	{
		if (nullptr != mSelected)
			mSelected->mbSelected = false;

		mSelected = node;
		mSelected->mbSelected = true;
	}
}
