#include "../include/app.hpp"
#include "../include/shader.hpp"
#include "../include/mesh.hpp"
#include "../include/model.hpp"

class MyApp : public App
{
private:
    const GLfloat clearColor[4] = {0.2f, 0.3f, 0.3f, 1.0f};
    Plane floor;
    Plane top;
    Plane left;
    Plane right;
    Plane front;
    Plane back;
    glm::vec4 lightPosition01 = glm::vec4(5.0f, 0.0f, 5.0f, 1.0f);
    glm::vec4 lightPosition02 = glm::vec4(-5.0f, 0.0f, 5.0f, 1.0f);
    glm::vec4 lightPosition03 = glm::vec4(5.0f, 0.0f, -5.0f, 1.0f);
    glm::vec4 lightPosition04 = glm::vec4(-5.0f, 0.0f, -5.0f, 1.0f);
    glm::vec3 lightColor01 = glm::vec3(15.0f, 15.0f, 15.0f);
    glm::vec3 lightColor02 = glm::vec3(3.0f, 0.0f, 0.0f);
    glm::vec3 lightColor03 = glm::vec3(0.0f, 3.0f, 0.0f);
    glm::vec3 lightColor04 = glm::vec3(0.0f, 0.0f, 3.0f);
    Sphere light01;
    Sphere light02;
    Sphere light03;
    Sphere light04;
    Sphere sphere;
    Torus torus;
    Shader lightShader;
    Shader sceneShader;
    GLuint hdrFBO;
    GLuint colorBuffers[2];
    GLuint pingpongFBO[2];
    GLuint pingpongBuffer[2];
    Plane quad;
    Plane quad2;
    GLuint depthBuffer;

public:
    void init()
    {
        //用来绘制光源的Shader文件
        ShaderInfo lightShaders[] = {
            {GL_VERTEX_SHADER, "light.vert"},
            {GL_FRAGMENT_SHADER, "light.frag"},
            {GL_NONE, ""}};
        //用来绘制场景的Shader文件
        ShaderInfo sceneShaders[] = {
            {GL_VERTEX_SHADER, "scene.vert"},
            {GL_FRAGMENT_SHADER, "scene.frag"},
            {GL_NONE, ""}};

        //用来进行高斯模糊的Shader文件
        ShaderInfo bloomShaders[] = {
            {GL_VERTEX_SHADER, "bloom.vert"},
            {GL_FRAGMENT_SHADER, "bloom.frag"},
            {GL_NONE, ""}};
        ShaderInfo bloomShaders2[] = {
            {GL_VERTEX_SHADER, "bloom.vert"},
            {GL_FRAGMENT_SHADER, "bloom2.frag"},
            {GL_NONE, ""}};

        //光源
        lightShader = Shader(lightShaders);
        light01.generateMesh(60);
        light01.setShader(lightShader);
        light01.setup();
        light02.generateMesh(60);
        light02.setShader(lightShader);
        light02.setup();
        light03.generateMesh(60);
        light03.setShader(lightShader);
        light03.setup();
        light04.generateMesh(60);
        light04.setShader(lightShader);
        light04.setup();
        //地面和墙壁
        floor.setShader(Shader(sceneShaders));
        floor.generateMesh(20);
        floor.setup();
        floor.setMapKd("media/texture01.jpg");
        left.setShader(Shader(sceneShaders));
        left.generateMesh(20);
        left.setup();
        left.setMapKd("media/texture07.jpg");
        front.setShader(Shader(sceneShaders));
        front.generateMesh(20);
        front.setup();
        front.setMapKd("media/texture04.jpg");
        right.setShader(Shader(sceneShaders));
        right.generateMesh(20);
        right.setup();
        right.setMapKd("media/texture05.jpg");
        back.setShader(Shader(sceneShaders));
        back.generateMesh(20);
        back.setup();
        back.setMapKd("media/texture08.jpg");
        top.setShader(Shader(sceneShaders));
        top.generateMesh(20);
        top.setup();
        top.setMapKd("media/texture06.jpg");
        sphere.setShader(Shader(sceneShaders));
        sphere.generateMesh(96);
        sphere.setup();
        sphere.setMapKd("media/texture03.jpg");
        torus.setShader(Shader(sceneShaders));
        torus.generateMesh(96);
        torus.setup();
        torus.setMapKd("media/texture02.jpg");

        //用来进行高斯模糊的四边形
        quad.setShader(Shader(bloomShaders));
        quad.generateMesh(2);
        quad.setup();
        quad2.setShader(Shader(bloomShaders2));
        quad2.generateMesh(2);
        quad2.setup();

        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);

        //创建一个framebuffer和两个texture，用来分别提取渲染结果的亮部和暗部
        glGenFramebuffers(1, &hdrFBO);
        glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);

        glGenTextures(1, &depthBuffer);
        glBindTexture(GL_TEXTURE_2D, depthBuffer);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 2000, 1500, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthBuffer, 0);
        
        glGenTextures(2, colorBuffers);
        for (GLuint i = 0; i < 2; i++)
        {
            glBindTexture(GL_TEXTURE_2D, colorBuffers[i]);
            glTexImage2D(
                GL_TEXTURE_2D, 0, GL_RGB16F, 2000, 1500, 0, GL_RGB, GL_FLOAT, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            // attach texture to framebuffer
            glFramebufferTexture2D(
                GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, colorBuffers[i], 0);
        }
        
        
        GLuint attachments[2] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
        glDrawBuffers(2, attachments);

        //创建用于高斯模糊的缓冲区
        glGenFramebuffers(2, pingpongFBO);
        glGenTextures(2, pingpongBuffer);
        for (GLuint i = 0; i < 2; i++)
        {
            glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[i]);
            glBindTexture(GL_TEXTURE_2D, pingpongBuffer[i]);
            glTexImage2D(
                GL_TEXTURE_2D, 0, GL_RGB16F, 2000, 1500, 0, GL_RGB, GL_FLOAT, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glFramebufferTexture2D(
                GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pingpongBuffer[i], 0);
        }
    }

    void display()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
        //glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClearBufferfv(GL_COLOR, 0, clearColor);
        glClearDepth(1.0f);
        glClear(GL_DEPTH_BUFFER_BIT);
        glm::mat4 I(1.0f);
        glm::vec3 X(1.0f, 0.0f, 0.0f);
        glm::vec3 Y(0.0f, 1.0f, 0.0f);
        glm::vec3 Z(0.0f, 0.0f, 1.0f);
        float t = (float)glfwGetTime();

        glm::mat4 view_matrix = glm::lookAt(glm::vec3(cameraPosition), glm::vec3(cameraPosition) + cameraFront, cameraUp);
        glm::mat4 projection_matrix = glm::perspective(glm::radians(45.0f), aspect, 1.0f, 100.0f);
        //渲染光源
        glm::mat4 light01_model_matrix = glm::translate(I, glm::vec3(lightPosition01)) * glm::scale(I, glm::vec3(0.5f, 0.5f, 0.5f));
        light01.setMatrix(light01_model_matrix, view_matrix, projection_matrix);
        light01.setLightColor(lightColor01);
        light01.render();
        glm::mat4 light02_model_matrix = glm::translate(I, glm::vec3(lightPosition02)) * glm::scale(I, glm::vec3(0.5f, 0.5f, 0.5f));
        light02.setMatrix(light02_model_matrix, view_matrix, projection_matrix);
        light02.setLightColor(lightColor02);
        light02.render();
        glm::mat4 light03_model_matrix = glm::translate(I, glm::vec3(lightPosition03)) * glm::scale(I, glm::vec3(0.5f, 0.5f, 0.5f));
        light03.setMatrix(light03_model_matrix, view_matrix, projection_matrix);
        light03.setLightColor(lightColor03);
        light03.render();
        glm::mat4 light04_model_matrix = glm::translate(I, glm::vec3(lightPosition04)) * glm::scale(I, glm::vec3(0.5f, 0.5f, 0.5f));
        light04.setMatrix(light04_model_matrix, view_matrix, projection_matrix);
        light04.setLightColor(lightColor04);
        light04.render();
        //地面和墙壁
        glm::mat4 floor_model_matrix = glm::translate(I, glm::vec3(0.0f, -2.0f, 0.0f)) * glm::rotate(I, glm::radians(-90.0f), X) * glm::scale(I, glm::vec3(20.0f, 20.0f, 20.0f));
        floor.setMatrix(floor_model_matrix, view_matrix, projection_matrix);
        floor.shader.setVec4("lightPosition01", view_matrix * lightPosition01);
        floor.shader.setVec3("lightColor01", lightColor01);
        floor.shader.setVec4("lightPosition02", view_matrix * lightPosition02);
        floor.shader.setVec3("lightColor02", lightColor02);
        floor.shader.setVec4("lightPosition03", view_matrix * lightPosition03);
        floor.shader.setVec3("lightColor03", lightColor03);
        floor.shader.setVec4("lightPosition04", view_matrix * lightPosition04);
        floor.shader.setVec3("lightColor04", lightColor04);
        floor.render();
        glm::mat4 left_model_matrix = glm::translate(I, glm::vec3(-10.0f, 0.0f, 0.0f)) * glm::rotate(I, glm::radians(90.0f), Y) * glm::scale(I, glm::vec3(20.0f, 20.0f, 20.0f));
        left.setMatrix(left_model_matrix, view_matrix, projection_matrix);
        left.shader.setVec4("lightPosition01", view_matrix * lightPosition01);
        left.shader.setVec3("lightColor01", lightColor01);
        left.shader.setVec4("lightPosition02", view_matrix * lightPosition02);
        left.shader.setVec3("lightColor02", lightColor02);
        left.shader.setVec4("lightPosition03", view_matrix * lightPosition03);
        left.shader.setVec3("lightColor03", lightColor03);
        left.shader.setVec4("lightPosition04", view_matrix * lightPosition04);
        left.shader.setVec3("lightColor04", lightColor04);
        left.render();
        glm::mat4 front_model_matrix = glm::translate(I, glm::vec3(0.0f, 0.0f, -10.0f)) * glm::scale(I, glm::vec3(20.0f, 20.0f, 20.0f));
        front.setMatrix(front_model_matrix, view_matrix, projection_matrix);
        front.shader.setVec4("lightPosition01", view_matrix * lightPosition01);
        front.shader.setVec3("lightColor01", lightColor01);
        front.shader.setVec4("lightPosition02", view_matrix * lightPosition02);
        front.shader.setVec3("lightColor02", lightColor02);
        front.shader.setVec4("lightPosition03", view_matrix * lightPosition03);
        front.shader.setVec3("lightColor03", lightColor03);
        front.shader.setVec4("lightPosition04", view_matrix * lightPosition04);
        front.shader.setVec3("lightColor04", lightColor04);
        front.render();
        glm::mat4 right_model_matrix = glm::translate(I, glm::vec3(10.0f, 0.0f, 0.0f)) * glm::rotate(I, glm::radians(-90.0f), Y) * glm::scale(I, glm::vec3(20.0f, 20.0f, 20.0f));
        right.setMatrix(right_model_matrix, view_matrix, projection_matrix);
        right.shader.setVec4("lightPosition01", view_matrix * lightPosition01);
        right.shader.setVec3("lightColor01", lightColor01);
        right.shader.setVec4("lightPosition02", view_matrix * lightPosition02);
        right.shader.setVec3("lightColor02", lightColor02);
        right.shader.setVec4("lightPosition03", view_matrix * lightPosition03);
        right.shader.setVec3("lightColor03", lightColor03);
        right.shader.setVec4("lightPosition04", view_matrix * lightPosition04);
        right.shader.setVec3("lightColor04", lightColor04);
        right.render();
        glm::mat4 back_model_matrix = glm::translate(I, glm::vec3(0.0f, 0.0f, 10.0f)) * glm::rotate(I, glm::radians(-180.0f), Y) * glm::scale(I, glm::vec3(20.0f, 20.0f, 20.0f));
        back.setMatrix(back_model_matrix, view_matrix, projection_matrix);
        back.shader.setVec4("lightPosition01", view_matrix * lightPosition01);
        back.shader.setVec3("lightColor01", lightColor01);
        back.shader.setVec4("lightPosition02", view_matrix * lightPosition02);
        back.shader.setVec3("lightColor02", lightColor02);
        back.shader.setVec4("lightPosition03", view_matrix * lightPosition03);
        back.shader.setVec3("lightColor03", lightColor03);
        back.shader.setVec4("lightPosition04", view_matrix * lightPosition04);
        back.shader.setVec3("lightColor04", lightColor04);
        back.render();
        glm::mat4 top_model_matrix = glm::translate(I, glm::vec3(0.0f, 10.0f, 0.0f)) * glm::rotate(I, glm::radians(90.0f), X) * glm::scale(I, glm::vec3(20.0f, 20.0f, 20.0f));
        top.setMatrix(top_model_matrix, view_matrix, projection_matrix);
        top.shader.setVec4("lightPosition01", view_matrix * lightPosition01);
        top.shader.setVec3("lightColor01", lightColor01);
        top.shader.setVec4("lightPosition02", view_matrix * lightPosition02);
        top.shader.setVec3("lightColor02", lightColor02);
        top.shader.setVec4("lightPosition03", view_matrix * lightPosition03);
        top.shader.setVec3("lightColor03", lightColor03);
        top.shader.setVec4("lightPosition04", view_matrix * lightPosition04);
        top.shader.setVec3("lightColor04", lightColor04);
        top.render();
        glm::mat4 sphere_model_matrix = glm::translate(I, glm::vec3(-2.5f, 0.0f, 0.0f)) * glm::scale(I, glm::vec3(2.0f, 2.0f, 2.0f));
        sphere.setMatrix(sphere_model_matrix, view_matrix, projection_matrix);
        sphere.shader.setVec4("lightPosition01", view_matrix * lightPosition01);
        sphere.shader.setVec3("lightColor01", lightColor01);
        sphere.shader.setVec4("lightPosition02", view_matrix * lightPosition02);
        sphere.shader.setVec3("lightColor02", lightColor02);
        sphere.shader.setVec4("lightPosition03", view_matrix * lightPosition03);
        sphere.shader.setVec3("lightColor03", lightColor03);
        sphere.shader.setVec4("lightPosition04", view_matrix * lightPosition04);
        sphere.shader.setVec3("lightColor04", lightColor04);
        sphere.render();
        glm::mat4 torus_model_matrix = glm::translate(I, glm::vec3(2.5f, 0.0f, 0.0f)) * glm::scale(I, glm::vec3(2.0f, 2.0f, 2.0f));
        torus.setMatrix(torus_model_matrix, view_matrix, projection_matrix);
        torus.shader.setVec4("lightPosition01", view_matrix * lightPosition01);
        torus.shader.setVec3("lightColor01", lightColor01);
        torus.shader.setVec4("lightPosition02", view_matrix * lightPosition02);
        torus.shader.setVec3("lightColor02", lightColor02);
        torus.shader.setVec4("lightPosition03", view_matrix * lightPosition03);
        torus.shader.setVec3("lightColor03", lightColor03);
        torus.shader.setVec4("lightPosition04", view_matrix * lightPosition04);
        torus.shader.setVec3("lightColor04", lightColor04);
        torus.render();
        //渲染到纹理完成

        //下面开始高斯模糊
        GLboolean horizontal = true, first_iteration = true;
        GLuint amount = 10;
        for (GLuint i = 0; i < amount; i++)
        {
            glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[horizontal]);
            quad.shader.setInt("horizontal", horizontal);
            glActiveTexture(GL_TEXTURE4);
            glBindTexture(
                GL_TEXTURE_2D, first_iteration ? colorBuffers[1] : pingpongBuffer[!horizontal]);
            quad.shader.setInt("image", 4);
            quad.render();
            horizontal = !horizontal;
            if (first_iteration)
                first_iteration = false;
        }
        

        //最后进行融合
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClearBufferfv(GL_COLOR, 0, clearColor);
        glClearDepth(1.0f);
        glClear(GL_DEPTH_BUFFER_BIT);
        glActiveTexture(GL_TEXTURE5);
        glBindTexture(GL_TEXTURE_2D, colorBuffers[0]);
        quad2.shader.setInt("scene", 5);
        glActiveTexture(GL_TEXTURE6);
        glBindTexture(GL_TEXTURE_2D, colorBuffers[1]);
        quad2.shader.setInt("bloomBlur", 6);
        quad2.render();

        
    }
};

DECLARE_MAIN(MyApp)