#include "../include/app.hpp"
#include "../include/shader.hpp"

class MyApp : public App {
    private:
        GLuint VBO, VAO, EBO;
        Shader shader;
        const GLuint NumOfVertices = 4;
        const GLfloat clearColor[4] = {0.2f, 0.3f, 0.3f, 1.0f};
        glm::vec2 c[200*200];
        int p = 0;
    public:
        void init(){
            glGenVertexArrays(1, &VAO);
            glBindVertexArray(VAO);

            GLfloat vertices[NumOfVertices][4] = {
                { 1.0f, 1.0f, 2.0f, 2.0f }, 
                { 1.0f, -1.0f, 2.0f, -2.0f }, 
                { -1.0f, -1.0f, -2.0f, -2.0f }, 
                { -1.0f, 1.0f, -2.0f, 2.0f }
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
                {GL_VERTEX_SHADER, "juliaset.vert", 0},
                {GL_FRAGMENT_SHADER, "juliaset.frag", 0},
                {GL_NONE, "", 0}
            };

            shader = Shader(shaders);
            shader.setCurrent();

            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)0);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)(2*sizeof(float)));
            glEnableVertexAttribArray(0);
            glEnableVertexAttribArray(1);
            for(int i=0; i<200; i++){
                for(int j=0; j<200; j++){
                    float r = 2.0f/200.0f * i;
                    float theta = 2 * 3.1415926 / 200.0f * j;
                    c[i*200 + j].x = r * cos(theta);
                    c[i*200 + j].y = r * sin(theta);
                }
            }
        }

        void display(){
            double start = glfwGetTime();
            glClearBufferfv(GL_COLOR, 0, clearColor);
            if(p >= 200*200) p = 0;
            p++;
            shader.setCurrent();
            shader.setVec2("c", c[p]);
            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            while(glfwGetTime() - start < 0.05){

            }
        }

};


DECLARE_MAIN(MyApp)