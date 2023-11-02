#include "application.hpp"

void Application::Run()
{
    this->OnInit();

    while (!glfwWindowShouldClose(_window))
    {
        // Update screen dimensions
        int width, height;
        glfwGetFramebufferSize(_window, &width, &height);
        screen.width = width;
        screen.height = height;
        
        // Render stuff
        this->OnRender(0.0f);
        _renderer->Render();

        // Poll events
        glfwPollEvents();
    }
}

void framebufferResizeCallback(GLFWwindow *window, int width, int height)
{
    auto app = reinterpret_cast<Application *>(glfwGetWindowUserPointer(window));
    if (app != nullptr)
    {
        app->GetRenderer()->FrameBufferResized();
    }
}

void cursorPositionCallback(GLFWwindow *window, double x, double y)
{
    auto app = reinterpret_cast<Application *>(glfwGetWindowUserPointer(window));
    if (app != nullptr)
    {
        // app->GetRenderer()->pane->position = glm::vec3((float)x, (float)y, 0.0);
    }
}