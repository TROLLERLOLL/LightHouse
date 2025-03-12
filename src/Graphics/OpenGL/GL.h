#pragma once
#ifdef GL
#include <Graphics/Texture.h>
#include <cstdlib>

#ifdef WIN32
#ifndef _WIN64
#define APIENTRY __stdcall
#endif
#endif
#ifndef APIENTRY
#define APIENTRY
#endif

#ifdef _WIN64
typedef signed long long int khronos_intptr_t;
typedef signed long long int khronos_ssize_t;
#else
typedef signed long int khronos_intptr_t;
typedef signed long int khronos_ssize_t;
#endif

typedef signed char	khronos_uint8_t;

typedef unsigned int GLuint;
typedef int GLint;
typedef float GLfloat;
typedef int GLsizei;
typedef unsigned int GLenum;
typedef unsigned int GLbitfield;
typedef char GLchar;
typedef khronos_ssize_t Glsizeiptr;
typedef khronos_intptr_t GLintptr;
typedef unsigned char GLboolean;
typedef khronos_uint8_t GLubyte;

//OpenGL 1.0
typedef void (APIENTRY* glViewport_type)(GLint x, GLint y, GLint w, GLint h);
extern glViewport_type glViewport;

typedef void (APIENTRY* glClearColor_type)(GLfloat r, GLfloat g, GLfloat b, GLfloat a);
extern glClearColor_type glClearColor;

typedef void (APIENTRY* glClear_type)(GLbitfield mask);
extern glClear_type glClear;

typedef GLubyte* (APIENTRY* glGetString_type)(GLenum name);
extern glGetString_type glGetString;

typedef void (APIENTRY* glEnable_type)(GLenum cap);
extern glEnable_type glEnable;

typedef void (APIENTRY* glDisable_type)(GLenum cap);
extern glDisable_type glDisable;

typedef void (APIENTRY* glGetIntegerv_type)(GLenum name, GLint* data);
extern glGetIntegerv_type glGetIntegerv;

typedef void (APIENTRY* glGenTextures_type)(GLsizei n, GLuint* ids);
extern glGenTextures_type glGenTextures;

typedef void (APIENTRY* glDeleteTextures_type)(GLsizei n, GLuint* textures);
extern glDeleteTextures_type glDeleteTextures;

typedef void (APIENTRY* glBindTexture_type)(GLenum target, GLuint texture);
extern glBindTexture_type glBindTexture;

typedef void (APIENTRY* glTexImage2D_type)(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void* data);
extern glTexImage2D_type glTexImage2D;

typedef void (APIENTRY* glTexParameteri_type)(GLenum target, GLenum pname, GLint param);
extern glTexParameteri_type glTexParameteri;

typedef void (APIENTRY* glBlendFunc_type)(GLenum sfactor, GLenum dfactor);
extern glBlendFunc_type glBlendFunc;

typedef void (APIENTRY* glPolygonMode_type)(GLenum face, GLenum mode);
extern glPolygonMode_type glPolygonMode;

//OpenGL 2.0
typedef GLuint(APIENTRY* glCreateProgram_type)();
extern glCreateProgram_type glCreateProgram;

typedef void (APIENTRY* glAttachShader_type)(GLuint program, GLuint shader);
extern glAttachShader_type glAttachShader;

typedef void (APIENTRY* glLinkProgram_type)(GLuint program);
extern glLinkProgram_type glLinkProgram;

typedef void (APIENTRY* glDeleteShader_type)(GLuint shader);
extern glDeleteShader_type glDeleteShader;

typedef void (APIENTRY* glUseProgram_type)(GLuint program);
extern glUseProgram_type glUseProgram;

typedef GLint(APIENTRY* glGetUniformLocation_type)(GLuint program, const GLchar* name);
extern glGetUniformLocation_type glGetUniformLocation;

typedef void (APIENTRY* glUniform1i_type)(GLint location, GLint value);
extern glUniform1i_type glUniform1i;

typedef void (APIENTRY* glUniform1f_type)(GLint location, GLfloat value);
extern glUniform1f_type glUniform1f;

typedef void (APIENTRY* glUniform2f_type)(GLint location, GLfloat value1, GLfloat value2);
extern glUniform2f_type glUniform2f;

typedef unsigned int (APIENTRY* glCreateShader_type)(GLenum shaderType);
extern glCreateShader_type glCreateShader;

typedef void (APIENTRY* glShaderSource_type)(GLuint shader, GLsizei count, const GLchar** string, const GLint* length);
extern glShaderSource_type glShaderSource;

typedef void (APIENTRY* glCompileShader_type)(GLuint shader);
extern glCompileShader_type glCompileShader;

typedef void (APIENTRY* glGetShaderiv_type)(GLuint shader, GLenum pname, GLint* params);
extern glGetShaderiv_type glGetShaderiv;

typedef void (APIENTRY* glGetShaderInfoLog_type)(GLuint shader, GLsizei maxLength, GLsizei* length, GLchar* infolog);
extern glGetShaderInfoLog_type glGetShaderInfoLog;

typedef void (APIENTRY* glDeleteProgram_type)(GLuint program);
extern glDeleteProgram_type glDeleteProgram;

typedef void (APIENTRY* glUniformMatrix4fv_type)(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
extern glUniformMatrix4fv_type glUniformMatrix4fv;

typedef void (APIENTRY* glGenBuffers_type)(GLsizei n, GLuint* buffers);
extern glGenBuffers_type glGenBuffers;

typedef void (APIENTRY* glBindBuffer_type)(GLenum target, GLuint buffer);
extern glBindBuffer_type glBindBuffer;

typedef void (APIENTRY* glBufferData_type)(GLenum target, Glsizeiptr size, const void* data, GLenum usage);
extern glBufferData_type glBufferData;

typedef void (APIENTRY* glEnableVertexAttribArray_type)(GLuint index);
extern glEnableVertexAttribArray_type glEnableVertexAttribArray;

typedef void (APIENTRY* glDisableVertexAttribArray_type)(GLuint index);
extern glDisableVertexAttribArray_type glDisableVertexAttribArray;

typedef void (APIENTRY* glVertexAttribPointer_type)(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer);
extern glVertexAttribPointer_type glVertexAttribPointer;

typedef void (APIENTRY* glDrawArrays_type)(GLenum mode, GLint first, GLsizei count);
extern glDrawArrays_type glDrawArrays;

typedef void (APIENTRY* glDrawElements_type)(GLenum mode, GLsizei count, GLenum type, const void* indices);
extern glDrawElements_type glDrawElements;

typedef void (APIENTRY* glActiveTexture_type)(GLenum texture);
extern glActiveTexture_type glActiveTexture;

typedef void (APIENTRY* glDeleteBuffers_type)(GLsizei n, GLuint* buffers);
extern glDeleteBuffers_type glDeleteBuffers;

typedef void* (APIENTRY* glMapBuffer_type)(GLenum target, GLenum access);
extern glMapBuffer_type glMapBuffer;

typedef void (APIENTRY* glUnmapBuffer_type)(GLenum target);
extern glUnmapBuffer_type glUnmapBuffer;

//OpenGL 3.0
typedef void (APIENTRY* glGenFramebuffers_type)(GLsizei n, GLuint* ids);
extern glGenFramebuffers_type glGenFramebuffers;

typedef void (APIENTRY* glBindFramebuffer_type)(GLenum target, GLuint framebuffer);
extern glBindFramebuffer_type glBindFramebuffer;

typedef void (APIENTRY* glDeleteFramebuffers_type)(GLsizei n, GLuint* ids);
extern glDeleteFramebuffers_type glDeleteFramebuffers;

typedef void (APIENTRY* glGenVertexArrays_type)(GLsizei n, GLuint* arrays);
extern glGenVertexArrays_type glGenVertexArrays;

typedef void (APIENTRY* glBindVertexArray_type)(GLuint array);
extern glBindVertexArray_type glBindVertexArray;

typedef void (APIENTRY* glGenerateMipmap_type)(GLenum target);
extern glGenerateMipmap_type glGenerateMipmap;

typedef void (APIENTRY* glFramebufferTexture2D_type)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
extern glFramebufferTexture2D_type glFramebufferTexture2D;

typedef void (APIENTRY* glDeleteVertexArrays_type)(GLsizei, GLuint* arrays);
extern glDeleteVertexArrays_type glDeleteVertexArrays;

typedef void* (APIENTRY* glMapBufferRange_type)(GLenum target, GLintptr offset, Glsizeiptr length, GLbitfield access);
extern glMapBufferRange_type glMapBufferRange;

//OpenGL 3.1
typedef void (APIENTRY* glDrawElementsInstanced_type)(GLenum mode, GLsizei count, GLenum type, const void* indices, GLsizei instancecount);
extern glDrawElementsInstanced_type glDrawElementsInstanced;

//OpenGL 3.3
typedef void (APIENTRY* glVertexAttribDivisor_type)(GLuint index, GLuint divisor);
extern glVertexAttribDivisor_type glVertexAttribDivisor;



#define GL_FRAMEBUFFER 0x8D40
#define GL_VERTEX_SHADER 0x8B31
#define GL_FRAGMENT_SHADER 0x8B30

#define GL_DELETE_STATUS 0x8B80
#define GL_COMPILE_STATUS 0x8B81
#define GL_LINK_STATUS 0x8B82
#define GL_INFO_LOG_LENGTH 0x8B84

#define GL_DEPTH_BUFFER_BIT 0x00000100
#define GL_STENCIL_BUFFER_BIT 0x00000400
#define GL_COLOR_BUFFER_BIT 0x00004000

#define GL_VENDOR 0x1F00
#define GL_RENDERER 0x1F01
#define GL_VERSION 0x1F02
#define GL_SHADING_LANGUAGE_VERSION 0x8B8C

#define GL_ARRAY_BUFFER 0x8892

#define GL_STATIC_DRAW 0x88E4
#define GL_DYNAMIC_DRAW 0x88E8

#define GL_TRUE 1
#define GL_FALSE 0
#define GL_BYTE 0x1400
#define GL_UNSIGNED_BYTE 0x1401
#define GL_SHORT 0x1402
#define GL_UNSIGNED_SHORT 0x1403
#define GL_INT 0x1404
#define GL_UNSIGNED_INT 0x1405
#define GL_FLOAT 0x1406

#define GL_TRIANGLES 0x0004

#define GL_TEXTURE_2D 0x0DE1

#define GL_RED 0x1903
#define GL_GREEN 0x1904
#define GL_BLUE 0x1905
#define GL_ALPHA 0x1906
#define GL_RGB 0x1907
#define GL_RGBA 0x1908

#define GL_TEXTURE0 0x84C0
#define GL_TEXTURE31 0x84DF

#define GL_NEAREST 0x2600
#define GL_LINEAR 0x2601

#define GL_TEXTURE_MAG_FILTER 0x2800
#define GL_TEXTURE_MIN_FILTER 0x2801

#define GL_COLOR_ATTACHMENT0 0x8CE0
#define GL_COLOR_ATTACHMENT31 0x8CFF

#define GL_BLEND 0x0BE2
#define GL_BLEND_DST 0x0BE0
#define GL_BLEND_SRC 0x0BE1

#define GL_SRC_ALPHA 0x0302
#define GL_ONE_MINUS_SRC_ALPHA 0x0303

#define GL_ELEMENT_ARRAY_BUFFER 0x8893

#define GL_DEPTH_TEST 0x0B71
#define GL_STENCIL_TEST 0x0B90
#define GL_SCISSOR_TEST 0x0C11

#define GL_MAX_TEXTURE_SIZE 0x0D33

#define GL_R8 0x8229
#define GL_TEXTURE_SWIZZLE_R 0x8E42
#define GL_TEXTURE_SWIZZLE_G 0x8E43
#define GL_TEXTURE_SWIZZLE_B 0x8E44
#define GL_TEXTURE_SWIZZLE_A 0x8E45
#define GL_TEXTURE_SWIZZLE_RGBA 0x8E46

#define GL_WRITE_ONLY 0x88B8

#define GL_MAP_WRITE_BIT 0x0002
#define GL_MAP_INVALIDATE_BUFFER_BIT 0x0008

#define GL_FRONT 0x0404
#define GL_BACK 0x0405
#define GL_FRONT_AND_BACK 0x0408

#define GL_POINT 0x1B00
#define GL_LINE 0x1B01
#define GL_FILL 0x1B02

void loadGL(void* proc);



struct GLTexture : public virtual Texture {
	unsigned int ID;
	const bool needsToBeFlipped;
	GLTexture(unsigned int ID, int Width, int Height, bool needsToBeFlipped = false, unsigned char* data = nullptr) : ID(ID), needsToBeFlipped(needsToBeFlipped), Texture(Width, Height, data) {}

	~GLTexture() {
		glDeleteTextures(1, &ID);
		free(data);
	}
};

struct GLRenderTexture : public RenderTexture, public GLTexture {
	unsigned int fbo;
	GLRenderTexture(unsigned int fbo, unsigned int ID, int Width, int Height) : fbo(fbo), Texture(Width, Height), GLTexture(ID, Width, Height, true), RenderTexture(Width, Height) { }
	~GLRenderTexture() {
		glDeleteFramebuffers(1, &fbo);
	}
};


#endif