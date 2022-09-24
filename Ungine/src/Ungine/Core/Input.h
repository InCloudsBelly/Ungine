#pragma once
#include "KeyCodes.h"

namespace U {

	class Input
	{
	public:
		static bool IsKeyPressed(KeyCode keycode);

		static bool IsMouseButtonPressed(int button);
		static float GetMouseX();
		static float GetMouseY();

		static std::pair<float, float> GetMousePosition();

	};

}