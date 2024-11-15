#include "Shader.hpp"


Shader::Shader(const char* vertexPath, const char* fragmentPath)
{

    // 1. retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    // ensure ifstream objects can throw exceptions:
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        // open files
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // close file handlers
        vShaderFile.close();
        fShaderFile.close();
        // convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }

    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    uint32 vertex, fragment;
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");
    // fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");
    // shader Program
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    checkCompileErrors(ID, "PROGRAM");
    // delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertex);
    glDeleteShader(fragment);

}

void Shader::use() const
{
    glUseProgram(ID);
}

void Shader::setBool(const std::string& name, bool value) const
{
    GLint location = getLocation(name);
    glUniform1i(location, (int)value);
}
void Shader::setInt(const std::string& name, int value) const
{
    GLint location = getLocation(name);
    glUniform1i(location, value);
}
void Shader::setFloat(const std::string& name, float value) const
{
    GLint location = getLocation(name);
    glUniform1f(location, value);
}

void Shader::setVec2(const std::string& name, const glm::vec2& value) const
{
    GLint location = getLocation(name);
    glUniform2fv(location, 1, &value[0]);
}
void Shader::setVec2(const std::string& name, float x, float y) const
{
    GLint location = getLocation(name);
    glUniform2f(location, x, y);
}

// ------------------------------------------------------------------------

void Shader::setVec3(const std::string& name, const glm::vec3& value) const
{
    GLint location = getLocation(name);
    glUniform3fv(location, 1, &value[0]);
}
void Shader::setVec3(const std::string& name, float x, float y, float z) const
{
    GLint location = getLocation(name);
    glUniform3f(location, x, y, z);
}

// ------------------------------------------------------------------------

void Shader::setVec4(const std::string& name, const glm::vec4& value) const
{
    GLint location = getLocation(name);
    glUniform4fv(location, 1, &value[0]);
}
void Shader::setVec4(const std::string& name, float x, float y, float z, float w) const
{
    GLint location = getLocation(name);
    glUniform4f(location, x, y, z, w);
}

// ------------------------------------------------------------------------

void Shader::setMat2(const std::string& name, const glm::mat2& mat) const
{
    GLint location = getLocation(name);
    glUniformMatrix2fv(location, 1, GL_FALSE, &mat[0][0]);
}

void Shader::setMat3(const std::string& name, const glm::mat3& mat) const
{
    GLint location = getLocation(name);
    glUniformMatrix3fv(location, 1, GL_FALSE, &mat[0][0]);
}

void Shader::setMat4(const std::string& name, const glm::mat4& mat) const
{
    GLint location = getLocation(name);
    glUniformMatrix4fv(location, 1, GL_FALSE, &mat[0][0]);
}

GLint Shader::getLocation(const std::string& name) const
{
    if (m_locationCache.find(name) != m_locationCache.end())
    {
        return m_locationCache[name];
    }

    GLint location = glGetUniformLocation(ID, name.c_str());
    m_locationCache[name] = location;
    return location;
}

void Shader::checkCompileErrors(GLuint shader, std::string type)
{
    GLint success;
    GLchar infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}