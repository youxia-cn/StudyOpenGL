#ifndef __MODEL_HPP__
#define __MODEL_HPP__

#include "mesh.hpp"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

class Model
{
private:
    std::vector<Mesh> meshes;
    Shader shader;
    std::string dir;

public:
    void loaModel(std::string dir, std::string filename)
    {
        Assimp::Importer importer;
        const aiScene *scene = importer.ReadFile(dir+filename,
                                                 aiProcess_Triangulate |
                                                     aiProcess_GenNormals |
                                                     aiProcess_OptimizeMeshes);
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
                aiColor4D tempColor;
                aiGetMaterialColor(meterial, AI_MATKEY_COLOR_AMBIENT, &tempColor);
                tempMesh.setKa(glm::vec3(tempColor.r, tempColor.g, tempColor.b));
                aiGetMaterialColor(meterial, AI_MATKEY_COLOR_DIFFUSE, &tempColor);
                tempMesh.setKd(glm::vec3(tempColor.r, tempColor.g, tempColor.b));
                aiGetMaterialColor(meterial, AI_MATKEY_COLOR_SPECULAR, &tempColor);
                tempMesh.setKs(glm::vec3(tempColor.r, tempColor.g, tempColor.b));
                float Ns;
                aiGetMaterialFloat(meterial, AI_MATKEY_SHININESS, &Ns);
                tempMesh.setNs(Ns);
                aiString texture_filename;
                meterial->GetTexture(aiTextureType_DIFFUSE, 0, &texture_filename);
                if(texture_filename.length != 0){
                    tempMesh.setMapKd(dir+std::string(texture_filename.C_Str()));
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

    void setMapKd(std::string mapKd)
    {
        for (auto i = meshes.begin(); i != meshes.end(); i++)
        {
            i->setMapKd(mapKd);
        }
    }
};

#endif