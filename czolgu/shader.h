#ifndef SHADER_H
#define SHADER_H

#include "stdio.h"
#include <glad\glad.h>

class Shader {
private:
	GLuint ID; //Uchwyt reprezentuj�cy program cieniujacy
	GLuint vertexShader; //Uchwyt reprezentuj�cy vertex shader
	GLuint geometryShader; //Uchwyt reprezentuj�cy geometry shader
	GLuint fragmentShader; //Uchwyt reprezentuj�cy fragment shader
	char* readFile(const char* fileName); //metoda wczytuj�ca plik tekstowy do tablicy znak�w
	GLuint loadShader(GLenum shaderType, const char* fileName); //Metoda wczytuje i kompiluje shader, a nast�pnie zwraca jego uchwyt
public:

	Shader(const char* vertexShaderFile, const char* fragmentShaderFile, const char* geometryShaderFile);
	~Shader();
	void use(); //W��cza wykorzystywanie programu cieniuj�cego
	GLuint u(const char* variableName); //Pobiera numer slotu zwi�zanego z dan� zmienn� jednorodn�
	GLuint a(const char* variableName); //Pobiera numer slotu zwi�zanego z danym atrybutem
	GLuint getID();
    void setBool(const std::string& name, bool value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
    }
    // ------------------------------------------------------------------------
    void setInt(const std::string& name, int value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }
    // ------------------------------------------------------------------------
    void setFloat(const std::string& name, float value) const
    {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }
    // ------------------------------------------------------------------------
    void setVec2(const std::string& name, const glm::vec2& value) const
    {
        glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
    }
    void setVec2(const std::string& name, float x, float y) const
    {
        glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
    }
    // ------------------------------------------------------------------------
    void setVec3(const std::string& name, const glm::vec3& value) const
    {
        glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
    }
    void setVec3(const std::string& name, float x, float y, float z) const
    {
        glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
    }
    // ------------------------------------------------------------------------
    void setVec4(const std::string& name, const glm::vec4& value) const
    {
        glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
    }
    void setVec4(const std::string& name, float x, float y, float z, float w) const
    {
        glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
    }
    // ------------------------------------------------------------------------
    void setMat2(const std::string& name, const glm::mat2& mat) const
    {
        glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    // ------------------------------------------------------------------------
    void setMat3(const std::string& name, const glm::mat3& mat) const
    {
        glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    // ------------------------------------------------------------------------
    void setMat4(const std::string& name, const glm::mat4& mat) const
    {
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
};

#endif