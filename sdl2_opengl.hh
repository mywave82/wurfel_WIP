#ifndef SDL2_OPENGL_HH
#define SDL2_OPENGL_HH 1

#ifdef SDL_GLSYMBOL

#include <SDL.h>
#include <SDL_opengl.h>

typedef void   (APIENTRY * GL_Enable_Func)                  (GLenum cap);
typedef void   (APIENTRY * GL_Disable_Func)                 (GLenum cap);
typedef void   (APIENTRY * GL_Viewport_Func)                (GLint x, GLint y, GLsizei width, GLsizei height);
typedef void   (APIENTRY * GL_ClearColor_Func)              (GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
typedef void   (APIENTRY * GL_Clear_Func)                   (GLbitfield mas);
typedef void   (APIENTRY * GL_StencilFunc_Func)             (GLenum func, GLint ref, GLuint mask);
typedef void   (APIENTRY * GL_StencilMask_Func)             (GLuint mask);
typedef void   (APIENTRY * GL_StencilOp_Func)               (GLenum fail, GLenum zfail, GLenum zpass);
typedef void   (APIENTRY * GL_DepthMask_Func)               (GLboolean flag);
typedef void   (APIENTRY * GL_GenVertexArrays_Func)         (GLsizei n, GLuint *arrays);
typedef void   (APIENTRY * GL_BindVertexArray_Func)         (GLuint array);
typedef void   (APIENTRY * GL_DeleteVertexArray_Func)       (GLsizei n, const GLuint *arrays);
typedef void   (APIENTRY * GL_GenBuffers_Func)              (GLsizei n, GLuint *buffers);
typedef void   (APIENTRY * GL_DeleteBuffers_Func)           (GLsizei n, const GLuint *buffers);
typedef void   (APIENTRY * GL_BindBuffer_Func)              (GLenum target, GLuint buffer);
typedef void   (APIENTRY * GL_BufferData_Func)              (GLenum target, GLsizeiptr size, const void *data, GLenum usage);
typedef GLuint (APIENTRY * GL_CreateShader_Func)            (GLenum type);
typedef void   (APIENTRY * GL_ShaderSource_Func)            (GLuint shader, GLsizei count, const GLchar *const*string, const GLint *length);
typedef void   (APIENTRY * GL_CompileShader_Func)           (GLuint shader);
typedef void   (APIENTRY * GL_GetShaderiv_Func)             (GLuint shader, GLenum pname, GLint *params);
typedef void   (APIENTRY * GL_GetShaderInfoLog_Func)        (GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
typedef void   (APIENTRY * GL_DeleteShader_Func)            (GLuint shader);
typedef GLuint (APIENTRY * GL_CreateProgram_Func)           (void);
typedef void   (APIENTRY * GL_AttachShader_Func)            (GLuint program, GLuint shader);
typedef void   (APIENTRY * GL_LinkProgram_Func)             (GLuint);
typedef void   (APIENTRY * GL_UseProgram_Func)              (GLuint program);
typedef void   (APIENTRY * GL_DeleteProgram_Func)           (GLuint program);
typedef void   (APIENTRY * GL_BlendFunc_Func)               (GLenum sfactor, GLenum dfactor);
typedef void   (APIENTRY * GL_EnableVertexAttribArray_Func) (GLuint index);
typedef void   (APIENTRY * GL_VertexAttribPointer_Func)     (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer);
typedef void   (APIENTRY * GL_BindFragDataLocation_Func)    (GLuint program, GLuint color, const GLchar *name);
typedef GLint  (APIENTRY * GL_GetAttribLocation_Func)       (GLuint program, const GLchar *name);
typedef GLint  (APIENTRY * GL_GetUniformLocation_Func)      (GLuint program, const GLchar *name);
typedef void   (APIENTRY * GL_Uniform1f_Func)               (GLint location, GLfloat v0);
typedef void   (APIENTRY * GL_Uniform3f_Func)               (GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
typedef void   (APIENTRY * GL_Uniform1i_Func)               (GLint location, GLint v0);
typedef void   (APIENTRY * GL_Uniform3fv_Func)              (GLint location, GLsizei count, const GLfloat *value);
typedef void   (APIENTRY * GL_Uniform4fv_Func)              (GLint location, GLsizei count, const GLfloat *value);
typedef void   (APIENTRY * GL_UniformMatrix4fv_Func)        (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void   (APIENTRY * GL_FrontFace_Func)               (GLenum mode);
typedef void   (APIENTRY * GL_DrawElements_Func)            (GLenum mode, GLsizei count, GLenum type, const void *indices);

struct DrawFunctions_t
{
	GL_Enable_Func                  glEnable;
	GL_Disable_Func                 glDisable;
	GL_Viewport_Func                glViewport;
	GL_ClearColor_Func              glClearColor;
	GL_Clear_Func                   glClear;
	GL_StencilFunc_Func             glStencilFunc;
	GL_StencilMask_Func             glStencilMask;
	GL_StencilOp_Func               glStencilOp;
	GL_DepthMask_Func               glDepthMask;
	GL_GenVertexArrays_Func         glGenVertexArrays;
	GL_BindVertexArray_Func         glBindVertexArray;
	GL_DeleteVertexArray_Func       glDeleteVertexArrays;
	GL_GenBuffers_Func              glGenBuffers;
	GL_DeleteBuffers_Func           glDeleteBuffers;
	GL_BindBuffer_Func              glBindBuffer;
	GL_BufferData_Func              glBufferData;
	GL_CreateShader_Func            glCreateShader;
	GL_ShaderSource_Func            glShaderSource;
	GL_CompileShader_Func           glCompileShader;
	GL_GetShaderiv_Func             glGetShaderiv;
	GL_GetShaderInfoLog_Func        glGetShaderInfoLog;
	GL_DeleteShader_Func            glDeleteShader;
	GL_CreateProgram_Func           glCreateProgram;
	GL_AttachShader_Func            glAttachShader;
	GL_LinkProgram_Func             glLinkProgram;
	GL_UseProgram_Func              glUseProgram;
	GL_DeleteProgram_Func           glDeleteProgram;
	GL_BlendFunc_Func               glBlendFunc;
	GL_EnableVertexAttribArray_Func glEnableVertexAttribArray;
	GL_VertexAttribPointer_Func     glVertexAttribPointer;
	GL_BindFragDataLocation_Func    glBindFragDataLocation;
	GL_GetAttribLocation_Func       glGetAttribLocation;
	GL_GetUniformLocation_Func      glGetUniformLocation;
	GL_Uniform1f_Func               glUniform1f;
	GL_Uniform3f_Func               glUniform3f;
	GL_Uniform1i_Func               glUniform1i;
	GL_Uniform3fv_Func              glUniform3fv;
	GL_Uniform4fv_Func              glUniform4fv;
	GL_UniformMatrix4fv_Func        glUniformMatrix4fv;
	GL_FrontFace_Func               glFrontFace;
	GL_DrawElements_Func            glDrawElements;
};
#endif


#endif
