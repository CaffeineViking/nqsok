#include "texture.hh"

#include <iostream>

nq::Texture::Texture(std::vector<GLfloat>& data, GLsizei width,
                     GLsizei height, const Parameters& params)
                     : width {width}, height {height} {
    std::cout << "\nTexture (upload and config)..." << std::endl;
    std::cout << "Texture of size " << width << "x"  << height << ", occupying "
              << data.size() * sizeof(data[0]) << " bytes found" << std::endl;

    glGenTextures(1, &handle);
    std::cout << "Assigned texture handle id number " << handle << std::endl;

    glBindTexture(GL_TEXTURE_2D, handle);
    std::cout << "Uploading texture...";
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, width, 0,
                 GL_RGB, GL_FLOAT, data.data());
    std::cout << "done" << std::endl;

    std::cout << "Using minifying filter " << params.minifying_filter << std::endl;
    std::cout << "Using magnifying filter " << params.magnifying_filter << std::endl;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, params.minifying_filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, params.magnifying_filter);
    // Might make it so the above filters are more customizable in the future (cube etc...).
    std::cout << "...done (Texture)" << std::endl;
}

void nq::Texture::active(Shader& shader, GLenum texture_unit,
                         const std::string& uniform_name) {
    glActiveTexture(texture_unit);
    glBindTexture(GL_TEXTURE_2D, handle);
    shader.uniformi(uniform_name.c_str(), texture_unit);
}
