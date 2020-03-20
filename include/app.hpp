#ifndef __APP_HPP__
#define __APP_HPP__

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
class App{
    private:
        const int SCR_WIDTH = 1500;
        const int SCR_HEIGHT = 1500;
        
    public:
        static App* the_app;
        float aspect;
        double time_frame_start;
        double time_frame_end;
        int frame_count;
        glm::vec4 cameraPosition;
        glm::vec3 cameraFront;
        glm::vec3 cameraUp;
        float cameraSpeed;
        bool isShowFps;
        bool firstMouse;
        float yaw;
        float pitch;
        float lastX;
        float lastY;

        
        App(){
            aspect = (float)SCR_WIDTH/(float)SCR_HEIGHT;
            cameraPosition = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
            cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
            cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
            yaw   = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
            pitch =  0.0f;
            firstMouse = true;
        }

        virtual void init(){
            
        }
        virtual void display(){
            
        }

        void showFps(int frames){
            //考虑到写入到标准输出是一个比较慢的操作，故每渲染frames帧之后才输出一次帧率
            if(frame_count > frames){
                frame_count = 0;
                std::cout << "fps: " << 1.0/(time_frame_end - time_frame_start) << std::endl;
            }else{
                frame_count ++;
            }
        }

        virtual void onWindowSize(GLFWwindow* window, int width, int height)
        {
            glViewport(0, 0, width, height);
            aspect = (float)width/(float)height;
        }

        
        virtual void onKey(GLFWwindow* window, int key, int scancode, int action, int mods)
        {
            if (action == GLFW_PRESS)
            {
                switch (key)
                {
                    case GLFW_KEY_M: //切换线框模式和填充模式
                        {
                            static GLenum  mode = GL_FILL;
                            mode = ( mode == GL_FILL ? GL_LINE : GL_FILL );
                            glPolygonMode( GL_FRONT_AND_BACK, mode );
                        }
                        return;
                    
                    case GLFW_KEY_F: //切换是否显示帧率
                        {
                            isShowFps = ! isShowFps;
                            return;
                        }
                    case GLFW_KEY_ESCAPE: //在鼠标被捕获后，可以用这个键关闭窗口
                        {
                            glfwSetWindowShouldClose(window, true);
                            return;
                        }
                }
            
            }
        }

        virtual void processInput(GLFWwindow* window){
            if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
                cameraPosition += cameraSpeed * glm::vec4(cameraFront, 0.0f);
            }
            if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
                cameraPosition -= cameraSpeed * glm::vec4(cameraFront, 0.0f);
            }
            if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
                cameraPosition -= cameraSpeed * glm::vec4(glm::normalize(glm::cross(cameraFront, cameraUp)), 0.0f);
            }
            if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
                cameraPosition += cameraSpeed * glm::vec4(glm::normalize(glm::cross(cameraFront, cameraUp)), 0.0f);
            }
            if(glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS){
                pitch += 0.2f;
                calcCameraFront();
            }
            if(glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS){
                pitch -= 0.2f;
                calcCameraFront();
            }
            if(glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS){
                yaw -= 0.2f;
                calcCameraFront();
            }
            if(glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS){
                yaw += 0.2f;
                calcCameraFront();
            }
        }

        void calcCameraFront(){
            // make sure that when pitch is out of bounds, screen doesn't get flipped
            if (pitch > 89.0f)
                pitch = 89.0f;
            if (pitch < -89.0f)
                pitch = -89.0f;

            glm::vec3 front;
            front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
            front.y = sin(glm::radians(pitch));
            front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
            cameraFront = glm::normalize(front);
        }

        virtual void run(App* app){
            if(the_app != NULL){ //同一时刻，只能有一个App运行
                std::cerr << "The the_app is already run." << std::endl;
                return;
            }
            the_app = app;
            glfwInit();
            GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "StudyOpenGL", NULL, NULL);
            if (window == NULL)
            {
                std::cerr << "Failed to create GLFW window" << std::endl;
                glfwTerminate();
                return;
            }
            glfwMakeContextCurrent(window);
            glfwSetKeyCallback(window, glfw_onKey);
            glfwSetWindowSizeCallback(window, glfw_onWindowSize);

            if(glewInit() != GLEW_OK){
                std::cerr << "Failed to initalize GLEW" << std::endl;
                return;
            }

            init(); //Init主要是用来创建VAO、VBO等，并准备要各种数据

            while (!glfwWindowShouldClose(window))
            {
                time_frame_start = glfwGetTime();
                display(); //这里才是渲染图形的主战场                
                glfwSwapBuffers(window);
                time_frame_end = glfwGetTime();
                if(isShowFps){
                    showFps(200);
                }
                cameraSpeed = (time_frame_end - time_frame_start) * 2.5f;
                processInput(window);
                glfwPollEvents();
            }
            glfwDestroyWindow(window);

            glfwTerminate();
            return;
        }

        static void glfw_onKey(GLFWwindow* window, int key, int scancode, int action, int mods){
            the_app->onKey(window, key, scancode, action, mods);
        }

        static void glfw_onWindowSize(GLFWwindow* window, int width, int height){
            the_app->onWindowSize(window, width, height);
        }

};

App* App::the_app = NULL;

#define DECLARE_MAIN(a)                             \
int main(int argc, const char ** argv)              \
{                                                   \
    a *app = new a;                                 \
    app->run(app);                                  \
    delete app;                                     \
    return 0;                                       \
}

#endif