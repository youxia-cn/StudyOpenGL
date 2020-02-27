#include "../include/app.hpp"
#include "../include/shader.hpp"
#include "../include/mesh.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class MyApp : public App {
    private:
        const GLfloat clearColor[4] = {0.2f, 0.3f, 0.3f, 1.0f};
        Plane plane;
        Sphere sphere;
        Torus torus;

    public:
        void init(){
            
            ShaderInfo shaders[] = {
                {GL_VERTEX_SHADER, "sphereworld.vert"},
                {GL_FRAGMENT_SHADER, "sphereworld.frag"},
                {GL_NONE, ""}
            };

            plane.generateMesh(20);
            plane.setShader(Shader(shaders));
            plane.setup();
            plane.setKa(glm::vec3(0.5f, 0.5f, 0.5f));

            sphere.generateMesh(60);
            sphere.setShader(Shader(shaders));
            sphere.setup();
            sphere.setKa(glm::vec3(0.5f, 0.0f, 0.0f));

            torus.generateMesh(60);
            torus.setShader(Shader(shaders));
            torus.setup();
            torus.setKa(glm::vec3(0.0f, 0.0f, 0.5f));
            
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

            glm::mat4 view_matrix = glm::translate(I, glm::vec3(0.0f, 0.0f, -5.0f))
                                        * glm::rotate(I, t, Y);

            glm::mat4 projection_matrix = glm::perspective(glm::radians(45.0f), aspect, 1.0f, 100.0f);

            glm::mat4 plane_model_matrix = glm::translate(I, glm::vec3(0.0f, -1.0f, 0.0f)) 
                                        * glm::rotate(I, glm::radians(-90.0f), X)
                                        * glm::scale(I, glm::vec3(50.0f, 50.0f, 50.0f));
            
            plane.setMatrix(plane_model_matrix, view_matrix, projection_matrix);
            plane.render();

            glm::mat4 sphere_model_matrix = glm::translate(I, glm::vec3(1.0f, 0.3f, 0.0f))
                                                * glm::scale(I, glm::vec3(0.8f, 0.8f, 0.8f));
            sphere.setMatrix(sphere_model_matrix, view_matrix, projection_matrix);
            sphere.render();

            glm::mat4 torus_model_matrix = glm::translate(I, glm::vec3(-1.0f, 0.3f, 0.0f))
                                                * glm::rotate(I, glm::radians(90.0f), Y)
                                                * glm::scale(I, glm::vec3(1.3f, 1.3f, 1.3f));
            torus.setMatrix(torus_model_matrix, view_matrix, projection_matrix);
            torus.render();

            //showFps();
        }

        ~MyApp(){
            
        }

};


DECLARE_MAIN(MyApp)