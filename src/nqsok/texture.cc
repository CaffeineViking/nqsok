#include "texture.hh"

#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <png.h>

nq::Texture::Texture(const std::string& file,
                     const Parameters& params) {
    std::cout << "\nTexture (upload and config)..." << std::endl;
    std::FILE* file_pointer {std::fopen(file.c_str(), "rb")};
    if (!file_pointer) throw Texture_error{"Texture error (#1) file not found."};

    constexpr std::size_t HEADER_SIZE {8};
    unsigned char header[HEADER_SIZE];
    std::fread(header, sizeof(*header), HEADER_SIZE, file_pointer);
    if (png_sig_cmp(header, 0, HEADER_SIZE)) {
        throw Texture_error{"Texture error (#2) file doesn't conform to png standard!"};
    }

    png_structp png_ptr {png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr)};
    if (!png_ptr) throw Texture_error{"Texture error (#3) png struct creation failed."};

    png_infop info_ptr {png_create_info_struct(png_ptr)};
    if (!info_ptr) {
        std::fclose(file_pointer);
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        throw Texture_error{"Texture error (#4) png info creation failed."};
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        std::fclose(file_pointer);
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        throw Texture_error{"Texture error (#5) failed to set png jump buffer."};
    }

    png_init_io(png_ptr, file_pointer);
    png_set_sig_bytes(png_ptr, HEADER_SIZE);

    png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_STRIP_16|PNG_TRANSFORM_PACKING|PNG_TRANSFORM_EXPAND, nullptr);
    png_uint_32 width, height;
    int bit_depth, color_type, interlace_type;
    png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type, &interlace_type, nullptr, nullptr);
    this->width = width;
    this->height = height;

    std::cout << "Texture of size " << width << "x"  << height << ", bit depth of "
              << bit_depth << " with color type #" << color_type << " found" << std::endl;

    std::size_t row_bytes {png_get_rowbytes(png_ptr, info_ptr)};
    GLubyte* pixel_data = static_cast<GLubyte*>(std::malloc(row_bytes * height));
    png_bytepp row_pointers {png_get_rows(png_ptr, info_ptr)};
    for (std::size_t y {0}; y < height; ++y) {
        std::memcpy(&pixel_data[row_bytes * (height - 1 - y)],
                    row_pointers[y], row_bytes);
    }

    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    fclose(file_pointer);

    glGenTextures(1, &handle);
    std::cout << "Assigned texture handle id number " << handle << std::endl;

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glBindTexture(GL_TEXTURE_2D, handle);
    std::cout << "Uploading texture...";
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, width, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, pixel_data);
    std::cout << "done" << std::endl;
    free(pixel_data);

    std::cout << "Minifying filter " << params.minifying_filter << std::endl;
    std::cout << "Magnifying filter " << params.magnifying_filter << std::endl;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, params.minifying_filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, params.magnifying_filter);
    // Might make it so the above filters are more customizable in the future (cube etc...).
    std::cout << "...done (Texture)" << std::endl;
}

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

    std::cout << "Minifying filter " << params.minifying_filter << std::endl;
    std::cout << "Magnifying filter " << params.magnifying_filter << std::endl;
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
