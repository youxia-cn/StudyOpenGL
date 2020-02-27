#ifndef __MESH_H__
#define __MESH_H__

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <string>
#include <GL/glew.h>
#include "shader.hpp"

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
        glm::vec3 Ka;
        glm::vec3 Kd;
        glm::vec3 Ks;
        std::string mapKa;
        std::string mapKd;
        std::string mapKs;
        GLfloat Ns;
        //把数据传递到Shader需要的一些id
        GLuint VAO, VBO, EBO, texKa, texKd, texKs;
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
            glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
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

        void setLightPosition(glm::vec4 lightPosition){
            shader.setCurrent();
            shader.setVec4("lightPosition", lightPosition);
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
                    temp_vertex.texCoord = glm::vec2(1.0/(float)n * j, 1.0f - 1.0f/(float)n * i);
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
            int n = iSlices;
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
                    temp_vertex.texCoord = glm::vec2(1.0/(float)n * j, 1.0f/(float)m * i);
                    vertices.push_back(temp_vertex);
                }
            }
            
            for(int i=0; i<m-1; i++){
                for(int j=0; j<n; j++){
                    indices.push_back(i*n + j);
                    indices.push_back((i+1)*n + j);
                    indices.push_back((i+1)*n + (j+1)%n);

                    indices.push_back(i*n + j);
                    indices.push_back((i+1)*n + (j+1)%n);
                    indices.push_back(i*n + (j + 1)%n);                   
                }
            }
        }
};

class Torus: public Mesh{
    public:
        void generateMesh(int iSlices){
            int n = iSlices + 1;
            float s = 360.0f/(float)iSlices;
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
                    temp_vertex.texCoord = glm::vec2(1.0/(float)n * j * 4, 1.0f/(float)n * i);
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