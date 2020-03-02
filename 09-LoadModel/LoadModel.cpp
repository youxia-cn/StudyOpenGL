#include "../include/app.hpp"
#include "../include/shader.hpp"
#include "../include/mesh.hpp"
#include "../include/model.hpp"

class MyApp : public App {
    private:
        const GLfloat clearColor[4] = {0.2f, 0.3f, 0.3f, 1.0f};
        Model model;

    public:
        void init(){
            
            ShaderInfo shaders[] = {
                {GL_VERTEX_SHADER, "loadmodel.vert"},
                {GL_FRAGMENT_SHADER, "loadmodel.frag"},
                {GL_NONE, ""}
            };
            model.setShader(Shader(shaders));
            model.loaModel("media/mapletree/MapleTree.obj");
            
            glEnable(GL_CULL_FACE);
            glCullFace(GL_BACK);
            glEnable(GL_DEPTH_TEST);
            glDepthFunc(GL_LEQUAL);

        }

        void display(){
            glClearBufferfv(GL_COLOR, 0, clearColor);
            glClear(GL_DEPTH_BUFFER_BIT);

            glm::mat4 I(1.0f);
            glm::vec3 X(1.0f, 0.0f, 0.0f);
            glm::vec3 Y(0.0f, 1.0f, 0.0f);
            glm::vec3 Z(0.0f, 0.0f, 1.0f);
            float t = (float)glfwGetTime();

            glm::mat4 view_matrix = glm::lookAt(glm::vec3(cameraPosition), glm::vec3(cameraPosition) + cameraFront, cameraUp);

            glm::mat4 projection_matrix = glm::perspective(glm::radians(45.0f), aspect, 1.0f, 100.0f);

            glm::mat4 model_matrix = I;

            model.setMatrix(model_matrix, view_matrix, projection_matrix);
            model.render();
        }

};


DECLARE_MAIN(MyApp)