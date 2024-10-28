#include "glad.h"
#include "glfw3.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>
#include <cmath>
#include "Shader.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Camera.hpp"
#include "Window.hpp"
#include "Shapes.hpp"
#include "Util.hpp"
#include "Settings.hpp"


typedef uint32_t uint32;
typedef uint64_t uint64;
typedef unsigned char uchar;


void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void keyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouseCallBack(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
void Draw(uint32& VAO, Shader& shader, glm::vec3 position, glm::vec3 scale);
unsigned int loadTexture(char const* path);

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = g_windowSettings.SCR_WIDTH / 2.0f;
float lastY = g_windowSettings.SCR_HEIGHT / 2.0f;
bool firstMouse = true;

glm::vec3 lightPos = glm::vec3(1.2f, 0.0f, 2.0f);

int main()
{
   Window::Get().Init(g_windowSettings.SCR_WIDTH, g_windowSettings.SCR_HEIGHT, "OpenGL Practice");

    glfwSetFramebufferSizeCallback(Window::Get().window, framebufferSizeCallback);
    glfwSetKeyCallback(Window::Get().window, keyCallBack);
    glfwSetInputMode(Window::Get().window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(Window::Get().window, mouseCallBack);
    glfwSetScrollCallback(Window::Get().window, scroll_callback);

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        exit(-1);
    }

    Shader cube("Shaders/light.vert", "Shaders/light.frag");
    Shader light("Shaders/light_cube.vert", "Shaders/light_cube.frag");

    // drawing stuff
    uint32 VBO, cubeVAO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &VBO);

    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(cubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_shapes.cubeTexDiff), g_shapes.cubeTexDiff, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
   

    uint32 lampVAO;
    glGenVertexArrays(1, &lampVAO);
    glBindVertexArray(lampVAO);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    unsigned int diffuseMap = loadTexture("Resources/container2.png");

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffuseMap);

    // uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // render loop
    while (!glfwWindowShouldClose(Window::Get().window))
    {   
        Util::CalculateDeltaTime();

        // input here - if needed
        processInput(Window::Get().window);

        // render & clear buffer
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        lightPos.x = sin(glfwGetTime()) * 2.0f;
        lightPos.z = cos(glfwGetTime()) * 2.0f;
  
        cube.use();
        cube.setVec3("light.position", lightPos);

        // lighting
        cube.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
        cube.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f); // darken diffuse light a bit
        cube.setVec3("light.specular", 1.0f, 1.0f, 1.0f);

        // material
        cube.setInt("material.diffuse", 0);
        cube.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
        cube.setFloat("material.shininess", 64.0f);



        Draw(cubeVAO, cube, glm::vec3(0.0f), glm::vec3(1.0f));



        // lamp
        Draw(lampVAO, light, lightPos, glm::vec3(0.2f));
        
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(Window::Get().window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &lampVAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(cube.ID);
    glDeleteProgram(light.ID);

    return 0;
}


void mouseCallBack(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}


void keyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, Util::deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, Util::deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, Util::deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, Util::deltaTime);
}


// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    // Avoid division by zero
    if (height == 0) height = 1;

    g_windowSettings.SCR_WIDTH = width;
    g_windowSettings.SCR_HEIGHT = height;

    // Set the viewport to cover the new window size
    glViewport(0, 0, g_windowSettings.SCR_WIDTH, g_windowSettings.SCR_HEIGHT);
}


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

void Draw(uint32& VAO, Shader& shader, glm::vec3 position, glm::vec3 scale)
{
    shader.use();

    glm::mat4 model = glm::mat4(1.0f);


    // make changes here to the model
    model = glm::translate(model, position);
    model = glm::scale(model, scale);


    shader.setMat4("model", model);

    glm::mat4 view = camera.GetViewMatrix();
    shader.setMat4("view", view);

    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)g_windowSettings.SCR_WIDTH / (float)g_windowSettings.SCR_HEIGHT, 0.1f, 100.0f);
    shader.setMat4("projection", projection);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    
}

unsigned int loadTexture(char const* path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format = GL_RGB;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}