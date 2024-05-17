/*
#include "engine.hpp"

Engine::Engine(void (*appStart)(void), void (*appUpdate)(void), void (*appEnd)(void))
    : mAppStartFunc(appStart), mAppUpdateFunc(appUpdate), mAppEndFunc(appEnd)
{}

Window Engine::GetWindow()
{ return mWindow; }

void Engine::SetWindow(Window win)
{ mWindow = win; }

void Engine::Start()
{
    mIsRunning = 1;
    mWindow.Create();

    if (mAppStartFunc != NULL) mAppStartFunc();
}

void Engine::Update()
{
    GLFWwindow* glfwWindow = mWindow.GetGLFW();

    while (!glfwWindowShouldClose(glfwWindow))
    {
        time.CalculateUpdate();

        glfwPollEvents();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(RGBA_COLOR_UNPACK(mWindow.GetColor().Normalize()));

        if (mAppUpdateFunc != NULL) mAppUpdateFunc();

        if (Input::GetKeyDown(GLFW_KEY_ESCAPE)) { End(); break; }

        // End frame funcs
        Input::EndFrame();
        glfwSwapBuffers(glfwWindow);
    }
}

void Engine::End()
{
    if (!mIsRunning) return; 

    if (mAppEndFunc != NULL) mAppEndFunc();

    mIsRunning = 0;

    mWindow.Close();
}

int8_t Engine::GetRunningState()
{ return mIsRunning; }
*/
