#ifndef SDL_GLEW
#define GLEW_STATIC
#include <GL/glew.h>
#else
// Let SDL_opengl.h include glew functions
#define GL_GLEXT_PROTOTYPES 1
#endif

#include <SDL.h>
#include <SDL_opengl.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <chrono>
#include <stdio.h>

#include "sdl2_opengl.hh"
#include "cubic.hh"
#include "dice.hh"
#include "floor.hh"
#include "lightstar.hh"

int main(int argc, char *argv[])
{
#ifdef SDL_GLSYMBOL
	struct DrawFunctions_t DrawFunctions;
#define D  DrawFunctions.
#define D2 &DrawFunctions

#else
#define D
#define D2
#endif
	uint32_t WindowFlags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;
	int FullScreen = 0;
	int windowWidth = 800;
	int windowHeight = 600;
	int quit = 0;
	int pause = 0;
	float scale = 1.0f;
#if 1
	float rotx = 180.0f;
	float roty = 2.0f;
	float rotz = 0.5f;
#endif
	float trax = 1.0f;
	float tray = 1.0f;
	float traz = 1.0f;

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		fprintf (stderr, "SDL_Init(SDL_INIT_VIDEO) failed: %s\n", SDL_GetError());
		return 1;
	}

	if (SDL_GL_LoadLibrary	(NULL) < 0)
	{
		fprintf (stderr, "SDL_GL_LoadLibrary() failed: %s\n", SDL_GetError());
		SDL_Quit();
		return 1;
	}

	SDL_GL_SetAttribute (SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute (SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute (SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute (SDL_GL_STENCIL_SIZE, 8);

	SDL_Window *window = SDL_CreateWindow ("OpenGL", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowWidth, windowHeight, WindowFlags);
	SDL_SetWindowMinimumSize (window, 100, 100);
	SDL_GLContext context = SDL_GL_CreateContext (window);
	SDL_GL_SetSwapInterval (0); /* Disable VSYNC */

#ifdef SDL_GLSYMBOL
	DrawFunctions.glEnable                  = (GL_Enable_Func)                  SDL_GL_GetProcAddress("glEnable");
	DrawFunctions.glDisable                 = (GL_Enable_Func)                  SDL_GL_GetProcAddress("glDisable");
	DrawFunctions.glViewport                = (GL_Viewport_Func)                SDL_GL_GetProcAddress("glViewport");
	DrawFunctions.glClearColor              = (GL_ClearColor_Func)              SDL_GL_GetProcAddress("glClearColor");
	DrawFunctions.glClear                   = (GL_Clear_Func)                   SDL_GL_GetProcAddress("glClear");
	DrawFunctions.glStencilFunc             = (GL_StencilFunc_Func)             SDL_GL_GetProcAddress("glStencilFunc");
	DrawFunctions.glStencilMask             = (GL_StencilMask_Func)             SDL_GL_GetProcAddress("glStencilMask");
	DrawFunctions.glStencilOp               = (GL_StencilOp_Func)               SDL_GL_GetProcAddress("glStencilOp");
	DrawFunctions.glDepthMask               = (GL_DepthMask_Func)               SDL_GL_GetProcAddress("glDepthMask");
	DrawFunctions.glGenVertexArrays         = (GL_GenVertexArrays_Func)         SDL_GL_GetProcAddress("glGenVertexArrays");
	DrawFunctions.glBindVertexArray         = (GL_BindVertexArray_Func)         SDL_GL_GetProcAddress("glBindVertexArray");
	DrawFunctions.glDeleteVertexArrays      = (GL_DeleteVertexArray_Func)       SDL_GL_GetProcAddress("glDeleteVertexArrays");
	DrawFunctions.glGenBuffers              = (GL_GenBuffers_Func)              SDL_GL_GetProcAddress("glGenBuffers");
	DrawFunctions.glDeleteBuffers           = (GL_DeleteBuffers_Func)           SDL_GL_GetProcAddress("glDeleteBuffers");
	DrawFunctions.glBindBuffer              = (GL_BindBuffer_Func)              SDL_GL_GetProcAddress("glBindBuffer");
	DrawFunctions.glBufferData              = (GL_BufferData_Func)              SDL_GL_GetProcAddress("glBufferData");
	DrawFunctions.glCreateShader            = (GL_CreateShader_Func)            SDL_GL_GetProcAddress("glCreateShader");
	DrawFunctions.glShaderSource            = (GL_ShaderSource_Func)            SDL_GL_GetProcAddress("glShaderSource");
	DrawFunctions.glCompileShader           = (GL_CompileShader_Func)           SDL_GL_GetProcAddress("glCompileShader");
	DrawFunctions.glGetShaderiv             = (GL_GetShaderiv_Func)             SDL_GL_GetProcAddress("glGetShaderiv");
	DrawFunctions.glGetShaderInfoLog        = (GL_GetShaderInfoLog_Func)        SDL_GL_GetProcAddress("glGetShaderInfoLog");
	DrawFunctions.glDeleteShader            = (GL_DeleteShader_Func)            SDL_GL_GetProcAddress("glDeleteShader");
	DrawFunctions.glCreateProgram           = (GL_CreateProgram_Func)           SDL_GL_GetProcAddress("glCreateProgram");
	DrawFunctions.glAttachShader            = (GL_AttachShader_Func)            SDL_GL_GetProcAddress("glAttachShader");
	DrawFunctions.glLinkProgram             = (GL_LinkProgram_Func)             SDL_GL_GetProcAddress("glLinkProgram");
	DrawFunctions.glUseProgram              = (GL_UseProgram_Func)              SDL_GL_GetProcAddress("glUseProgram");
	DrawFunctions.glDeleteProgram           = (GL_DeleteProgram_Func)           SDL_GL_GetProcAddress("glDeleteProgram");
	DrawFunctions.glBlendFunc               = (GL_BlendFunc_Func)               SDL_GL_GetProcAddress("glBlendFunc");
	DrawFunctions.glEnableVertexAttribArray = (GL_EnableVertexAttribArray_Func) SDL_GL_GetProcAddress("glEnableVertexAttribArray");
	DrawFunctions.glVertexAttribPointer     = (GL_VertexAttribPointer_Func)     SDL_GL_GetProcAddress("glVertexAttribPointer");
	DrawFunctions.glBindFragDataLocation    = (GL_BindFragDataLocation_Func)    SDL_GL_GetProcAddress("glBindFragDataLocation");
	DrawFunctions.glGetAttribLocation       = (GL_GetAttribLocation_Func)       SDL_GL_GetProcAddress("glGetAttribLocation");
	DrawFunctions.glGetUniformLocation      = (GL_GetUniformLocation_Func)      SDL_GL_GetProcAddress("glGetUniformLocation");
	DrawFunctions.glUniform1f               = (GL_Uniform1f_Func)               SDL_GL_GetProcAddress("glUniform1f");
	DrawFunctions.glUniform3f               = (GL_Uniform3f_Func)               SDL_GL_GetProcAddress("glUniform3f");
	DrawFunctions.glUniform1i               = (GL_Uniform1i_Func)               SDL_GL_GetProcAddress("glUniform1i");
	DrawFunctions.glUniform3fv              = (GL_Uniform3fv_Func)              SDL_GL_GetProcAddress("glUniform3fv");
	DrawFunctions.glUniform4fv              = (GL_Uniform4fv_Func)              SDL_GL_GetProcAddress("glUniform4fv");
	DrawFunctions.glUniformMatrix4fv        = (GL_UniformMatrix4fv_Func)        SDL_GL_GetProcAddress("glUniformMatrix4fv");
	DrawFunctions.glFrontFace               = (GL_FrontFace_Func)               SDL_GL_GetProcAddress("glFrontFace");
	DrawFunctions.glDrawElements            = (GL_DrawElements_Func)            SDL_GL_GetProcAddress("glDrawElements");
#endif

#ifndef SDL_GLEW
	glewExperimental = GL_TRUE;
	glewInit();
#else
	// We let SDL handle this
#endif

	wurfel_cubic *cubic = new wurfel_cubic(D2);
	wurfel_dice *dice = new wurfel_dice(D2);
	wurfel_floor *floor = new wurfel_floor(D2);
	wurfel_lightstar *lightstar = new wurfel_lightstar(D2);

	auto t_start = std::chrono::high_resolution_clock::now();
	auto t_now = t_start;
	auto t_previous = t_start;

	D glEnable(GL_DEPTH_TEST);
	D glEnable(GL_CULL_FACE);

	while (!quit)
	{
		SDL_Event event;

		while (SDL_PollEvent(&event))
		{
			fprintf (stderr, "event.type = %d\n", event.type);
			switch (event.type)
			{
				case SDL_QUIT:
					quit = 1;
					break;
				case SDL_WINDOWEVENT:
					if (event.window.event == SDL_WINDOWEVENT_RESIZED)
					{
						windowWidth = event.window.data1;
						windowHeight = event.window.data2;
						D glViewport(0, 0, windowWidth, windowHeight);
					}
					break;
				case SDL_KEYUP:
					if (event.key.keysym.sym == SDLK_ESCAPE)
						quit = 1;
					break;
				case SDL_KEYDOWN:
		        		switch (event.key.keysym.sym)
					{
#if 1
						case 'q': rotx += 1.0; break;
						case 'a': rotx -= 1.0; break;
						case 'w': roty += 0.05; break;
						case 's': roty -= 0.05; break;
						case 'e': rotz += 0.05; break;
						case 'd': rotz -= 0.05; break;
#endif
						case 'r': scale *= 1.1; break;
						case 'f': scale /= 1.1; break;
						case 't': trax *= 1.1; break;
						case 'g': trax /= 1.1; break;
						case 'y': tray *= 1.1; break;
						case 'h': tray /= 1.1; break;
						case 'u': traz *= 1.1; break;
						case 'j': traz /= 1.1; break;
						case ' ':
							pause = !pause;
							break;
						case 'z':
							FullScreen = !FullScreen;
							if (FullScreen)
							{
								SDL_SetWindowFullscreen (window, WindowFlags | SDL_WINDOW_FULLSCREEN_DESKTOP);
							} else {
								SDL_SetWindowFullscreen (window, WindowFlags);
							}
							break;
					}
//					printf ("scale=%f rotate={%f %f %f} translate={%f %f %f}\n", scale, rotx, roty, rotz, trax, tray, traz);
//					printf ("scale=%f translate={%f %f %f}\n", scale, trax, tray, traz);
					break;
			}
		}

		// Clear the screen to black
		D glClearColor (0.0f, 0.0f, 0.0f, 1.0f);
		D glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Calculate transformation
		if (!pause)
		{
			t_now = std::chrono::high_resolution_clock::now();
		}

		//float time = std::chrono::duration_cast<std::chrono::duration<float>>(t_now - t_start).count();

		float timeCamRotation = std::chrono::duration_cast<std::chrono::duration<float>>((t_now - t_start) % 20000000000ll).count(); /* resolution is ten second */
		float CamRotation = glm::radians(timeCamRotation * 18.0); /* 20 seconds * 18.0 = 360 degrees */

		float timeCamHeight = std::chrono::duration_cast<std::chrono::duration<float>>((t_now - t_start) % 10000000000ll).count(); /* resolution is ten second */
		float CamHeight = glm::radians(timeCamHeight * 36.0); /* 10 seconds * 36.0 = 360 degrees */

		float timeLightRotation = std::chrono::duration_cast<std::chrono::duration<float>>((t_now - t_start) % 8000000000ll).count(); /* resolution is eight second */
		float LightRotation = glm::radians(timeLightRotation * 45.0); /* 8 seconds * 45.0 = 360 degrees */

		float timeClockRotation = std::chrono::duration_cast<std::chrono::duration<float>>((t_now - t_start) % 60000000000ll).count(); /* resolution is sixty second */

		float timeCubeRotation = std::chrono::duration_cast<std::chrono::duration<float>>((t_now - t_start) % 6000000000ll).count(); /* resolution is six second */

		dice->elapse (std::chrono::duration_cast<std::chrono::duration<float>>(t_now - t_previous).count());
		t_previous = t_now;

		float camera[3] =
		{
#if 0
			sinf(rotx * M_PI / 180.0f) * roty,
			cosf(rotx * M_PI / 180.0f) * roty,
			rotz,
#else
			sinf(CamRotation)*(sinf(CamHeight + M_PI) * 0.5f + 3.5f),
			cosf(CamRotation)*(sinf(CamHeight + M_PI) * 0.5f + 3.5f),
			sinf(CamHeight) * 1.0f + 1.3f
#endif
		};

		glm::mat4 view = glm::lookAt(
		    glm::vec3(camera[0], camera[1], camera[2]),
		    glm::vec3(0.0f, 0.0f, 0.5f),
		    glm::vec3(0.0f, 0.0f, 1.0f)
		);

		glm::mat4 proj = glm::perspectiveFov(glm::radians(45.0f), (float)windowWidth, (float)windowHeight, 0.2f, 20.0f);

		float light1[3];
		light1[0] = sin(LightRotation*2.0f)*2.4f;
		light1[1] = cos(LightRotation*2.0f)*2.4f;
		light1[2] = 1.0f;

		float light2[3];
		light2[0] = 0.0f;
		light2[1] = 0.0f;
		light2[2] = 0.5f;

#if 1
		D glEnable(GL_STENCIL_TEST);

		// Draw floor MASK
		D glStencilFunc(GL_ALWAYS, 1, 0xFF); // Set any stencil to 1
		D glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		D glStencilMask(0xFF); // Write to stencil buffer

		//D glDisable(GL_DEPTH_TEST);
		D glDepthMask(GL_FALSE);

		//D glDepthMask(GL_FALSE); // Don't write to depth buffer
		D glClear(GL_STENCIL_BUFFER_BIT); // Clear stencil buffer (0 by default)

		floor->render (true, glm::value_ptr(proj), glm::value_ptr(view));

		//D glEnable(GL_DEPTH_TEST);
		D glDepthMask(GL_TRUE);

		// Draw cube reflection
		D glStencilFunc(GL_EQUAL, 1, 0xFF); // Pass test if stencil value is 1
		D glStencilMask(0x00); // Don't write anything to stencil buffer
		//D glDepthMask(GL_TRUE); // Write to depth buffer

		cubic->render (true, glm::value_ptr(proj), glm::value_ptr(view), light1, light2);
		dice->render (true, glm::value_ptr(proj), glm::value_ptr(view), light1, timeCubeRotation / 6.0f /*scale, trax, tray, traz*/);
		lightstar->render (true, glm::value_ptr(proj), glm::value_ptr(view), light1, camera, timeClockRotation);

		D glDisable(GL_STENCIL_TEST);
		//D glClear(GL_DEPTH_BUFFER_BIT);
#endif

		floor->render (false, glm::value_ptr(proj), glm::value_ptr(view));

		cubic->render (false, glm::value_ptr(proj), glm::value_ptr(view), light1, light2);
		dice->render (false, glm::value_ptr(proj), glm::value_ptr(view), light1, timeCubeRotation / 6.0f /*scale, trax, tray, traz*/);
		lightstar->render (false, glm::value_ptr(proj), glm::value_ptr(view), light1, camera, timeClockRotation);

		SDL_GL_SwapWindow(window);
	}

	delete cubic; cubic = 0;
	delete dice; dice = 0;
	delete floor; floor = 0;
	delete lightstar; lightstar = 0;

	SDL_GL_DeleteContext(context);

	SDL_GL_UnloadLibrary();

	SDL_Quit();
	return 0;
}
