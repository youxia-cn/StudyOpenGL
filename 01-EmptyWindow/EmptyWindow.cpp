#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

const int SCR_WIDTH = 2000;
const int SCR_HEIGHT = 1500;

int main(int argc, char** argv){
    glfwInit();
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "StudyOpenGL", nullptr, nullptr);
    if (window == NULL)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if(glewInit() != GLEW_OK){
        std::cerr << "Failed to initalize GLEW" << std::endl;
        return -1;
    }

    while (!glfwWindowShouldClose(window))
    {
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}