#pragma once

namespace core::ui
{
	class UILayout
	{
	public:
		virtual ~UILayout() = default;

		virtual void update() = 0;
		virtual void setLayoutSize(float width, float height) = 0;
	};
}