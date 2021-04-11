#define GLEW_STATIC
#include <GL/glew.h>

#include <SDL.h>
#include <SDL_opengl.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <chrono>
#include <stdio.h>

#include "cubic.hh"
#include "dice.hh"
#include "floor.hh"
#include "lightstar.hh"

const static float vertices[] =
{
   /*      position   |       color     | texture   */
        -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
         0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
         0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
         0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
         0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
         0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,

         0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
         0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
         0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
         0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
         0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
         0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
         0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
         0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
         0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
         0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
         0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
         0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,

    -1.0f, -1.0f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
     1.0f, -1.0f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
     1.0f,  1.0f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
     1.0f,  1.0f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
    -1.0f,  1.0f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
    -1.0f, -1.0f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f
};

int main(int argc, char *argv[])
{
	uint32_t WindowFlags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;
	int FullScreen = 0;
	int windowWidth = 800;
	int windowHeight = 600;
	int cycle = 1;
	int quit = 0;
	float scale = 1.0f;
#if 1
	float rotx = 180.0f;
	float roty = 2.0f;
	float rotz = 0.5f;
#endif
	float trax = 1.0f;
	float tray = 1.0f;
	float traz = 1.0f;

	SDL_Init(SDL_INIT_VIDEO);

	SDL_GL_SetAttribute (SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute (SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute (SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute (SDL_GL_STENCIL_SIZE, 8);

	SDL_Window *window = SDL_CreateWindow ("OpenGL", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowWidth, windowHeight, WindowFlags);
	SDL_SetWindowMinimumSize (window, 100, 100);
	SDL_GLContext context = SDL_GL_CreateContext (window);
	SDL_GL_SetSwapInterval (0); /* Disable VSYNC */

	glewExperimental = GL_TRUE;
	glewInit();


		GLuint vao; // Vertex Array Objects (VAO)
		glGenVertexArrays (1, &vao);
		glBindVertexArray (vao);

		GLuint vbo; // Vertex Buffer Object (VBO):
		glGenBuffers (1, &vbo);
		glBindBuffer (GL_ARRAY_BUFFER, vbo);
		glBufferData (GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		const char *vertexSource = R"glsl(
			#version 150 core
			in vec3 position;
			in vec3 color;
			in vec2 texcoord;

			out vec3 Color;
			out vec2 Texcoord;

			uniform mat4 model;
			uniform mat4 view;
			uniform mat4 proj;

			uniform vec3 overrideColor;

			void main()
			{
				gl_Position = proj * view * model * vec4(position, 1.0);
				Color = overrideColor * color;
				Texcoord = texcoord;
			})glsl";

		GLuint vertexShader = glCreateShader (GL_VERTEX_SHADER);
		glShaderSource (vertexShader, 1, &vertexSource, NULL);
		glCompileShader (vertexShader);
		GLint status;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
		if (status == false)
		{
			char buffer[512];
			glGetShaderInfoLog(vertexShader, 512, NULL, buffer);
			printf ("Failed to compile vertexSource:\n%s\n", buffer);
		}

		const char *fragmentSource = R"glsl(
			#version 150 core
			in vec3 Color;
			in vec2 Texcoord;

			out vec4 outColor;

			uniform sampler2D texKitten;
			uniform sampler2D texPuppy;

			void main()
			{
			       outColor = vec4(Color, 1.0) * mix(texture(texPuppy, Texcoord), texture(texKitten, Texcoord), 0.5);
			})glsl";

		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource (fragmentShader, 1, &fragmentSource, NULL);
		glCompileShader (fragmentShader);
		glGetShaderiv (fragmentShader, GL_COMPILE_STATUS, &status);
		if (status == false)
		{
			char buffer[512];
			glGetShaderInfoLog(fragmentShader, 512, NULL, buffer);
			printf ("Failed to compile fragmentSource:\n%s\n", buffer);
		}

		GLuint shaderProgram = glCreateProgram ();
		glAttachShader (shaderProgram, vertexShader);
		glAttachShader (shaderProgram, fragmentShader);

		glBindFragDataLocation(shaderProgram, 0, "outColor"); // This is the default when only when output, so not needed
		glLinkProgram (shaderProgram);
		glDeleteShader (vertexShader); // The finished program is not affected by this
		glDeleteShader (fragmentShader);

		glUseProgram (shaderProgram); /* program must be the active one, else glUniform1i will not perform as expected for the second texture */
#if 0
		GLuint textures[2];
		glGenTextures (2, textures);
		{
			int width, height;
			unsigned char *image;

			glActiveTexture(GL_TEXTURE0);
			glBindTexture (GL_TEXTURE_2D, textures[0]);
			image = SOIL_load_image("sample.png", &width, &height, 0, SOIL_LOAD_RGB);
			glTexImage2D (GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
			SOIL_free_image_data (image);
			glUniform1i(glGetUniformLocation(shaderProgram, "texKitten"), 0);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glActiveTexture(GL_TEXTURE1);
			glBindTexture (GL_TEXTURE_2D, textures[1]);
			image = SOIL_load_image("sample2.png", &width, &height, 0, SOIL_LOAD_RGB);
			glTexImage2D (GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
			SOIL_free_image_data (image);
			glUniform1i(glGetUniformLocation(shaderProgram, "texPuppy"), 1);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}
#endif


	wurfel_cubic *cubic = new wurfel_cubic();
	wurfel_dice *dice = new wurfel_dice();
	wurfel_floor *floor = new wurfel_floor();
	wurfel_lightstar *lightstar = new wurfel_lightstar();

	auto t_start = std::chrono::high_resolution_clock::now();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

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
						glViewport(0, 0, windowWidth, windowHeight);
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
							cycle = !cycle;
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
		glClearColor (0.0f, 0.0f, 0.0f, 1.0f);
		glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Calculate transformation
		auto t_now = std::chrono::high_resolution_clock::now();

		float time = std::chrono::duration_cast<std::chrono::duration<float>>(t_now - t_start).count();

		float timeCamRotation = std::chrono::duration_cast<std::chrono::duration<float>>((t_now - t_start) % 20000000000ll).count(); /* resolution is ten second */
		float CamRotation = glm::radians(timeCamRotation * 18.0); /* 20 seconds * 18.0 = 360 degrees */

		float timeCamHeight = std::chrono::duration_cast<std::chrono::duration<float>>((t_now - t_start) % 10000000000ll).count(); /* resolution is ten second */
		float CamHeight = glm::radians(timeCamHeight * 36.0); /* 10 seconds * 36.0 = 360 degrees */

		float timeLightRotation = std::chrono::duration_cast<std::chrono::duration<float>>((t_now - t_start) % 8000000000ll).count(); /* resolution is eight second */
		float LightRotation = glm::radians(timeLightRotation * 45.0); /* 8 seconds * 45.0 = 360 degrees */

		float timeClockRotation = std::chrono::duration_cast<std::chrono::duration<float>>((t_now - t_start) % 60000000000ll).count(); /* resolution is sixty second */

		float timeCubeRotation = std::chrono::duration_cast<std::chrono::duration<float>>((t_now - t_start) % 6000000000ll).count(); /* resolution is six second */

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
		glEnable(GL_STENCIL_TEST);

		// Draw floor MASK
		glStencilFunc(GL_ALWAYS, 1, 0xFF); // Set any stencil to 1
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		glStencilMask(0xFF); // Write to stencil buffer

		//glDisable(GL_DEPTH_TEST);
		glDepthMask(GL_FALSE);

		//glDepthMask(GL_FALSE); // Don't write to depth buffer
		glClear(GL_STENCIL_BUFFER_BIT); // Clear stencil buffer (0 by default)

		floor->render (true, glm::value_ptr(proj), glm::value_ptr(view));

		//glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);

		// Draw cube reflection
		glStencilFunc(GL_EQUAL, 1, 0xFF); // Pass test if stencil value is 1
		glStencilMask(0x00); // Don't write anything to stencil buffer
		//glDepthMask(GL_TRUE); // Write to depth buffer

		cubic->render (true, glm::value_ptr(proj), glm::value_ptr(view), light1, light2);
		dice->render (true, glm::value_ptr(proj), glm::value_ptr(view), light1, timeCubeRotation / 6.0f /*scale, trax, tray, traz*/);
		lightstar->render (true, glm::value_ptr(proj), glm::value_ptr(view), light1, camera, timeClockRotation);

		glDisable(GL_STENCIL_TEST);
		//glClear(GL_DEPTH_BUFFER_BIT);
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

#if 0
	glDeleteTextures(2, textures);
	glDeleteProgram (shaderProgram);
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
#endif

	SDL_GL_DeleteContext(context);
	SDL_Quit();
	return 0;
}
