#include "Window.hpp"

Window::~Window()
{
    glfwDestroyWindow(window);
    // glfw: terminate, clearing all previously allocated GLFW resources.
    glfwTerminate();
}

Window& Window::Get()
{
    static Window instance;
    return instance;
}

void Window::Init(uint32 width, uint32 height, const char* title)
{
    m_width = width;
    m_height = height;

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width, height, title, NULL, NULL);

    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(-1);
    }

    glfwMakeContextCurrent(window);
}
