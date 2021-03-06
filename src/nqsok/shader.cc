#include "shader.hh"

#include <fstream>
#include <iterator>
#include <iostream>

nq::Shader::Shader(const std::string& vertex_shader_path,
                   const std::string& fragment_shader_path)
                   : vertex_shader_file {vertex_shader_path},
                     fragment_shader_file {fragment_shader_path} {
    std::cout << "\nShader (compiling and linking)..." << std::endl;

    GLuint vertex_shader {load_shader(vertex_shader_path, GL_VERTEX_SHADER)};
    GLuint fragment_shader {load_shader(fragment_shader_path, GL_FRAGMENT_SHADER)};
    GLuint shader_program {load_program(vertex_shader, fragment_shader)};
    handle = shader_program; // Oooops. This is important (forgot...)
    glUseProgram(handle); // User needs to be aware of this.

    std::cout << "Linking successful, deleting shaders" << std::endl;
    glDeleteShader(fragment_shader);
    glDeleteShader(vertex_shader);
    std::cout << "...done (Shader)" << std::endl;
}

nq::Shader::~Shader() { glDeleteProgram(handle); }

nq::Shader* nq::Shader::current {nullptr};
bool nq::Shader::is_current() const {
    if (current == this) return true;
    else return false;
}

void nq::Shader::uniformf(const std::string& name, float value) {
    if (!is_current()) use(); // Make sure we are using this...
    GLint location {glGetUniformLocation(handle, name.c_str())};
    glUniform1f(location, value);
}

void nq::Shader::uniformi(const std::string& name, int value) {
    if (!is_current()) use(); // Make sure we are using this...
    GLint location {glGetUniformLocation(handle, name.c_str())};
    glUniform1i(location, value);
}

void nq::Shader::uniform_vector(const std::string& name, const glm::vec3& value) {
    if (!is_current()) use(); // Make sure we are using this...
    GLint location {glGetUniformLocation(handle, name.c_str())};
    glUniform3fv(location, 1, glm::value_ptr(value));
}

void nq::Shader::uniform_matrix(const std::string& name, const glm::mat4& value) {
    if (!is_current()) use(); // Make sure we are using this...
    GLint location {glGetUniformLocation(handle, name.c_str())};
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

GLint nq::Shader::attribute_location(const std::string& name) const {
    return glGetAttribLocation(handle, name.c_str());
}

GLuint nq::Shader::load_shader(const std::string& shader_path, GLenum type) {
    std::ifstream shader_file {shader_path};
    if (!shader_file) throw Shader_error{"Shader error (#1) file not found"};
    std::string shader_source {std::istreambuf_iterator<char>{shader_file}, std::istreambuf_iterator<char>{}};
    const char* shader_csource = shader_source.c_str(); int shader_length = shader_source.length();

    std::cout << shader_path << " (compiling)...";
    GLuint shader {glCreateShader(type)};
    glShaderSource(shader, 1, &shader_csource, &shader_length);
    glCompileShader(shader);

    GLint shader_state {GL_TRUE};
    glGetShaderiv(shader, GL_COMPILE_STATUS, &shader_state);
    if (shader_state == GL_FALSE) {
        GLint log_size {42};
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_size);
        char* log {new char[log_size]};
        glGetShaderInfoLog(shader, log_size, nullptr, log);
        std::string shader_error {"Shader error (#2) "}; shader_error += log; delete log;
        throw Shader_error{shader_error};
    }

    std::cout << "done" << std::endl;
    return shader;
}

GLuint nq::Shader::load_program(GLuint vertex_shader, GLuint fragment_shader) {
    GLuint shader_program {glCreateProgram()};
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);

    GLint shader_state {GL_TRUE};
    glGetProgramiv(shader_program, GL_LINK_STATUS, &shader_state);
    if (shader_state == GL_FALSE) {
        GLint log_size {42};
        glGetProgramiv(shader_program, GL_INFO_LOG_LENGTH, &log_size);
        char* log {new char[log_size]};
        glGetProgramInfoLog(shader_program, log_size, nullptr, log);
        std::string shader_error {"Shader error (#3) "}; shader_error += log; delete log;
        throw Shader_error{shader_error};
    }

    return shader_program;
}
