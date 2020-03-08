#include "../include/app.hpp"
#include "../include/shader.hpp"
#include "../include/mesh.hpp"
#include "../include/model.hpp"

class MyApp : public App
{
private:
    const GLfloat clearColor[4] = {0.2f, 0.3f, 0.3f, 1.0f};
    Model tower;
    Shader towerShadowShader;
    Shader towerShader;
    Shader groundShadowShader;
    Shader groundShader;
    Shader testShader;
    Sphere light;
    Plane ground;
    glm::vec4 lightPosition = glm::vec4(10.0f, 10.0f, 10.0f, 1.0f);
    GLuint depth_texture;
    GLuint depth_fbo;
    Plane plane;

public:
    void init()
    {
        //该程序中主要的Shader文件
        ShaderInfo shaders[] = {
            {GL_VERTEX_SHADER, "shadowmapping.vert"},
            {GL_FRAGMENT_SHADER, "shadowmapping.frag"},
            {GL_NONE, ""}};
        //用来生成阴影贴图的Shader文件
        ShaderInfo shadowShaders[] = {
            {GL_VERTEX_SHADER, "shadow.vert"},
            {GL_FRAGMENT_SHADER, "shadow.frag"},
            {GL_NONE, ""}};
        //用来绘制光源的Shader文件
        ShaderInfo lightShaders[] = {
            {GL_VERTEX_SHADER, "light.vert"},
            {GL_FRAGMENT_SHADER, "light.frag"},
            {GL_NONE, ""}};
        //用来测试阴影贴图的Shader文件
        ShaderInfo testShaders[] = {
            {GL_VERTEX_SHADER, "test.vert"},
            {GL_FRAGMENT_SHADER, "test.frag"},
            {GL_NONE, ""}};

        //光源
        light.generateMesh(20);
        light.setShader(Shader(lightShaders));
        light.setup();

        //塔
        towerShadowShader = Shader(shadowShaders);
        towerShader = Shader(shaders);
        tower.setShader(towerShader);
        tower.loaModel("media/tower/", "tower.obj");
        tower.setLightColor(glm::vec3(1.0f, 1.0f, 1.0f));
        tower.setTexCoordScaleFactor(1.0f);

        //地面
        groundShadowShader = Shader(shadowShaders);
        groundShader = Shader(shaders);
        ground.generateMesh(20);
        ground.setShader(groundShader);
        ground.setup();
        ground.setKa(glm::vec3(1.0f, 1.0f, 1.0f));
        ground.setKd(glm::vec3(1.0f, 1.0f, 1.0f));
        ground.setKs(glm::vec3(1.0f, 1.0f, 1.0f));
        ground.setNs(80.0f);
        ground.setMapKd("media/tiles03.jpg");
        ground.setTexCoordScaleFactor(50.0f);
        ground.setLightColor(glm::vec3(1.0f, 1.0f, 1.0f));

        //测试阴影贴图用的平面
        testShader = Shader(testShaders);
        plane.generateMesh(10);
        plane.setShader(testShader);
        plane.setup();

        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);

        //创建阴影贴图需要使用的 texture 和 framebuffer
        glGenTextures(1, &depth_texture);
        glBindTexture(GL_TEXTURE_2D, depth_texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 2000, 1500, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        //下面两行很重要，如果没有，则在Shader中不能使用 textureProj() 函数
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glBindTexture(GL_TEXTURE_2D, 0);

        glGenFramebuffers(1, &depth_fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, depth_fbo);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth_texture, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void display()
    {
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
        glm::mat4 light_model_matrix = glm::translate(I, glm::vec3(10.0f, 10.0f, 10.0f)) * glm::scale(I, glm::vec3(0.2f, 0.2f, 0.2f));
        light.setMatrix(light_model_matrix, view_matrix, projection_matrix);
        light.render();

        //创建阴影贴图
        glBindFramebuffer(GL_FRAMEBUFFER, depth_fbo);
        glViewport(0, 0, 2000, 1500);
        glClearDepth(1.0f);
        glClear(GL_DEPTH_BUFFER_BIT);
        glEnable(GL_POLYGON_OFFSET_FILL);
        glPolygonOffset(2.0f, 4.0f);

        glm::mat4 light_view_matrix = glm::lookAt(glm::vec3(lightPosition), glm::vec3(0.0f, -4.0f, -15.0f), Y);
        //用阴影Shader程序渲染塔
        glm::mat4 tower_model_matrix = glm::translate(I, glm::vec3(0.0f, -4.0f, -15.0f));
        tower.setShader(towerShadowShader);
        tower.setMatrix(tower_model_matrix, light_view_matrix, projection_matrix);
        tower.render();
        //用阴影Shader程序渲染地面
        glm::mat4 ground_model_matrix = glm::translate(I, glm::vec3(0.0f, -3.3f, 0.0f)) * glm::rotate(I, glm::radians(-90.0f), X) * glm::scale(I, glm::vec3(100.0f, 100.0f, 100.0f));
        ground.setShader(groundShadowShader);
        ground.setMatrix(ground_model_matrix, light_view_matrix, projection_matrix);
        ground.render();

        glDisable(GL_POLYGON_OFFSET_FILL);

        //测试阴影贴图
        // glBindFramebuffer(GL_FRAMEBUFFER, 0);
        // testShader.setCurrent();
        // glActiveTexture(GL_TEXTURE4);
        // glBindTexture(GL_TEXTURE_2D, depth_texture);
        // testShader.setInt("shadowMap", 4);
        // plane.render();

        //使用阴影贴图
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glBindTexture(GL_TEXTURE_2D, depth_texture);
        glm::mat4 scale_bias_matrix = glm::mat4(glm::vec4(0.5f, 0.0f,0.0f, 0.0f),glm::vec4(0.0f, 0.5f, 0.0f, 0.0f), glm::vec4(0.0f, 0.0f, 0.5f, 0.0f), glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
        glm::mat4 shadow_matrix = scale_bias_matrix * projection_matrix * light_view_matrix;
        tower.setShader(towerShader);
        tower.setMatrix(tower_model_matrix, view_matrix, projection_matrix);
        tower.setLightPosition(view_matrix * lightPosition);
        tower.setCameraPosition(view_matrix * cameraPosition);
        tower.setShadowMatrix(shadow_matrix);
        towerShader.setCurrent();
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, depth_texture);
        towerShader.setInt("depth_texture", 4);
        tower.render();
        glBindTexture(GL_TEXTURE_2D, depth_texture);
        ground.setShader(groundShader);
        ground.setMatrix(ground_model_matrix, view_matrix, projection_matrix);
        ground.setLightPosition(lightPosition);
        ground.setCameraPosition(view_matrix * cameraPosition);
        ground.setShadowMatrix(shadow_matrix);
        groundShader.setCurrent();
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, depth_texture);
        groundShader.setInt("depth_texture", 4);
        ground.render();
    }
};

DECLARE_MAIN(MyApp)