#ifndef GL_SETUP_HPP
#define GL_SETUP_HPP

#include <glad/glad.h>

#include <cassert>

int init_window_and_gl_context();
void destroy_window_and_gl_context();

GLuint create_ssbo(unsigned n, void *data = NULL);

template<typename T>
void get_ssbo_data(GLuint ssbo, unsigned n, T *data)
{
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
    T *bufferData = static_cast<T*>(glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY));
    assert(bufferData != NULL);
    for (unsigned i = 0u; i < n; ++i)
    {
        *data = *bufferData;
        ++bufferData;
    }
    glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}


#endif // GL_SETUP_HPP