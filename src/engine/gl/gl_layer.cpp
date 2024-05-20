#include "gl_layer.hpp"

namespace sm 
{
    namespace gl_layer
    {
        // VBO

        vbo::vbo()
        {
            generate();
            bind();
        }

        void vbo::generate()
        { glGenBuffers(1, &id); }

        void vbo::bind()
        { glBindBuffer(GL_ARRAY_BUFFER, id); }

        void vbo::detach()
        { glBindBuffer(GL_ARRAY_BUFFER, 0); }

        void vbo::send_data(GLsizeiptr size, const void* data, GLenum usage)
        { glBufferData(GL_ARRAY_BUFFER, size, data, usage); }

        void vbo::free()
        { glDeleteBuffers(1, &id); }


        // VAO 

        vao::vao()
        { 
            generate();
            bind();
        }

        void vao::generate()
        { glGenVertexArrays(1, &id); }

        void vao::bind()
        { glBindVertexArray(id); }

        void vao::detach()
        { glBindVertexArray(0); }

        void vao::free()
        { glDeleteVertexArrays(1, &id); }

        // EBO

        ebo::ebo()
        {
            generate();
            bind();
        }

        void ebo::generate()
        { glGenBuffers(1, &id); }

        void ebo::bind()
        { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id); }

        void ebo::detach()
        { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }

         void ebo::send_data(GLsizeiptr size, const void* data, GLenum usage)
        { glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, usage); }

        void ebo::free()
        { glDeleteBuffers(1, &id); }


        void enable_attrib_ptr(GLuint index, GLint size, GLenum type, GLsizei stride, const void* ptr)
        { glVertexAttribPointer(index, size, type, GL_FALSE, stride, ptr); }

        void enable_attrib_array(GLint index)
        { glEnableVertexAttribArray(index); }

        void disable_attrib_array(GLint index)
        { glDisableVertexAttribArray(index); }

        void draw_indexed(GLenum mode, GLsizei count, GLenum type, const void* indices)
        { glDrawElements(mode, count, type, indices); }
    }
}

