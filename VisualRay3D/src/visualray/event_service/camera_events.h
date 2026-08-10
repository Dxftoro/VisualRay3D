#pragma once

#include "event.h"
#include "../kernel.h"
#include "../world/components.h"

namespace vray {

	class VRAYLIB CameraChangedEvent : public Event {
	private:
		CompCamera* camera;

	public:
		CameraChangedEvent(CompCamera* _camera) : camera(_camera) {}

		void dump() const override {
			VR_LOGINFO("Event dump: new camera");
		}

		CompCamera* getNewCamera() const { return camera; }

		VR_DEFINE_EVENT_TYPE(events::CAMERA_CHANGED)
		VR_DEFINE_EVENT_CATEGORY(EventCategory::GAME_EVENT)
	};

}