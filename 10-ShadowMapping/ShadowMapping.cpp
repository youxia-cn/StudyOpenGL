#include "../include/app.hpp"
#include "../include/shader.hpp"
#include "../include/mesh.hpp"
#include "../include/model.hpp"

class MyApp : public App
{
private:
    const GLfloat clearColor[4] = {0.2f, 0.3f, 0.3f, 1.0f};
    Model model;
    Sphere light;
    Plane ground;
    glm::vec4 lightPosition = glm::vec4(10.0f, 10.0f, 10.0f, 1.0f);

public:
    void init()
    {
        ShaderInfo shaders[] = {
            {GL_VERTEX_SHADER, "shadowmapping.vert"},
            {GL_FRAGMENT_SHADER, "shadowmapping.frag"},
            {GL_NONE, ""}};
        model.setShader(Shader(shaders));
        model.loaModel("media/tower/", "tower.obj");
        model.setLightColor(glm::vec3(1.0f, 1.0f, 1.0f));
        model.setTexCoordScaleFactor(1.0f);

        ShaderInfo lightShaders[] = {
            {GL_VERTEX_SHADER, "light.vert"},
            {GL_FRAGMENT_SHADER, "light.frag"},
            {GL_NONE, ""}};

        light.generateMesh(20);
        light.setShader(Shader(lightShaders));
        light.setup();

        ground.generateMesh(20);
        ground.setShader(Shader(shaders));
        ground.setup();
        ground.setKa(glm::vec3(1.0f, 1.0f, 1.0f));
        ground.setKd(glm::vec3(1.0f, 1.0f, 1.0f));
        ground.setKs(glm::vec3(1.0f, 1.0f, 1.0f));
        ground.setNs(80.0f);
        ground.setMapKd("media/tiles03.jpg");
        ground.setTexCoordScaleFactor(50.0f);

        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
    }

    void display()
    {
        glClearBufferfv(GL_COLOR, 0, clearColor);
        glClear(GL_DEPTH_BUFFER_BIT);
        glm::mat4 I(1.0f);
        glm::vec3 X(1.0f, 0.0f, 0.0f);
        glm::vec3 Y(0.0f, 1.0f, 0.0f);
        glm::vec3 Z(0.0f, 0.0f, 1.0f);
        float t = (float)glfwGetTime();

        glm::mat4 view_matrix = glm::lookAt(glm::vec3(cameraPosition), glm::vec3(cameraPosition) + cameraFront, cameraUp);

        glm::mat4 projection_matrix = glm::perspective(glm::radians(45.0f), aspect, 1.0f, 1000.0f);

        glm::mat4 model_matrix = glm::translate(I, glm::vec3(0.0f, -4.0f, -15.0f));

        model.setMatrix(model_matrix, view_matrix, projection_matrix);
        model.setLightPosition(view_matrix * lightPosition);
        model.setCameraPosition(view_matrix * cameraPosition);
        model.render();

        glm::mat4 light_model_matrix = glm::translate(I, glm::vec3(10.0f, 10.0f, 10.0f)) * glm::scale(I, glm::vec3(0.2f, 0.2f, 0.2f));
        light.setMatrix(light_model_matrix, view_matrix, projection_matrix);
        light.render();

        glm::mat4 ground_model_matrix = glm::translate(I, glm::vec3(0.0f, -4.0f, 0.0f)) * glm::rotate(I, glm::radians(-90.0f), X) * glm::scale(I, glm::vec3(100.0f, 100.0f, 100.0f));
        ground.setMatrix(ground_model_matrix, view_matrix, projection_matrix);
        ground.setLightPosition(lightPosition);
        ground.setCameraPosition(view_matrix * cameraPosition);
        ground.setLightColor(glm::vec3(1.0f, 1.0f, 1.0f));
        ground.render();
    }
};

DECLARE_MAIN(MyApp)