#include <Graphics/OpenGL/GL.h>

//OpenGL 1.0
glViewport_type glViewport = nullptr;
glClearColor_type glClearColor = nullptr;
glClear_type glClear = nullptr;
glGetString_type glGetString = nullptr;
glEnable_type glEnable = nullptr;
glDisable_type glDisable = nullptr;
glBlendFunc_type glBlendFunc = nullptr;
glGenTextures_type glGenTextures = nullptr;
glBindTexture_type glBindTexture = nullptr;
glTexImage2D_type glTexImage2D = nullptr;
glTexParameteri_type glTexParameteri = nullptr;
glDeleteTextures_type glDeleteTextures = nullptr;
glGetIntegerv_type glGetIntegerv = nullptr;
glPolygonMode_type glPolygonMode = nullptr;

//OpenGL 2.0
glCreateProgram_type glCreateProgram = nullptr;
glAttachShader_type glAttachShader = nullptr;
glLinkProgram_type glLinkProgram = nullptr;
glDeleteShader_type glDeleteShader = nullptr;
glUseProgram_type glUseProgram = nullptr;
glGetUniformLocation_type glGetUniformLocation = nullptr;
glUniform1i_type glUniform1i = nullptr;
glUniform1f_type glUniform1f = nullptr;
glUniform2f_type glUniform2f = nullptr;
glCreateShader_type glCreateShader = nullptr;
glShaderSource_type glShaderSource = nullptr;
glCompileShader_type glCompileShader = nullptr;
glGetShaderiv_type glGetShaderiv = nullptr;
glGetShaderInfoLog_type glGetShaderInfoLog = nullptr;
glDeleteProgram_type glDeleteProgram = nullptr;
glUniformMatrix4fv_type glUniformMatrix4fv = nullptr;
glGenBuffers_type glGenBuffers = nullptr;
glBindBuffer_type glBindBuffer = nullptr;
glBufferData_type glBufferData = nullptr;
glEnableVertexAttribArray_type glEnableVertexAttribArray = nullptr;
glDisableVertexAttribArray_type glDisableVertexAttribArray = nullptr;
glVertexAttribPointer_type glVertexAttribPointer = nullptr;
glDrawArrays_type glDrawArrays = nullptr;
glDrawElements_type glDrawElements = nullptr;
glActiveTexture_type glActiveTexture = nullptr;
glDeleteBuffers_type glDeleteBuffers = nullptr;
glMapBuffer_type glMapBuffer = nullptr;
glUnmapBuffer_type glUnmapBuffer = nullptr;

//OpenGL 3.0
glGenFramebuffers_type glGenFramebuffers = nullptr;
glBindFramebuffer_type glBindFramebuffer = nullptr;
glDeleteFramebuffers_type glDeleteFramebuffers = nullptr;
glGenVertexArrays_type glGenVertexArrays = nullptr;
glBindVertexArray_type glBindVertexArray = nullptr;
glGenerateMipmap_type glGenerateMipmap = nullptr;
glFramebufferTexture2D_type glFramebufferTexture2D = nullptr;
glDeleteVertexArrays_type glDeleteVertexArrays = nullptr;
glMapBufferRange_type glMapBufferRange = nullptr;

//OpenGL 3.1
glDrawElementsInstanced_type glDrawElementsInstanced = nullptr;

//OpenGL 3.3
glVertexAttribDivisor_type glVertexAttribDivisor = nullptr;



#ifdef WIN32
typedef void* (APIENTRY* GLproc)(const char* name);
#elif defined(X11)
typedef void* (*GLproc)(const GLubyte* name);
#endif

#ifdef X11
#define proc(text) proc((const GLubyte*) text) 
#endif

void loadGL_1_0(GLproc proc) {
	glViewport = (glViewport_type)proc("glViewport");
	glClearColor = (glClearColor_type)proc("glClearColor");
	glClear = (glClear_type)proc("glClear");
	glGetString = (glGetString_type)proc("glGetString");
	glEnable = (glEnable_type)proc("glEnable");
	glDisable = (glDisable_type)proc("glDisable");
	glBlendFunc = (glBlendFunc_type)proc("glBlendFunc");
	glGenTextures = (glGenTextures_type)proc("glGenTextures");
	glBindTexture = (glBindTexture_type)proc("glBindTexture");
	glTexImage2D = (glTexImage2D_type)proc("glTexImage2D");
	glTexParameteri = (glTexParameteri_type)proc("glTexParameteri");
	glDeleteTextures = (glDeleteTextures_type)proc("glDeleteTextures");
	glGetIntegerv = (glGetIntegerv_type)proc("glGetIntegerv");
	glPolygonMode = (glPolygonMode_type)proc("glPolygonMode");
}

void loadGL_2_0(GLproc proc) {
	glCreateProgram = (glCreateProgram_type)proc("glCreateProgram");
	glAttachShader = (glAttachShader_type)proc("glAttachShader");
	glLinkProgram = (glLinkProgram_type)proc("glLinkProgram");
	glDeleteShader = (glDeleteShader_type)proc("glDeleteShader");
	glUseProgram = (glUseProgram_type)proc("glUseProgram");
	glGetUniformLocation = (glGetUniformLocation_type)proc("glGetUniformLocation");
	glUniform1i = (glUniform1i_type)proc("glUniform1i");
	glUniform1f = (glUniform1f_type)proc("glUniform1f");
	glUniform2f = (glUniform2f_type)proc("glUniform2f");
	glCreateShader = (glCreateShader_type)proc("glCreateShader");
	glShaderSource = (glShaderSource_type)proc("glShaderSource");
	glCompileShader = (glCompileShader_type)proc("glCompileShader");
	glGetShaderiv = (glGetShaderiv_type)proc("glGetShaderiv");
	glGetShaderInfoLog = (glGetShaderInfoLog_type)proc("glGetShaderInfoLog");
	glDeleteProgram = (glDeleteProgram_type)proc("glDeleteProgram");
	glUniformMatrix4fv = (glUniformMatrix4fv_type)proc("glUniformMatrix4fv");
	glGenBuffers = (glGenBuffers_type)proc("glGenBuffers");
	glBindBuffer = (glBindBuffer_type)proc("glBindBuffer");
	glBufferData = (glBufferData_type)proc("glBufferData");
	glEnableVertexAttribArray = (glEnableVertexAttribArray_type)proc("glEnableVertexAttribArray");
	glDisableVertexAttribArray = (glDisableVertexAttribArray_type)proc("glDisableVertexAttribArray");
	glVertexAttribPointer = (glVertexAttribPointer_type)proc("glVertexAttribPointer");
	glDrawArrays = (glDrawArrays_type)proc("glDrawArrays");
	glDrawElements = (glDrawElements_type)proc("glDrawElements");
	glActiveTexture = (glActiveTexture_type)proc("glActiveTexture");
	glDeleteBuffers = (glDeleteBuffers_type)proc("glDeleteBuffers");
	glMapBuffer = (glMapBuffer_type)proc("glMapBuffer");
	glUnmapBuffer = (glUnmapBuffer_type)proc("glUnmapBuffer");
}

void loadGL_3_0(GLproc proc) {
	glGenFramebuffers = (glGenFramebuffers_type)proc("glGenFramebuffers");
	glBindFramebuffer = (glBindFramebuffer_type)proc("glBindFramebuffer");
	glDeleteFramebuffers = (glDeleteFramebuffers_type)proc("glDeleteFramebuffers");
	glGenVertexArrays = (glGenVertexArrays_type)proc("glGenVertexArrays");
	glBindVertexArray = (glBindVertexArray_type)proc("glBindVertexArray");
	glGenerateMipmap = (glGenerateMipmap_type)proc("glGenerateMipmap");
	glFramebufferTexture2D = (glFramebufferTexture2D_type)proc("glFramebufferTexture2D");
	glDeleteVertexArrays = (glDeleteVertexArrays_type)proc("glDeleteVertexArrays");
	glMapBufferRange = (glMapBufferRange_type)proc("glMapBufferRange");
}

void loadGL_3_1(GLproc proc) {
	glDrawElementsInstanced = (glDrawElementsInstanced_type)proc("glDrawElementsInstanced");
}

void loadGL_3_3(GLproc proc) {
	glVertexAttribDivisor = (glVertexAttribDivisor_type)proc("glVertexAttribDivisor");
}

void loadGL(void* procadd) {
	GLproc proc = (GLproc)procadd;
	loadGL_1_0(proc);
	loadGL_2_0(proc);
	loadGL_3_0(proc);
	loadGL_3_1(proc);
	loadGL_3_3(proc);
}