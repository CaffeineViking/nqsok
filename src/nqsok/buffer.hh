#ifndef NQSOK_BUFFER_HH
#define NQSOK_BUFFER_HH

#include <GL/glew.h>
#include <iostream>
#include <vector>

namespace nq {
    template<typename T>
    class Buffer final {};

    template<>
    class Buffer<GLfloat> final {
    public:
        ~Buffer() { glDeleteBuffers(1, &handle); } // Deallocate memory on GPU.
        std::size_t size() const { return elements; }
        Buffer(const std::vector<GLfloat>& data, GLenum usage) {
            std::cout << "\nBuffer (uploading)..." << std::endl;
            std::cout << "Total of " << data.size() << " elements of type GLfloat, "
                      << data.size() * sizeof(GLfloat) << " bytes to upload" << std::endl;

            elements = data.size();
            glGenBuffers(1, &handle);
            std::cout << "Assigned buffer GPU handle id number " << handle << std::endl;

            glBindBuffer(GL_ARRAY_BUFFER, handle);
            std::cout << "Uploading buffer (type array)...";
            glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(GLfloat), data.data(), usage);
            std::cout << "done" << std::endl;
            std::cout << "...done (Buffer)" << std::endl;
        }

    private:
        friend class Mesh;
        std::size_t elements;
        GLuint handle;
    };

    template<>
    class Buffer<GLuint> final {
    public:
        ~Buffer() { glDeleteBuffers(1, &handle); } // Deallocate memory on GPU.
        std::size_t size() const { return elements; }
        Buffer(const std::vector<GLuint>& data, GLenum usage) {
            std::cout << "\nBuffer (uploading)..." << std::endl;
            std::cout << "Total of " << data.size() << " elements of type GLuint, "
                      << data.size() * sizeof(GLuint) << " bytes to upload" << std::endl;

            elements = data.size();
            glGenBuffers(1, &handle);
            std::cout << "Assigned buffer GPU handle id number " << handle << std::endl;

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle);
            std::cout << "Uploading buffer (type element)...";
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.size() * sizeof(GLuint), data.data(), usage);
            std::cout << "done" << std::endl;
            std::cout << "...done (Buffer)" << std::endl;
        }

    private:
        friend class Mesh;
        std::size_t elements;
        GLuint handle;
    };
}

#endif
