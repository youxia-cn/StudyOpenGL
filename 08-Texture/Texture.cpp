#include "../include/app.hpp"
#include "../include/shader.hpp"
#include "../include/mesh.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class MyApp : public App {
    private:
        const GLfloat clearColor[4] = {0.2f, 0.3f, 0.3f, 1.0f};
        Plane plane;
        Sphere light;
        Sphere sphere;
        Torus torus;
        glm::vec4 lightPosition;

    public:
        void init(){
            
            ShaderInfo shaders[] = {
                {GL_VERTEX_SHADER, "texture.vert"},
                {GL_FRAGMENT_SHADER, "texture.frag"},
                {GL_NONE, ""}
            };

            ShaderInfo lightShaders[] = {
                {GL_VERTEX_SHADER, "light.vert"},
                {GL_FRAGMENT_SHADER, "light.frag"},
                {GL_NONE, ""}
            };

            plane.generateMesh(20);
            plane.setShader(Shader(shaders));
            plane.setup();
            plane.setKa(glm::vec3(0.0f, 0.0f, 0.0f));
            plane.setKd(glm::vec3(0.0f, 0.0f, 0.0f));
            plane.setKs(glm::vec3(1.0f, 1.0f, 1.0f));
            plane.setNs(80.0f);
            plane.setMapKa("media/tiles03.jpg");
            plane.setMapKd("media/tiles03.jpg");
            plane.setTexCoordScaleFactor(50.0f);

            sphere.generateMesh(96);
            sphere.setShader(Shader(shaders));
            sphere.setup();
            sphere.setKa(glm::vec3(0.0f, 0.0f, 0.0f));
            sphere.setKd(glm::vec3(0.0f, 0.0f, 0.0f));
            sphere.setKs(glm::vec3(1.0f, 1.0f, 1.0f));
            sphere.setNs(80.0f);
            sphere.setMapKa("media/tiles02.jpg");
            sphere.setMapKd("media/tiles02.jpg");
            sphere.setTexCoordScaleFactor(5.0f);

            torus.generateMesh(96);
            torus.setShader(Shader(shaders));
            torus.setup();
            torus.setKa(glm::vec3(0.0f, 0.0f, 0.0f));
            torus.setKd(glm::vec3(0.0f, 0.0f, 0.0f));
            torus.setKs(glm::vec3(1.0f, 1.0f, 1.0f));
            torus.setNs(80.0f);
            torus.setMapKa("media/tiles01.jpg");
            torus.setMapKd("media/tiles01.jpg");
            torus.setTexCoordScaleFactor(5.0f);

            light.generateMesh(20);
            light.setShader(Shader(lightShaders));
            light.setup();
            
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

            glm::mat4 plane_model_matrix = glm::translate(I, glm::vec3(0.0f, -1.0f, 0.0f)) 
                                        * glm::rotate(I, glm::radians(-90.0f), X)
                                        * glm::scale(I, glm::vec3(100.0f, 100.0f, 100.0f));
            
            glm::mat4 light_model_matrix = glm::translate(I, glm::vec3(3.0f, 3.0f, 8.0f))
                                                * glm::scale(I, glm::vec3(0.2f, 0.2f, 0.2f));
            lightPosition = view_matrix * glm::vec4(3.0f, 3.0f, 8.0f, 1.0f);

            light.setMatrix(light_model_matrix, view_matrix, projection_matrix);
            light.render();

            plane.setMatrix(plane_model_matrix, view_matrix, projection_matrix);
            plane.setLightPosition(lightPosition);
            plane.setCameraPosition(view_matrix * cameraPosition);
            plane.setLightColor(glm::vec3(1.0f, 1.0f, 1.0f));
            plane.render();

            glm::mat4 sphere_model_matrix = glm::translate(I, glm::vec3(1.0f, 0.3f, 0.0f))
                                                * glm::scale(I, glm::vec3(0.8f, 0.8f, 0.8f));
            sphere.setMatrix(sphere_model_matrix, view_matrix, projection_matrix);
            sphere.setLightPosition(lightPosition);
            sphere.setCameraPosition(view_matrix * cameraPosition);
            sphere.setLightColor(glm::vec3(1.0f, 1.0f, 1.0f));
            sphere.render();

            glm::mat4 torus_model_matrix = glm::translate(I, glm::vec3(-1.0f, 0.3f, 0.0f))
                                                * glm::rotate(I, glm::radians(90.0f), Y)
                                                * glm::scale(I, glm::vec3(1.3f, 1.3f, 1.3f));
            torus.setMatrix(torus_model_matrix, view_matrix, projection_matrix);
            torus.setLightPosition(lightPosition);
            sphere.setCameraPosition(view_matrix * cameraPosition);
            torus.setLightColor(glm::vec3(1.0f, 1.0f, 1.0f));
            torus.render();
        }

        ~MyApp(){
            
        }

};


DECLARE_MAIN(MyApp)