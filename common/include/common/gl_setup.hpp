#ifndef GL_SETUP_HPP
#define GL_SETUP_HPP

#include <glad/glad.h>
#include <cassert>
#include <string>

#include <glsl_algo/init.h>

int init_window_and_gl_context(glsl_algo_gl_context *gl);
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
        data[i] = bufferData[i];
    }
    glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

std::string get_program_binary(GLuint program);


#endif // GL_SETUP_HPP