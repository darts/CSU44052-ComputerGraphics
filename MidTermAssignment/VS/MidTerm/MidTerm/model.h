#ifndef MODEL_H
#define MODEL_H

#include <glad/glad.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <mesh.h>
#include <shader.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
using namespace std;

class Model
{
public:
    vector<Mesh> model_meshes;
    string directory;

    //blank constructor as placeholder
    Model() {}

    Model(string const& path)
    {
        loadModel(path);
    }
      
    void Draw(Shader& shader)
    {
        for (unsigned int i = 0; i < model_meshes.size(); i++)
            model_meshes[i].Draw(shader);
    }

private:
    void loadModel(string const& path)
    {
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
        //check for errors
        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            cout << "Failed to load model: " << importer.GetErrorString() << endl;
            return;
        }

        //step through the nodes in the object
        processNode(scene->mRootNode, scene);
    }

    //recursively go through the nodes
    void processNode(aiNode* node, const aiScene* scene)
    {
        //Step through each mesh
        for (unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            //pull the data
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            model_meshes.push_back(processMesh(mesh, scene));
        }
        //this is the recursive bit, go through all the children
        for (unsigned int i = 0; i < node->mNumChildren; i++)
        {
            processNode(node->mChildren[i], scene);
        }

    }

    Mesh processMesh(aiMesh* mesh, const aiScene* scene)
    {
        //data to fill
        vector<Vertex> vertices;
        vector<unsigned int> indices;

        //go through all the vertices
        for (unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            Vertex vertex;
            glm::vec3 vector; 

            vector.x = mesh->mVertices[i].x;
            vector.y = mesh->mVertices[i].y;
            vector.z = mesh->mVertices[i].z;
            vertex.vertex_position = vector;
            //normals for shader
            if (mesh->HasNormals())
            {
                vector.x = mesh->mNormals[i].x;
                vector.y = mesh->mNormals[i].y;
                vector.z = mesh->mNormals[i].z;
                vertex.vertex_normal = vector;
            }

            vertices.push_back(vertex);
        }
        //get indices of triangles
        for (unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }

        return Mesh(vertices, indices);
    }
};
#endif
