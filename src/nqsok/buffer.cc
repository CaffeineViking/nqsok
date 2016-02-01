#include "buffer.hh"

template<typename T>
nq::Buffer<T>::Buffer(GLenum type, const std::vector<T>& data, GLenum usage) {
    elements = data.size();
    glGenBuffers(1, &handle);
    glBindBuffer(type, handle);
    glBufferData(type, data.size() * sizeof(T), data.data(), usage);
}

template<typename T>
nq::Buffer<T>::~Buffer() { glDeleteBuffers(1, &handle); }
