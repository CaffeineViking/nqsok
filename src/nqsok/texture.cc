#include "texture.hh"

#include <iostream>

nq::Texture::Texture(std::vector<GLfloat>& data, GLsizei width,
                     GLsizei height, const Parameters& params)
                     : width {width}, height {height} {
    glGenTextures(1, &handle);
    glBindTexture(GL_TEXTURE_2D, handle);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, width, 0,
                 GL_RGB, GL_FLOAT, data.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, params.minifying_filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, params.magnifying_filter);
    // Might make it so the above filters are more customizable in the future (cube etc...).
}

void nq::Texture::active(Shader& shader, GLenum texture_unit,
                         const std::string& uniform_name) {
    glBindTexture(GL_TEXTURE_2D, handle);
    glActiveTexture(texture_unit); // Very nice, yes!
    shader.uniformi(uniform_name.c_str(), texture_unit);
}
