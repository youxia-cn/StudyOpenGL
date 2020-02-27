#ifndef __APP_HPP__
#define __APP_HPP__

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
class App{
    private:
        const int SCR_WIDTH = 2000;
        const int SCR_HEIGHT = 1500;
        
    public:
        static App* the_app;
        float aspect;
        double time_start;
        double time_now;
        int frame_count;
        
        App(){
           aspect = (float)SCR_WIDTH/(float)SCR_HEIGHT;
        }

        virtual void init(){
            
        }
        virtual void display(){
            
        }

        void showFps(){
            if(frame_count == 0){
                time_start = glfwGetTime();
                frame_count ++;
            }else if(frame_count > 99){
                frame_count = 0;
                time_now = glfwGetTime();
                float fps = 100.0f / (float)(time_now - time_start);
                std::cout << "Fps: " << fps << std::endl;
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
                    case GLFW_KEY_M:
                        {
                            static GLenum  mode = GL_FILL;

                            mode = ( mode == GL_FILL ? GL_LINE : GL_FILL );
                            glPolygonMode( GL_FRONT_AND_BACK, mode );
                        }
                        return;
                }
            }
        }

        virtual void onMouseButton(GLFWwindow* window, int button, int action, int mods){

        }

        virtual void onMouseMove(GLFWwindow* window, double x, double y){

        }

        virtual void onMouseWheel(GLFWwindow* window, double xoffset, double yoffset){

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
            glfwSetMouseButtonCallback(window, glfw_onMouseButton);
            glfwSetCursorPosCallback(window, glfw_onMouseMove);
            glfwSetScrollCallback(window, glfw_onMouseWheel);

            if(glewInit() != GLEW_OK){
                std::cerr << "Failed to initalize GLEW" << std::endl;
                return;
            }

            init(); //Init主要是用来创建VAO、VBO等，并准备要各种数据

            while (!glfwWindowShouldClose(window))
            {
                display(); //这里才是渲染图形的主战场
                glfwSwapBuffers(window);
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

        static void glfw_onMouseButton(GLFWwindow* window, int button, int action, int mods)
        {
            the_app->onMouseButton(window, button, action, mods);
        }

        static void glfw_onMouseMove(GLFWwindow* window, double x, double y)
        {
            the_app->onMouseMove(window, x, y);
        }

        static void glfw_onMouseWheel(GLFWwindow* window, double xoffset, double yoffset)
        {
            the_app->onMouseWheel(window, xoffset, yoffset);
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