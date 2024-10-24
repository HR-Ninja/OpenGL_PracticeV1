#pragma once
#include <cstdint>
#include <iostream>
#include "glad.h"
#include "glfw3.h"


class Window 
{
	typedef uint32_t uint32;
	typedef uint64_t uint64;
	typedef unsigned char uchar;

public:

	GLFWwindow* window = nullptr;

	uint32 m_width = 0;
	uint32 m_height = 0;

	Window(const Window& copy) = delete;
	void operator=(const Window&) = delete;

	static Window& Get();

	void Init(uint32 width, uint32 height, const char* title);

private:
	Window() = default;
	~Window();
};

