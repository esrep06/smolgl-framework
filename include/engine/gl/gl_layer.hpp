// Just some OpenGL boilerplate code
//
// Creates, binds and detaches vaos, vbos and ebos,
// NOTE: WHEN CREATING ALL THREE ELEMENTS TO DRAW AN OBJECT, MAKE SURE TO GENERATE AND BIND VAOS FIRST BEFORE ANYTHING ELSE 

#pragma once 

#include <inttypes.h>

#define GLEW_STATIC
#include <glew/include/GL/glew.h>

namespace sm 
{
    namespace gl_layer
    {
        struct vao 
        {  
            vao();

            uint32_t id;

            void generate();
            void bind();
            void detach();
            void free();
        };

        struct vbo
        {  
            vbo();

            uint32_t id;

            void generate();
            void bind();
            void detach();
            void send_data(GLsizeiptr size, const void* data, GLenum usage);
            void update_data(GLintptr offset, GLsizeiptr size, const void* data);
            void free();
        };

        struct ebo 
        {  
            ebo();

            uint32_t id;

            void generate();
            void bind();
            void detach();
            void send_data(GLsizeiptr size, const void* data, GLenum usage);
            void update_data(GLintptr offset, GLsizeiptr size, const void* data);
            void free();
        };
        
        void enable_attrib_ptr(GLuint index, GLint size, GLenum type, GLsizei stride, const void* ptr);
        void enable_attrib_array(GLint index);
        void disable_attrib_array(GLint index);

        void draw_indexed(GLenum mode, GLsizei count, GLenum type, const void* indices);
        void draw_simple(GLenum mode, GLint first, GLsizei count);
    };
}

