#pragma once
#include "glfw3.h"
#include <iostream>

namespace Time {

	inline float deltaTime = 0.0f;	// Time between current frame and last frame
	inline float lastFrame = 0.0f; // Time of last frame

	inline void CalculateDeltaTime()
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
	}

	inline void ShowFPS()
	{
		float fps = 1 / deltaTime;

		std::cout << "FPS: " << fps << std::endl;
	}
}

