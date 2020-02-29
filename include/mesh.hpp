#ifndef __MESH_H__
#define __MESH_H__

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <string>
#include <GL/glew.h>
#include "shader.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

struct Vertex{
    glm::vec4 position;
    glm::vec3 normal;
    glm::vec2 texCoord;
};

class Mesh{
    protected:
        std::vector<Vertex> vertices;
        std::vector<GLuint> indices;
        //和材质有关的属性
        GLuint mapKa;
        GLuint mapKd;
        GLuint mapKs;
        //把数据传递到Shader需要的一些id
        GLuint VAO, VBO, EBO;
        //每一个Mesh对应一个Shader
        Shader shader;
        
    public:
        Mesh(){
            
        }
        void generateMesh(int iSlices);
        void setup(){
            glCreateVertexArrays(1, &VAO);
            glBindVertexArray(VAO);
            glCreateBuffers(1, &VBO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glNamedBufferStorage(VBO, sizeof(Vertex)*vertices.size(), &vertices[0], 0);
            glGenBuffers(1, &EBO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float)*indices.size(), &indices[0], GL_STATIC_DRAW);
            glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));
            glEnableVertexAttribArray(2);
        }
        void render(){
            glBindVertexArray(VAO);
            shader.setCurrent();
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, mapKa);
            shader.setInt("mapKa", 0);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, mapKd);
            shader.setInt("mapKd", 1);
            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, mapKs);
            shader.setInt("mapKs", 2);
            glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        }

        GLuint loadTexture(std::string texture_path){
            GLuint texture_id;
            glGenTextures(1, &texture_id);

            int width, height, channels;
            unsigned char *data = stbi_load(texture_path.c_str(), &width, &height, &channels, 0);
            if(data){
                GLenum format;
                if(channels == 1){
                    format = GL_RED;
                }else if(channels == 3){
                    format = GL_RGB;
                }else if(channels == 4){
                    format = GL_RGBA;
                }

                glBindTexture(GL_TEXTURE_2D, texture_id);
                glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
                glGenerateMipmap(GL_TEXTURE_2D);

                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

                stbi_image_free(data);
            }else{
                std::cout << "Texture load failed : " << texture_path << std::endl;
                stbi_image_free(data);
            }
            return texture_id;
        }

        void setMatrix(glm::mat4 model_matrix, glm::mat4 view_matrix, glm::mat4 projection_matrix){
            shader.setCurrent();
            shader.setModelMatrix(model_matrix);
            shader.setViewMatrix(view_matrix);
            shader.setProjectionMatrix(projection_matrix);
        }

        void setKa(glm::vec3 Ka){
            shader.setCurrent();
            shader.setVec3("Ka", Ka);
        }

        void setKd(glm::vec3 Kd){
            shader.setCurrent();
            shader.setVec3("Kd", Kd);
        }

        void setKs(glm::vec3 Ks){
            shader.setCurrent();
            shader.setVec3("Ks", Ks);
        }
        void setNs(GLfloat Ns){
            shader.setCurrent();
            shader.setFloat("Ns", Ns);
        }

        void setTexCoordScaleFactor(float factor){
            shader.setCurrent();
            shader.setFloat("texCoordScaleFactor", factor);
        }

        void setMapKa(std::string filename){
                mapKa = loadTexture(filename);
        }

        void setMapKd(std::string filename){
                mapKd = loadTexture(filename);
        }

        void setMapKs(std::string filename){
                mapKs = loadTexture(filename);
        }

        void setLightPosition(glm::vec4 lightPosition){
            shader.setCurrent();
            shader.setVec4("lightPosition", lightPosition);
        }

        void setCameraPosition(glm::vec4 cameraPosition){
            shader.setCurrent();
            shader.setVec4("cameraPosition", cameraPosition);
        }

        void setLightColor(glm::vec3 color){
            shader.setCurrent();
            shader.setVec3("lightColor", color);
        }

        void setShader(Shader shader){
            this->shader = shader;
        }

        ~Mesh(){
            
        }
};

class Plane: public Mesh{
    public:
        void generateMesh(int iSlices){
            int n = iSlices + 1;
            float s = 2.0f/(float)iSlices;
            Vertex temp_vertex;
            for(int i=0; i<n; i++){
                for(int j=0; j<n; j++){
                    temp_vertex.position = glm::vec4(s*j - 1.0f, s*i - 1.0f, 0.0f, 1.0f);
                    temp_vertex.normal = glm::vec3(0.0f, 0.0f, 1.0f);
                    temp_vertex.texCoord = glm::vec2(1.0f/(float)iSlices * j, 1.0f/(float)iSlices * i);
                    vertices.push_back(temp_vertex);
                }
            }
            
            for(int i=0; i<iSlices; i++){
                for(int j=0; j<iSlices; j++){
                    indices.push_back(i*n + j);
                    indices.push_back((i+1)*n + j+1);
                    indices.push_back((i+1)*n + j);

                    indices.push_back(i*n + j);
                    indices.push_back(i*n + j + 1);
                    indices.push_back((i+1)*n + j+1);
                }
            }
        }
};

class Sphere: public Mesh{
    public:
        void generateMesh(int iSlices){
            int m = iSlices/2 + 1;
            int n = iSlices+1;
            float s = 360.0f/(float)iSlices;
            glm::vec4 up(0.0f, 1.0f, 0.0f, 1.0f);
            glm::mat4 I(1.0f);
            glm::vec3 X(1.0f, 0.0f, 0.0f);
            glm::vec3 Y(0.0f, 1.0f, 0.0f);
            glm::vec3 Z(0.0f, 0.0f, 1.0f);
            Vertex temp_vertex;
            for(int i=0; i<m; i++){
                for(int j=0; j<n; j++){
                    temp_vertex.position = glm::rotate(I, glm::radians(s*j), Y) * glm::rotate(I, glm::radians(s*i), Z) * up;
                    temp_vertex.normal = temp_vertex.position;
                    temp_vertex.texCoord = glm::vec2(1.0f/(float)(n-1) * j, 1.0f/(float)(m-1) * i);
                    vertices.push_back(temp_vertex);
                }
            }
            
            for(int i=0; i<m-1; i++){
                for(int j=0; j<n-1; j++){
                    indices.push_back(i*n + j);
                    indices.push_back((i+1)*n + j);
                    indices.push_back((i+1)*n + j+1);

                    indices.push_back(i*n + j);
                    indices.push_back((i+1)*n + j+1);
                    indices.push_back(i*n + j + 1);                   
                }
            }
        }
};

class Torus: public Mesh{
    public:
        void generateMesh(int iSlices){
            int n = iSlices + 1;
            float s = -360.0f/(float)iSlices;
            glm::vec4 top(0.0f, 0.2f, 0.0f, 1.0f);
            glm::vec4 normal_up(0.0f, 1.0f, 0.0f, 1.0f);
            glm::mat4 I(1.0f);
            glm::vec3 X(1.0f, 0.0f, 0.0f);
            glm::vec3 Y(0.0f, 1.0f, 0.0f);
            glm::vec3 Z(0.0f, 0.0f, 1.0f);
            Vertex temp_vertex;
            for(int i=0; i<n; i++){
                for(int j=0; j<n; j++){
                    temp_vertex.position = glm::rotate(I, glm::radians(s*j), Z) 
                                            * glm::translate(I, glm::vec3(0.0f, 0.8f, 0.0f))
                                            * glm::rotate(I, glm::radians(s*i), X) 
                                            * top;
                    temp_vertex.normal = glm::rotate(I, glm::radians(s*j), Z) 
                                            * glm::rotate(I, glm::radians(s*i), X) 
                                            * normal_up;
                    temp_vertex.texCoord = glm::vec2(1.0f/(float)iSlices * j * 4, 1.0f/(float)iSlices * i);
                    vertices.push_back(temp_vertex);
                }

            }
            for(int i=0; i<iSlices; i++){
                for(int j=0; j<iSlices; j++){
                    indices.push_back(i*n + j);
                    indices.push_back((i+1)*n + j+1);
                    indices.push_back((i+1)*n + j);

                    indices.push_back(i*n + j);
                    indices.push_back(i*n + j + 1);
                    indices.push_back((i+1)*n + j+1);
                }
            }
        }
};


#endif