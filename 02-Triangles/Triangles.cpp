#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "../include/shader.hpp"

const int SCR_WIDTH = 2000;
const int SCR_HEIGHT = 1500;
GLuint VBO, VAO, EBO;
const GLuint NumOfVertices = 4;

void init(){
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    GLfloat vertices[NumOfVertices][2] = {
         { 0.5f, 0.5f }, 
         { 0.5f, -0.5f }, 
         { -0.5f, -0.5f }, 
         { -0.5f, 0.5f }
    };

    glCreateBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glNamedBufferStorage(VBO, sizeof(vertices), vertices, 0);

    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,  // first Triangle
        1, 2, 3   // second Triangle
    };
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    ShaderInfo shaders[] = {
        {GL_VERTEX_SHADER, "triangles.vert", 0},
        {GL_FRAGMENT_SHADER, "triangles.frag", 0},
        {GL_NONE, "", 0}
    };

    Shader shader_default(shaders);
    shader_default.setCurrent();

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);
}

void display(){
    static const GLfloat clearColor[] = {0.2f, 0.3f, 0.3f, 1.0f};
    glClearBufferfv(GL_COLOR, 0, clearColor);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void onResize(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void onKey(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS)
    {
        switch (key)
        {
            case GLFW_KEY_M:
                {
                    static GLenum  mode = GL_FILL;

                    mode = ( mode == GL_FILL ? GL_LINE : GL_FILL );
                    glPolygonMode( GL_FRONT_AND_BACK, mode );
                }
                return;
        }
    }
}

int main(int argc, char** argv){
    glfwInit();
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "StudyOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, onKey);
    glfwSetWindowSizeCallback(window, onResize);

    if(glewInit() != GLEW_OK){
        std::cerr << "Failed to initalize GLEW" << std::endl;
        return -1;
    }

    init(); //Init主要是用来创建VAO、VBO等，并准备要各种数据

    while (!glfwWindowShouldClose(window))
    {
        display(); //这里才是渲染图形的主战场
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwDestroyWindow(window);

    glfwTerminate();
    return 0;
}

