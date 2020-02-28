#include "../include/app.hpp"
#include "../include/shader.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class MyApp : public App {
    private:
        GLuint VBO, VAO, EBO;
        Shader *shader_cube;
        const GLuint NumOfVertices = 4;
        const GLfloat clearColor[4] = {0.2f, 0.3f, 0.3f, 1.0f};

    public:
        void init(){
            glGenVertexArrays(1, &VAO);
            glBindVertexArray(VAO);

            GLfloat vertices[] = {
                -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
                0.5f, -0.5f,  -0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
                0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
                -0.5f,  0.5f,  -0.5f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f,
                -0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
                0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
                0.5f,  0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
                -0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f
            };

            glCreateBuffers(1, &VBO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glNamedBufferStorage(VBO, sizeof(vertices), vertices, 0);

            static const GLushort indices[] =
            {
                1, 0, 2, 3, 6, 7, 5, 4,         // First strip
                0xFFFF,                         // <<-- This is the restart index
                2, 6, 1, 5, 0, 4, 3, 7          // Second strip
            };
            glGenBuffers(1, &EBO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

            ShaderInfo shaders[] = {
                {GL_VERTEX_SHADER, "cube.vert"},
                {GL_FRAGMENT_SHADER, "cube.frag"},
                {GL_NONE, ""}
            };

            shader_cube = new Shader(shaders);
            shader_cube->setCurrent();

            glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (void*)0);
            glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (void*)(4*sizeof(GLfloat)));
            glEnableVertexAttribArray(0);
            glEnableVertexAttribArray(1);

            glEnable(GL_CULL_FACE);
            glDisable(GL_DEPTH_TEST);

        }

        void display(){
            glClearBufferfv(GL_COLOR, 0, clearColor);

            glm::mat4 I(1.0f);
            glm::vec3 X(1.0f, 0.0f, 0.0f);
            glm::vec3 Y(0.0f, 1.0f, 0.0f);
            glm::vec3 Z(0.0f, 0.0f, 1.0f);
            float t = (float)glfwGetTime();

            glm::mat4 model_matrix = glm::rotate(I, t, X);
            shader_cube->setModelMatrix(model_matrix);

            glm::mat4 view_matrix = glm::translate(I, glm::vec3(0.0f, 0.0f, -3.0f))
                                        * glm::rotate(I, t, Y);
            shader_cube->setViewMatrix(view_matrix);

            glm::mat4 projection_matrix = glm::perspective(glm::radians(45.0f), aspect, 1.0f, 100.0f);
            shader_cube->setProjectionMatrix(projection_matrix);

            shader_cube->setCurrent();

            glBindVertexArray(VAO);
            glEnable(GL_PRIMITIVE_RESTART);
            glPrimitiveRestartIndex(0xFFFF);
            glDrawElements(GL_TRIANGLE_STRIP, 17, GL_UNSIGNED_SHORT, NULL);
        }

        ~MyApp(){
            if(shader_cube != NULL){
                delete shader_cube;
            }
        }

};


DECLARE_MAIN(MyApp)