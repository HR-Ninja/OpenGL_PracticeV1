#pragma once
#include "glfw3.h"

namespace Util {

	static float deltaTime = 0.0f;	// Time between current frame and last frame
	static float lastFrame = 0.0f; // Time of last frame

	static void CalculateDeltaTime()
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
	}
}
