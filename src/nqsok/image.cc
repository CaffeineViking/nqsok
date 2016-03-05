#include "image.hh"

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <png.h>

nq::Image::Image(const std::string& file)
                : file {file} {
    std::cout << "\nImage (reading into memory)..." << std::endl;
    std::cout << "Loading '" << file << "'...";
    std::FILE* file_pointer {std::fopen(file.c_str(), "rb")};
    if (!file_pointer) throw Image_error{"Image error (#1) file not found."};

    constexpr std::size_t HEADER_SIZE {8};
    unsigned char header[HEADER_SIZE];
    std::fread(header, sizeof(*header), HEADER_SIZE, file_pointer);
    if (png_sig_cmp(header, 0, HEADER_SIZE)) {
        throw Image_error{"Image error (#2) file doesn't conform to png standard!"};
    }

    png_structp png_ptr {png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr)};
    if (!png_ptr) throw Image_error{"Image error (#3) png struct creation failed."};

    png_infop info_ptr {png_create_info_struct(png_ptr)};
    if (!info_ptr) {
        std::fclose(file_pointer);
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        throw Image_error{"Image error (#4) png info creation failed."};
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        std::fclose(file_pointer);
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        throw Image_error{"Image error (#5) failed to set png jump buffer."};
    }

    png_init_io(png_ptr, file_pointer);
    png_set_sig_bytes(png_ptr, HEADER_SIZE);
    png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_STRIP_16|PNG_TRANSFORM_PACKING|PNG_TRANSFORM_EXPAND, nullptr);
    std::cout << "done" << std::endl;

    png_uint_32 pwidth, pheight;
    int pbit_depth, color_type, interlace_type;
    png_get_IHDR(png_ptr, info_ptr, &pwidth, &pheight, &pbit_depth,
                 &color_type, &interlace_type, nullptr, nullptr);
    width = pwidth; height = pheight, bit_depth = pbit_depth;

    std::cout << "Image loaded of size " << width << "x" << height << " in pixels\n";
    std::cout << "Bit depth of " << bit_depth << " bits on every channel found\n";
    std::cout << "Color type #" << color_type << " found with interlacing type #"
              << interlace_type << std::endl;

    std::size_t row_bytes {png_get_rowbytes(png_ptr, info_ptr)};
    data = static_cast<GLubyte*>(std::malloc(row_bytes * pheight));
    png_bytepp row_pointers {png_get_rows(png_ptr, info_ptr)};
    for (std::size_t y {0}; y < pwidth; ++y) {
        std::memcpy(&data[row_bytes * (pheight - 1 - y)],
                    row_pointers[y], row_bytes);
    }

    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    fclose(file_pointer);
    std::cout << "...done (Image)" << std::endl;
}
