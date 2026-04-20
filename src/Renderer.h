#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define ASSERT(x) if(!(x)) __debugbreak();

#define GLCall(x) GlClearError();\
    x;\
    ASSERT(GlLogCall(#x, __FILE__, __LINE__));


void GlClearError();

bool GlLogCall(const char* function, const char* file, int line);
