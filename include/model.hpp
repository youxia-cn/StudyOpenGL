#ifndef __MODEL_HPP__
#define __MODEL_HPP__

#include "mesh.hpp"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

struct TextureAndPath{
    std::string path;
    GLuint texture_id;
};

class Model
{
private:
    std::vector<Mesh> meshes;
    Shader shader;
    std::string dir;
    std::vector<TextureAndPath> loaded_textures;

public:
    void loaModel(std::string dir, std::string filename)
    {
        Assimp::Importer importer;
        const aiScene *scene = importer.ReadFile(dir+filename,
                                                 aiProcess_Triangulate |
                                                     aiProcess_GenNormals |
                                                     aiProcess_FlipUVs);
        this->dir = dir;
        processNode(scene->mRootNode, scene);
    }

    void processNode(aiNode *node, const aiScene *scene)
    {
        for (int i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
            Vertex tempVertex;
            std::vector<Vertex> tempVertices;
            std::vector<GLuint> tempIndices;
            //先读取顶点信息
            for (int j = 0; j < mesh->mNumVertices; j++)
            {
                if (mesh->HasPositions())
                {
                    tempVertex.position.x = mesh->mVertices[j].x;
                    tempVertex.position.y = mesh->mVertices[j].y;
                    tempVertex.position.z = mesh->mVertices[j].z;
                    tempVertex.position.w = 1.0f;
                }
                if (mesh->HasNormals())
                {
                    tempVertex.normal.x = mesh->mNormals[j].x;
                    tempVertex.normal.y = mesh->mNormals[j].y;
                    tempVertex.normal.z = mesh->mNormals[j].z;
                }
                if (mesh->HasTextureCoords(0))
                {
                    tempVertex.texCoord.x = mesh->mTextureCoords[0][j].x;
                    tempVertex.texCoord.y = mesh->mTextureCoords[0][j].y;
                }

                tempVertices.push_back(tempVertex);
            }
            //再读取索引信息
            for (int i = 0; i < mesh->mNumFaces; i++)
            {
                for (int j = 0; j < mesh->mFaces[i].mNumIndices; j++)
                {
                    tempIndices.push_back(mesh->mFaces[i].mIndices[j]);
                }
            }
            Mesh tempMesh;

            tempMesh.setVertices(std::move(tempVertices));
            tempMesh.setIndices(std::move(tempIndices));

            tempMesh.setShader(shader);
            //读取材质信息
            aiMaterial *meterial;
            if (scene->HasMaterials())
            {
                meterial = scene->mMaterials[mesh->mMaterialIndex];
                aiColor3D tempColor;
                //aiGetMaterialColor(meterial, AI_MATKEY_COLOR_AMBIENT, &tempColor);
                meterial->Get(AI_MATKEY_COLOR_AMBIENT, tempColor);
                tempMesh.setKa(glm::vec3(tempColor.r, tempColor.g, tempColor.b));
                meterial->Get(AI_MATKEY_COLOR_DIFFUSE, tempColor);
                tempMesh.setKd(glm::vec3(tempColor.r, tempColor.g, tempColor.b));
                meterial->Get(AI_MATKEY_COLOR_SPECULAR, tempColor);
                tempMesh.setKs(glm::vec3(tempColor.r, tempColor.g, tempColor.b));
                float Ns;
                aiGetMaterialFloat(meterial, AI_MATKEY_SHININESS, &Ns);
                tempMesh.setNs(Ns);
                aiString texture_filename;
                meterial->GetTexture(aiTextureType_DIFFUSE, 0, &texture_filename);
                if(texture_filename.length != 0){
                    std::string path = dir+std::string(texture_filename.C_Str());
                    bool loaded = false;
                    for(auto i=loaded_textures.begin(); i!=loaded_textures.end();++i){
                        if(path == i->path){
                            tempMesh.setMapKd(i->texture_id);
                            loaded = true;
                            break;
                        }
                    }
                    if(!loaded){
                        TextureAndPath temp;
                        temp.path = path;
                        temp.texture_id = loadTexture(path);
                        loaded_textures.push_back(temp);
                        tempMesh.setMapKd(temp.texture_id);
                    }
                }
                meterial->GetTexture(aiTextureType_SPECULAR, 0, &texture_filename);
                if(texture_filename.length != 0){
                    std::string path = dir+std::string(texture_filename.C_Str());
                    bool loaded = false;
                    for(auto i=loaded_textures.begin(); i!=loaded_textures.end();++i){
                        if(path == i->path){
                            tempMesh.setMapKs(i->texture_id);
                            loaded = true;
                            break;
                        }
                    }
                    if(!loaded){
                        TextureAndPath temp;
                        temp.path = path;
                        temp.texture_id = loadTexture(path);
                        loaded_textures.push_back(temp);
                        tempMesh.setMapKs(temp.texture_id);
                    }
                }
                
            }
            tempMesh.setup();
            this->meshes.push_back(std::move(tempMesh));
        }

        if (node->mNumChildren != 0)
        {
            for (int k = 0; k < node->mNumChildren; k++)
            {
                processNode(node->mChildren[k], scene);
            }
        }
        return;
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

    void setShader(Shader shader)
    {
        this->shader = shader;
    }

    void render()
    {
        for (auto i = meshes.begin(); i != meshes.end(); i++)
        {
            i->render();
        }
    }

    void setMatrix(glm::mat4 model_matrix, glm::mat4 view_matrix, glm::mat4 projection_matrix)
    {
        for (auto i = meshes.begin(); i != meshes.end(); i++)
        {
            i->setMatrix(model_matrix, view_matrix, projection_matrix);
        }
    }

    void setLightPosition(glm::vec4 lightPosition)
    {
        for (auto i = meshes.begin(); i != meshes.end(); i++)
        {
            i->setLightPosition(lightPosition);
        }
    }

    void setCameraPosition(glm::vec4 cameraPosition)
    {
        for (auto i = meshes.begin(); i != meshes.end(); i++)
        {
            i->setCameraPosition(cameraPosition);
        }
    }

    void setLightColor(glm::vec3 lightColor)
    {
        for (auto i = meshes.begin(); i != meshes.end(); i++)
        {
            i->setLightColor(lightColor);
        }
    }

    void setTexCoordScaleFactor(GLfloat factor)
    {
        for (auto i = meshes.begin(); i != meshes.end(); i++)
        {
            i->setTexCoordScaleFactor(factor);
        }
    }


    void setMapKd(std::string mapKd)
    {
        for (auto i = meshes.begin(); i != meshes.end(); i++)
        {
            i->setMapKd(mapKd);
        }
    }
};

#endif