#pragma once
#include "../vrpch.h"
#include "../kernel.h"
#include "event.h"

namespace vray {

	class VRAYLIB RenderStepEvent : public Event {
	public:
		RenderStepEvent() {}

		VR_DEFINE_EVENT_TYPE(EventType::RENDER_STEP)
		VR_DEFINE_EVENT_CATEGORY(EventCategory::RENDER_EVENT)
	};

}