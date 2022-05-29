#ifndef SDL_GLEW
#define GLEW_STATIC
#include <GL/glew.h>
#else
// Let SDL_opengl.h include glew functions
#define GL_GLEXT_PROTOTYPES 1
#endif

#include <SDL_opengl.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <assert.h>
#include <math.h>
#include <stdio.h>

#include "cubic.hh"

#include "cubic-vert.cpp" /* vertix shader */
#include "cubic-geom.cpp" /* geomery shader */
#include "cubic-frag.cpp" /* fragment shader */

#include "cubic-data.cpp" /* vertices w/normals and triangles */

float wurfel_cubic::measure_width(const struct vertix_t *v, const int count)
{
	float minx =  10.0f;
	float maxx = -10.0f;
	for (int i=0; i < count; i++)
	{
		if (v[i].position[0] > maxx) maxx = v[i].position[0];
		if (v[i].position[0] < minx) minx = v[i].position[0];
	}
	return maxx - minx;
}

wurfel_cubic::wurfel_cubic (void)
{
	float width[5];

	float totalwidth;

	width[0] = measure_width (vertices_C, sizeof (vertices_C) / sizeof (vertices_C[0]));
	width[1] = measure_width (vertices_U, sizeof (vertices_U) / sizeof (vertices_U[0]));
	width[2] = measure_width (vertices_B, sizeof (vertices_B) / sizeof (vertices_B[0]));
	width[3] = measure_width (vertices_I, sizeof (vertices_I) / sizeof (vertices_I[0]));
	width[4] = width[0];

	totalwidth = width[0] + width[1] + width[2] + width[3] + width[4] + 0.2 * 4;

	translate[0][0] = -totalwidth / 2 + width[0] / 2.0;
	translate[1][0] = translate[0][0] + 0.2 + width[0] / 2.0 + width[1] / 2.0;
	translate[2][0] = translate[1][0] + 0.2 + width[1] / 2.0 + width[2] / 2.0;
	translate[3][0] = translate[2][0] + 0.2 + width[2] / 2.0 + width[3] / 2.0;
	translate[4][0] = translate[3][0] + 0.2 + width[3] / 2.0 + width[4] / 2.0;

	printf ("width: %f %f %f %f %f\n", width[0], width[1], width[2], width[3], width[4]);
	printf ("translations: %f %f %f %f %f\n", translate[0][0], translate[1][0], translate[2][0], translate[3][0], translate[4][0]);

	translate[0][1] = 0.0;
	translate[1][1] = 0.0;
	translate[2][1] = 0.0;
	translate[3][1] = 0.0;
	translate[4][1] = 0.0;

	translate[0][2] = 0.0;
	translate[1][2] = 0.0;
	translate[2][2] = 0.0;
	translate[3][2] = 0.0;
	translate[4][2] = 0.0;

	glGenVertexArrays (1, &cubic_vao);
	glBindVertexArray (cubic_vao);

	glGenBuffers (4, cubic_vbo);

	glBindBuffer (GL_ARRAY_BUFFER, cubic_vbo[0]);
	glBufferData (GL_ARRAY_BUFFER, sizeof(vertices_C), vertices_C, GL_STATIC_DRAW);
	glBindBuffer (GL_ARRAY_BUFFER, cubic_vbo[1]);
	glBufferData (GL_ARRAY_BUFFER, sizeof(vertices_U), vertices_U, GL_STATIC_DRAW);
	glBindBuffer (GL_ARRAY_BUFFER, cubic_vbo[2]);
	glBufferData (GL_ARRAY_BUFFER, sizeof(vertices_B), vertices_B, GL_STATIC_DRAW);
	glBindBuffer (GL_ARRAY_BUFFER, cubic_vbo[3]);
	glBufferData (GL_ARRAY_BUFFER, sizeof(vertices_I), vertices_I, GL_STATIC_DRAW);

	glGenBuffers (4, cubic_ebo);
	glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, cubic_ebo[0]);
	glBufferData (GL_ELEMENT_ARRAY_BUFFER, sizeof(elements_C), elements_C, GL_STATIC_DRAW);
	glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, cubic_ebo[1]);
	glBufferData (GL_ELEMENT_ARRAY_BUFFER, sizeof(elements_U), elements_U, GL_STATIC_DRAW);
	glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, cubic_ebo[2]);
	glBufferData (GL_ELEMENT_ARRAY_BUFFER, sizeof(elements_B), elements_B, GL_STATIC_DRAW);
	glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, cubic_ebo[3]);
	glBufferData (GL_ELEMENT_ARRAY_BUFFER, sizeof(elements_I), elements_I, GL_STATIC_DRAW);

	GLuint vertexShader = glCreateShader (GL_VERTEX_SHADER);
	glShaderSource (vertexShader, 1, &cubic_vertexSource, NULL);
	glCompileShader (vertexShader);
	GLint status;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
	if (status == false)
	{
		char buffer[512];
		glGetShaderInfoLog(vertexShader, 512, NULL, buffer);
		printf ("Failed to compile cubic_vertexSource:\n%s\n", buffer);
	}

	GLuint geometryShader = glCreateShader (GL_GEOMETRY_SHADER);
	glShaderSource (geometryShader, 1, &cubic_geometrySource, NULL);
	glCompileShader (geometryShader);
	glGetShaderiv(geometryShader, GL_COMPILE_STATUS, &status);
	if (status == false)
	{
		char buffer[512];
		glGetShaderInfoLog(geometryShader, 512, NULL, buffer);
		printf ("Failed to compile cubic_geometrySource:\n%s\n", buffer);
	}

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource (fragmentShader, 1, &cubic_fragmentSource, NULL);
	glCompileShader (fragmentShader);
	glGetShaderiv (fragmentShader, GL_COMPILE_STATUS, &status);
	if (status == false)
	{
		char buffer[512];
		glGetShaderInfoLog(fragmentShader, 512, NULL, buffer);
		printf ("Failed to compile cubic_fragmentSource:\n%s\n", buffer);
	}

	cubic_shaderProgram = glCreateProgram ();
	glAttachShader (cubic_shaderProgram, vertexShader);
	glAttachShader (cubic_shaderProgram, geometryShader);
	glAttachShader (cubic_shaderProgram, fragmentShader);

	glBindFragDataLocation(cubic_shaderProgram, 0, "outColor");
	glLinkProgram (cubic_shaderProgram);
	glDeleteShader (vertexShader); // The finished program is not affected by this
	glDeleteShader (geometryShader);
	glDeleteShader (fragmentShader);

	glUseProgram (cubic_shaderProgram); /* program must be the activated once before glGetAttribLocation() and friends work */

	attrib_position = glGetAttribLocation (cubic_shaderProgram, "position"); /* local position inside the model */
	attrib_vertexNormal    = glGetAttribLocation (cubic_shaderProgram,    "vertexNormal");
	cubic_uniModel = glGetUniformLocation (cubic_shaderProgram, "model"); /* model location in the world */
	cubic_uniView  = glGetUniformLocation (cubic_shaderProgram, "view");   /* camera angle + position */
	cubic_uniProj  = glGetUniformLocation (cubic_shaderProgram, "proj");   /* project perspective */
	cubic_matrix  = glGetUniformLocation (cubic_shaderProgram, "matrix");   /* project perspective */

	uniform_ambient    = glGetUniformLocation (cubic_shaderProgram, "ambient");
	uniform_light1Pos   = glGetUniformLocation (cubic_shaderProgram, "light1Pos");
	uniform_light2Pos   = glGetUniformLocation (cubic_shaderProgram, "light2Pos");
	uniform_light1Color = glGetUniformLocation (cubic_shaderProgram, "light1Color");
	uniform_light2Color = glGetUniformLocation (cubic_shaderProgram, "light2Color");
}

void wurfel_cubic::render (bool mirror, float const *proj, float const *view, float const *light1, float const *light2)
{
	glBindVertexArray (cubic_vao);
	glUseProgram (cubic_shaderProgram);

	{
		float ambient[3] = {0.2f, 0.2f, 0.2f};
		//float lightPos[3] = {5.0f, 5.0f, 5.0f};
		float light1Color[3] = {0.6f, 1.0f, 1.0f};
		float light2Color[3] = {2.0f, 0.0f, 0.0f};

		glUniform3fv (uniform_ambient,    1, ambient);
		glUniform3fv (uniform_light1Pos,   1, light1);
		glUniform3fv (uniform_light2Pos,   1, light2);
		glUniform3fv (uniform_light1Color, 1, light1Color);
		glUniform3fv (uniform_light2Color, 1, light2Color);

		glUniformMatrix4fv(cubic_uniProj, 1, GL_FALSE, proj);
		glUniformMatrix4fv(cubic_uniView, 1, GL_FALSE, view);
	}		


	for (int letter = 0; letter < 5; letter++)
	{
		glBindBuffer (GL_ARRAY_BUFFER, cubic_vbo[letter%4]);
		glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, cubic_ebo[letter%4]);
	
		glEnableVertexAttribArray (attrib_position);
		glVertexAttribPointer (
			attrib_position,
			3,
			GL_FLOAT,
			GL_FALSE,
			sizeof (struct wurfel_cubic::vertix_t),
			(const void *)offsetof (struct wurfel_cubic::vertix_t, position)
		);

		glEnableVertexAttribArray (attrib_vertexNormal);
		glVertexAttribPointer (
			attrib_vertexNormal,
			3,
			GL_FLOAT,
			GL_FALSE,
			sizeof (struct wurfel_cubic::vertix_t),
			(const void *)offsetof (struct wurfel_cubic::vertix_t, normal)
		);

		/* we can move the texture around if we want to */
		glm::mat4 matrix = glm::mat4(1.0f);
		//matrix = glm::scale (matrix, glm::vec3(scale, scale, scale));
		matrix = glm::translate (matrix, glm::vec3((float)letter, 0.0f, 0.0f));
		glUniformMatrix4fv(cubic_matrix, 1, GL_FALSE, glm::value_ptr(matrix));


		glm::mat4 model = glm::mat4(1.0f);

		if (mirror == true)
		{
		    model = glm::scale(
			model,
			glm::vec3(1.0, 1.0, -1.0)
		    );
		}

		/* place in front of cube */
		model = glm::translate (model, glm::vec3(0.0f, -1.0f, 0.0f));

		model = glm::scale (model, glm::vec3(0.4f, 0.4f, 0.4f));
		// lift the cubic up from the floor (diagonal of a cubic is sqrt(3 * sideLength))
		model = glm::translate ( model, glm::vec3(translate[letter][0], translate[letter][1], translate[letter][2]));
		// spin with time
		//model = glm::rotate    ( model, (float)(spin * 2.0f * M_PI), glm::vec3(0.0f, 0.0f, 1.0f));

		// rotate the text to stand at the floor
		model = glm::rotate ( model, (float)(M_PI / 2.0f), glm::vec3(1.0, 0.0, 0.0));

		/* lift onto the top of the floor */
		model = glm::translate (model, glm::vec3(0.0f, 0.5f, 0.0f));

#if 0
	printf ("model[%7.2f %7.2f %7.2f %7.2f\n"
	        "      %7.2f %7.2f %7.2f %7.2f\n"
	        "      %7.2f %7.2f %7.2f %7.2f\n"
	        "      %7.2f %7.2f %7.2f %7.2f]\n",
		model[0][0], model[1][0], model[2][0], model[3][0],
		model[0][1], model[1][1], model[2][1], model[3][1],
		model[0][2], model[1][2], model[2][2], model[3][2],
		model[0][3], model[1][3], model[2][3], model[3][3]);
	printf ("view [%7.2f %7.2f %7.2f %7.2f\n"
	        "      %7.2f %7.2f %7.2f %7.2f\n"
	        "      %7.2f %7.2f %7.2f %7.2f\n"
	        "      %7.2f %7.2f %7.2f %7.2f]\n",
		view[0*4+0], view[1*4+0], view[2*4+0], view[3*4+0],
		view[0*4+1], view[1*4+1], view[2*4+1], view[3*4+1],
		view[0*4+2], view[1*4+2], view[2*4+2], view[3*4+2],
		view[0*4+3], view[1*4+3], view[2*4+3], view[3*4+3]);
	printf ("proj [%7.2f %7.2f %7.2f %7.2f\n"
	        "      %7.2f %7.2f %7.2f %7.2f\n"
	        "      %7.2f %7.2f %7.2f %7.2f\n"
	        "      %7.2f %7.2f %7.2f %7.2f]\n",
		proj[0*4+0], proj[1*4+0], proj[2*4+0], proj[3*4+0],
		proj[0*4+1], proj[1*4+1], proj[2*4+1], proj[3*4+1],
		proj[0*4+2], proj[1*4+2], proj[2*4+2], proj[3*4+2],
		proj[0*4+3], proj[1*4+3], proj[2*4+3], proj[3*4+3]);
#endif


		glUniformMatrix4fv(cubic_uniModel, 1, GL_FALSE, glm::value_ptr(model));

		if (mirror == true)
		{
			glFrontFace (GL_CW);
		}

		switch (letter)
		{
			case 0: case 4:
				glDrawElements (GL_TRIANGLES, sizeof (elements_C)/(sizeof(GLuint)), GL_UNSIGNED_INT, 0); break;
			case 1:
				glDrawElements (GL_TRIANGLES, sizeof (elements_U)/(sizeof(GLuint)), GL_UNSIGNED_INT, 0); break;
			case 2:
				glDrawElements (GL_TRIANGLES, sizeof (elements_B)/(sizeof(GLuint)), GL_UNSIGNED_INT, 0); break;
			case 3:
				glDrawElements (GL_TRIANGLES, sizeof (elements_I)/(sizeof(GLuint)), GL_UNSIGNED_INT, 0); break;
		}

		if (mirror == true)
		{
			glFrontFace (GL_CCW);
		}
	}
}

wurfel_cubic::~wurfel_cubic (void)
{
	glDeleteProgram (cubic_shaderProgram);
	glDeleteBuffers(4, cubic_ebo);
	glDeleteBuffers(4, cubic_vbo);
	glDeleteVertexArrays(1, &cubic_vao);
}
