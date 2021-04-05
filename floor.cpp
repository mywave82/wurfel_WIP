const static float floor_vertices[] =
{
        -0.5200f,  0.5200f,
         0.5200f,  0.5200f,
         0.5200f, -0.5200f,
        -0.5200f, -0.5200f,
};

static const GLuint floor_elements[] =
{
    2, 1, 0,
    0, 3, 2
};

const char *floor_vertexSource = R"glsl(
	#version 150 core
	in vec2 position;

	out vec2 Position;

	uniform mat4 model;
	uniform mat4 view;
	uniform mat4 proj;

	void main()
	{
		Position = position.xy;
		gl_Position = proj * view * model * vec4(position, 0.0, 0.2);
	})glsl";

const char *floor_fragmentSource = R"glsl(
	#version 150 core
	in vec2 Position;

	out vec4 outColor;
	uniform vec3 Color1;
	uniform vec3 Color2;
	uniform vec3 AvgColor;
	uniform float Frequency;

	void main()
	{
		vec3 color;
		vec2 fw = fwidth (Position.xy);
		vec2 fuzz = fw * Frequency * 2.0f;
		float fuzzMax = max(fuzz.s, fuzz.t);
		vec2 checkPos = fract(Position.xy * Frequency);
		//if (fuzzMax < 0.5)
		{
			vec2 p =          smoothstep(vec2(0.5), fuzz + vec2(0.5), checkPos) + 
			           (1.0 - smoothstep(vec2(0.0), fuzz,             checkPos));

			color = mix(Color1, Color2, p.x * p.y + (1.0 - p.x) * (1.0 - p.y));

			color = mix(color, AvgColor, smoothstep (0.125, 0.5, fuzzMax));

			outColor = vec4 (color, 1.0);
		//} else {
		//	outColor = vec4(AvgColor, 1.0);
		}
	})glsl";

GLuint floor_vao; // Vertex Array Objects (VAO)
GLuint floor_vbo; // Vertex Buffer Object (VBO):
GLuint floor_ebo; // Element Buffer Object - indexes vertices to basic elements, so we can re-use them
GLuint floor_shaderProgram;
GLint floor_posAttrib;
GLint floor_uniModel;
GLint floor_uniView;
GLint floor_uniProj;

static void floor_init (void)
{
	glGenVertexArrays (1, &floor_vao);
	glBindVertexArray (floor_vao);

	glGenBuffers (1, &floor_vbo);
	glBindBuffer (GL_ARRAY_BUFFER, floor_vbo);
	glBufferData (GL_ARRAY_BUFFER, sizeof(floor_vertices), floor_vertices, GL_STATIC_DRAW);

	glGenBuffers (1, &floor_ebo);
	glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, floor_ebo);
	glBufferData (GL_ELEMENT_ARRAY_BUFFER, sizeof(floor_elements), floor_elements, GL_STATIC_DRAW);

	GLuint vertexShader = glCreateShader (GL_VERTEX_SHADER);
	glShaderSource (vertexShader, 1, &floor_vertexSource, NULL);
	glCompileShader (vertexShader);
	GLint status;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
	if (status == false)
	{
		char buffer[512];
		glGetShaderInfoLog(vertexShader, 512, NULL, buffer);
		printf ("Failed to compile floor_vertexSource:\n%s\n", buffer);
	}

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource (fragmentShader, 1, &floor_fragmentSource, NULL);
	glCompileShader (fragmentShader);
	glGetShaderiv (fragmentShader, GL_COMPILE_STATUS, &status);
	if (status == false)
	{
		char buffer[512];
		glGetShaderInfoLog(fragmentShader, 512, NULL, buffer);
		printf ("Failed to compile floor_fragmentSource:\n%s\n", buffer);
	}

	floor_shaderProgram = glCreateProgram ();
	glAttachShader (floor_shaderProgram, vertexShader);
	glAttachShader (floor_shaderProgram, fragmentShader);

	glBindFragDataLocation(floor_shaderProgram, 0, "outColor");
	glLinkProgram (floor_shaderProgram);
	glDeleteShader (vertexShader); // The finished program is not affected by this
	glDeleteShader (fragmentShader);

	glUseProgram (floor_shaderProgram); /* program must be the activated once before glGetAttribLocation() and friends work */

	floor_posAttrib = glGetAttribLocation (floor_shaderProgram, "position"); /* local position inside the model */
	floor_uniModel = glGetUniformLocation (floor_shaderProgram, "model"); /* model location in the world */
	floor_uniView  = glGetUniformLocation (floor_shaderProgram, "view");   /* camera angle + position */
	floor_uniProj  = glGetUniformLocation (floor_shaderProgram, "proj");   /* project perspective */
}

static void floor_render (float const *proj, float const *view)
{
	glBindVertexArray (floor_vao);
	glBindBuffer (GL_ARRAY_BUFFER, floor_vbo);
	glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, floor_ebo);

	glUseProgram (floor_shaderProgram);

	glUniform3f (glGetUniformLocation (floor_shaderProgram, "Color1"),     0.000f, 0.000f, 1.00f);
	glUniform3f (glGetUniformLocation (floor_shaderProgram, "Color2"),     0.050f, 0.050f, 0.05f);
	glUniform3f (glGetUniformLocation (floor_shaderProgram, "AvgColor"),   0.025f, 0.025f, 0.50f);
	glUniform1f (glGetUniformLocation (floor_shaderProgram, "Frequency"),  8.0f);


	glEnableVertexAttribArray (floor_posAttrib);
	glVertexAttribPointer (floor_posAttrib, 2, GL_FLOAT, GL_FALSE, 2 * sizeof (float), 0);

	glUniformMatrix4fv(floor_uniProj, 1, GL_FALSE, proj);
	glUniformMatrix4fv(floor_uniView, 1, GL_FALSE, view);



	glm::mat4 model = glm::mat4(1.0f);
#if 0
	model = glm::rotate(
	    model,
	    glm::degrees(90.0f),
	    glm::vec3(1.0f, 0.0f, 0.0f)
	);
#endif
//	model = glm::rotate (model, glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));
//	model = glm::scale (model, glm::vec3(10.0f));
//	model = glm::translate (model, glm::vec3(0.0f, 0.0f, sinf(glm::radians(angle))));
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

	glUniformMatrix4fv(floor_uniModel, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements (GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

static void floor_cleanup (void)
{
	glDeleteProgram (floor_shaderProgram);
	glDeleteBuffers(1, &floor_ebo);
	glDeleteBuffers(1, &floor_vbo);
	glDeleteVertexArrays(1, &floor_vao);
}
