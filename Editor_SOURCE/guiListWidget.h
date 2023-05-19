#pragma once
#include "guiWidget.h"


namespace gui
{
	class ListWidget : public Widget
	{
	public:
		ListWidget();
		~ListWidget();
		
		virtual void Update() override;
		virtual void LateUpdate() override;
		void Close();

		void SetEvent(Widget* widget, std::function<void(std::string key)> event)
		{
			mEventWidget = widget;
			mEvent = std::move(event);
		}

		void AddResource(const std::vector<std::string>& resource);
		void AddResource(const std::vector<std::wstring>& resource);

	private:
		std::vector<std::string> mResourceList;
		int mListIndex;

		Widget* mEventWidget;
		std::function<void(std::string key)> mEvent;
	};
}
