#include "Renderer.h"
#include<iostream>


void GlClearError() { while (glGetError() != GL_NO_ERROR); }

bool GlLogCall(const char* function, const char* file, int line)
{
    bool noError = true;
    while (GLenum error = glGetError())
    {
        std::cout << "OpenGl error: (" << error << "): " << function
            << " " << file << ": " << line << std::endl;
        noError = false;
    }
    return noError;
}