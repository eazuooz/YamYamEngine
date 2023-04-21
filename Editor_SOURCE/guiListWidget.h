#pragma once
#include "guiWidget.h"


namespace gui
{
	class ListWidget : public Widget
	{
	public:
		ListWidget();
		~ListWidget();
		
		virtual void LateUpdate() override;
		void Close();


		void SetEvent(Widget* widget, std::function<void(std::string key)> event)
		{
			mEventWidget = widget;
			mEvent = std::move(event);
		}

		void SetItemList(const std::vector<std::string>& _inList);
		void SetItemList(const std::vector<std::wstring>& _inList);

		

	private:
		std::vector<std::string> mResourceList;
		int mListIndex;

		Widget* mEventWidget;
		std::function<void(std::string key)> mEvent;
	};
}
