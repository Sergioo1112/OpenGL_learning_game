#include <glad/glad.h>  
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <filesystem>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

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

    std::ifstream stream(filePath);
    //std::cout << "Directorio actual: " << std::filesystem::current_path() << std::endl;
    if (!stream.is_open())std::cout << "ERROR: No se pudo encontrar el archivo en: " << filePath << std::endl;
    
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
        char* message = (char*)_malloca(length*sizeof(char));
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

    //setting a opengl window v3.3, setting profile to core
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
    
    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL CMake", NULL, NULL);
    if (!window) 
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);//<--Vsync (prevents 5.000fps into poor monitors)

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Falló al inicializar GLAD" << std::endl;
        return -1;
    }

    std::cout << glGetString(GL_VERSION) << std::endl;

    {
        //shaders
        float positions[] = //bottom left to bottom rigth
        {
            -0.5f, -0.5f,//0
            0.5f , -0.5f,//1
            0.5f , 0.5f, //2
            -0.5f , 0.5f,//3
        };

        unsigned int indices[] = //<--index buffer
        {
            0,1,2,
            2,3,0
        };

        //Vertex Array Object: Vertex configuration atributes
        unsigned int vao;
        GLCall(glGenVertexArrays(1, &vao));
        GLCall(glBindVertexArray(vao));

        //create vertex
        VertexBuffer vb(positions, 6 * 2 * sizeof(float));

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);//specify what are we passing

        //create index
        IndexBuffer ib(indices, 6 * sizeof(unsigned int));

        //shader
        ShaderProgramSource source = ParseSHader("res/shaders/Basic.shader");
        unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
        glUseProgram(shader);

        //uniform
        GLCall(int location = glGetUniformLocation(shader, "u_color"));
        ASSERT(location != -1);
        GLCall(glUniform4f(location, 0.8f, 0.3f, 0.8f, 1.0f));//Uniform actúa como un puente entre la cpu y gpu(en este caso cambio de color)

        //fps
        double lastTime = glfwGetTime();
        int nbFrames = 0;

        //unbind everything
        GLCall(glBindVertexArray(0));
        glUseProgram(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        //color change
        float r = 0.0f;
        float increment = 0.01f;

        while (!glfwWindowShouldClose(window))
        {
            //################# fps
            double currentTime = glfwGetTime();
            nbFrames++;
            // Si ha pasado más de 1 segundo, calculamos y reiniciamos
            if (currentTime - lastTime >= 1.0)
            {
                //title string
                std::string newTitle = "OpenGL CMake - FPS: " + std::to_string(nbFrames);
                //Change title
                glfwSetWindowTitle(window, newTitle.c_str());
                nbFrames = 0; lastTime += 1.0;
            }
            //##################

            glClear(GL_COLOR_BUFFER_BIT);

            glUseProgram(shader);
            GLCall(glUniform4f(location, r, 0.3f, r, 1.0f));

            GLCall(glBindVertexArray(vao));
            ib.Bind();

            GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

            if (r > 1.0f)increment = -0.05f;
            else if (r < 0.0f)increment = 0.05f;

            r += increment;


            glfwSwapBuffers(window);
            glfwPollEvents();
        }//end while

        GLCall(glDeleteProgram(shader));
    }

    glfwTerminate();
    return 0;
}//end program
