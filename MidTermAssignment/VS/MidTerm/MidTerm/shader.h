#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
    uint32_t ID;

    Shader(const char* vertex_shader_path, const char* fragment_shader_path)
    {
        //load shaders from file
        std::string loaded_vertex_shader;
        std::string loaded_fragment_shader;
        std::ifstream vertex_shader_file_obj;
        std::ifstream fragment_shader_file_obj;
        
        vertex_shader_file_obj.open(vertex_shader_path);
        fragment_shader_file_obj.open(fragment_shader_path);
        std::stringstream vertex_shader_bitstream, fragment_shader_bitstream;

        vertex_shader_bitstream << vertex_shader_file_obj.rdbuf();
        fragment_shader_bitstream << fragment_shader_file_obj.rdbuf();
  
        vertex_shader_file_obj.close();
        fragment_shader_file_obj.close();

        loaded_vertex_shader = vertex_shader_bitstream.str();
        loaded_fragment_shader = fragment_shader_bitstream.str();

        //do this bc the function needs a const as input
        const char* vertex_shader_string = loaded_vertex_shader.c_str();
        const char* fragment_shader_string = loaded_fragment_shader.c_str();

        unsigned int vertex, fragment;
        ID = glCreateProgram();

        //load vertex shader
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vertex_shader_string, NULL);
        glCompileShader(vertex);  
        glAttachShader(ID, vertex);
        glDeleteShader(vertex);

        //load fragment shader
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fragment_shader_string, NULL);
        glCompileShader(fragment);        
        glAttachShader(ID, fragment);
        glDeleteShader(fragment);

        glLinkProgram(ID);
    }

    void use()
    {
        glUseProgram(ID);
    }
    
    void SetFloat(const std::string& name, float value) const
    {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }
    void SetVec3(const std::string& name, const glm::vec3& value) const
    {
        glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
    }
    void SetMat4(const std::string& name, const glm::mat4& mat) const
    {
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
};
#endif
