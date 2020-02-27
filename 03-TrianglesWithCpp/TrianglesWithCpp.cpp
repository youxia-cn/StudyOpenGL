#include "../include/app.hpp"
#include "../include/shader.hpp"

class MyApp : public App {
    private:
        GLuint VBO, VAO, EBO;
        Shader *shader_triangles;
        const GLuint NumOfVertices = 4;
        const GLfloat clearColor[4] = {0.2f, 0.3f, 0.3f, 1.0f};

    public:
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

            shader_triangles = new Shader(shaders);
            shader_triangles->setCurrent();

            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
            glEnableVertexAttribArray(0);

        }

        void display(){
            glClearBufferfv(GL_COLOR, 0, clearColor);

            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        }

        ~MyApp(){
            if(shader_triangles != NULL){
                delete shader_triangles;
            }
        }

};


DECLARE_MAIN(MyApp)