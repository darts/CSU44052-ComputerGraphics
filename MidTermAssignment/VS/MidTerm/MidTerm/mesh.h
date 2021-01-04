#pragma once

#ifndef MESH_H
#define MESH_H


#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <shader.h>

#include <string>
#include <vector>
using namespace std;

struct Vertex {
    glm::vec3 vertex_position;
    glm::vec3 vertex_normal;
    glm::vec2 texture_coordinates;
    glm::vec3 vertex_tangent;
    glm::vec3 vertex_bitangent;
};

struct Texture {
    unsigned int id;
    string type;
    string path;
};

struct Colour {
    glm::vec3 diffuse = glm::vec3(1.0f, 1.0f, 1.0f); //COLOR_DIFFUSE - Kd
    glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f); //COLOR_SPECULAR - Ks
    glm::vec3 ambient = glm::vec3(1.0f, 1.0f, 1.0f); //COLOR_AMBIENT - Ka
    //COLOR_EMISSIVE
    //COLOR_TRANSPARENT
    //COLOR_REFLECTIVE
    float spec_highlight = 100.0f; //SHININESS Ns
    float optical_density = 1.0f; //REFRACTI Ni
    float opacity = 1.0f; //OPACITY d
};

class Mesh {
public:
    // mesh Data
    vector<Vertex> vertices;
    vector<uint32_t> indices;
    vector<Texture> textures;
    Colour colour_data;
    
    uint32_t VAO;
      
    Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures, Colour colour_data)
    {
        this->vertices = vertices;
        this->indices = indices;
        this->textures = textures;
        this->colour_data = colour_data;

        setupMesh();
    }

    //render the mesh
    void Draw(Shader& shader, bool custom_colour = false, glm::vec3 diff_colour = glm::vec3(0.0f))
    {
        // bind appropriate textures
        unsigned int diffuseNr = 1;
        unsigned int specularNr = 1;
        unsigned int normalNr = 1;
        unsigned int heightNr = 1;
        for (unsigned int i = 0; i < textures.size(); i++)
        {
            glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
            // retrieve texture number (the N in diffuse_textureN)
            string number;
            string name = textures[i].type;
            if (name == "texture_diffuse")
                number = std::to_string(diffuseNr++);
            else if (name == "texture_specular")
                number = std::to_string(specularNr++); // transfer unsigned int to stream
            else if (name == "texture_normal")
                number = std::to_string(normalNr++); // transfer unsigned int to stream
            else if (name == "texture_height")
                number = std::to_string(heightNr++); // transfer unsigned int to stream

            // now set the sampler to the correct texture unit
            glUniform1i(glGetUniformLocation(shader.ID, (name + number).c_str()), i);
            // and finally bind the texture
            glBindTexture(GL_TEXTURE_2D, textures[i].id);
        }

        shader.SetVec3("diff_colour", this->colour_data.diffuse);
        if(custom_colour)
            shader.SetVec3("diff_colour", diff_colour);
        shader.SetVec3("spec_colour", this->colour_data.specular);
        shader.SetVec3("ambient_colour", this->colour_data.ambient);
        shader.SetFloat("opacity", this->colour_data.opacity);

        // draw mesh
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // always good practice to set everything back to defaults once configured.
        glActiveTexture(GL_TEXTURE0);
    }

private:
    unsigned int VBO, EBO;

    void setupMesh()
    {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);
        //add vertices to VBO
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

        // vertex locations
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        // vertex normals
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, vertex_normal));
        // vertex texture coords
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texture_coordinates));
        // vertex tangent
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, vertex_tangent));
        // vertex bitangent
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, vertex_bitangent));

        glBindVertexArray(0);
    }
};
#endif // !MESH_H