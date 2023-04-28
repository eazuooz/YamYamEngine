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

			void SetNodeName(const std::string& name) { mName = name; }
			void SetData(DWORD_PTR data) { mData = data; }
			void SetFrame(bool frame) { mbFrame = frame; }

			void AddChild(Node* node)
			{
				node->mParent = this;
				mChilds.push_back(node);
			}

			const std::vector<Node*>& GetChilds() { return mChilds; }

			Node* mParent;
			std::vector<Node*> mChilds;
			std::string mName;
			DWORD_PTR mData;
			TreeWidget* mTreeWidget;

			bool mbFrame;
			bool mbSelected;
		};

		TreeWidget();
		~TreeWidget();

		virtual void FixedUpdate() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Close() override;

		Node* AddNode(Node* parent, const std::string& name, DWORD_PTR data, bool isFrame = false);
		void Clear();
		void SetDummyRoot(bool enable) { mbDummyRootUse = enable; }
		void SetSelectedNode(Node* node);

		void AddDynamic_Selected(Widget* widget, std::function<void(std::string key)> func)
		{
			mEventWidget = widget;
			mEvent = func;
		}


	private:
		Node* mRoot;
		
		//
		bool mbDummyRootUse;
		Node* mSelectedNode;

		Widget* mEventWidget;
		std::function<void(std::string key)> mEvent;
	};
}
