#include <glad/glad.h>  ¡
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>


struct ShaderProgramSource
{
    std::string VertexSource;
    std::string FragmentSource;
};

static ShaderProgramSource ParseSHader(const std::string& filePath)
{
    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::fstream stream(filePath);
    std::string line;
    ShaderType type = ShaderType::NONE;
    std::stringstream ss[2];
    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
            {
                //set mode to vertex
                type = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos)
            {
                //set mode to fragment
                type = ShaderType::FRAGMENT;
            }
        }
        else 
        {
            ss[(int)type] << line << '\n';
        }
    }
    return { ss[0].str(), ss[1].str() };
}

static unsigned int CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();

    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (!result)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length*sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile"<< (type==GL_VERTEX_SHADER ? "vertex" : "fragment") << "shader: " << message << std::endl;

        glDeleteShader(id);
        return 0;
    }
    return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    GLuint program = glCreateProgram();
    GLuint vs = CompileShader(GL_VERTEX_SHADER,vertexShader);
    GLuint fs = CompileShader(GL_FRAGMENT_SHADER,fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;

}



int main() 
{
    if (!glfwInit()) return -1;

    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL CMake", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);//<--Vsync (prevents 5.000fps into poor monitors)
    // Cargar GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Falló al inicializar GLAD" << std::endl;
        return -1;
    }

    float positions[] =
    {
        -0.5f, -0.5f,
        0.0f , 0.5f,
        0.5f , -0.5f
    };

    unsigned int buffer;
    glGenBuffers(1,&buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, 6*sizeof(float),positions,GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

    ShaderProgramSource shaders = ParseSHader("shaders/Basic.shader");

    unsigned int shader = CreateShader(shaders.VertexSource,shaders.FragmentSource);
    glUseProgram(shader);



    double lastTime = glfwGetTime();
    int nbFrames = 0;
    while (!glfwWindowShouldClose(window)) 
    {
        //#################
        //       FPS
        double currentTime = glfwGetTime();
        nbFrames++;
        // Si ha pasado más de 1 segundo, calculamos y reiniciamos
        if (currentTime - lastTime >= 1.0) {
            double msPerFrame = 1000.0 / double(nbFrames);
            // Creamos el string del título
            // Nota: El título original era "OpenGL con CMake"
            std::string newTitle = "OpenGL CMake - FPS: " + std::to_string(nbFrames) +
                                   " (" + std::to_string(msPerFrame).substr(0, 4) + " ms)";
            // Cambiamos el título de la ventana
            glfwSetWindowTitle(window, newTitle.c_str());
            nbFrames = 0;lastTime += 1.0;
        }
        //##################
        glClear(GL_COLOR_BUFFER_BIT);
        glDrawArrays(GL_TRIANGLES,0,3);
       


        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}