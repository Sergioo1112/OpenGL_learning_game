#include <glad/glad.h>  // ¡Importante! Va primero
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
int main() {
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

    double lastTime = glfwGetTime();
    int nbFrames = 0;

    while (!glfwWindowShouldClose(window)) 
    {
        // Calcular FPS
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

            nbFrames = 0;
            lastTime += 1.0;
        }

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // Un color azul grisáceo
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}