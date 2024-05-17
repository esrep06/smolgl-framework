/*
#pragma once 

#include <iostream>

#include "../../../include/glew/include/GL/glew.h"
#include "../../../include/glfw/include/GLFW/glfw3.h"

#include "../window/window.hpp"
#include "../utils/time/time.hpp"
#include "../input/input.hpp"
#include "../shader/shader.hpp"
#include "../ecs/ecs.hpp"

class Engine
{
    public:
        Engine(void (*appStart)(void), 
               void (*appUpdate)(void), 
               void (*appEnd)(void));

        void Start();
        void Update();
        void End(); 

        void SetWindow(Window win);
        
        int8_t GetRunningState();

        Window GetWindow();
        Time time;
    private:
        Window mWindow;
        int8_t mIsRunning;

        // Function for the app to do all the start up stuff
        void (*mAppStartFunc)(void);
        // Update function, runs every frame
        void (*mAppUpdateFunc)(void);
        // Generally a cleanup function for our app 
        void (*mAppEndFunc)(void);
};
*/
