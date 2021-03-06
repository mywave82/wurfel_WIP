#include <stdint.h>

class wurfel_cubic
{
	struct vertix_t
	{
		float position[3];
		float normal[3];
	};

	struct element_t
	{
		GLuint vertices[3];
/*		float normal[3];*/
	};

	public:
#ifdef SDL_GLSYMBOL
		wurfel_cubic (const struct DrawFunctions_t *DrawFunctions);
#else
		wurfel_cubic (void);
#endif
		~wurfel_cubic (void);

		void render (bool mirror, float const *proj, float const *view, float const *light1, float const *light2);

	private:
#define HEADER
#include "cubic-data.cpp"
#undef HEADER
		float translate[5][3];

		float measure_width(const struct vertix_t *v, const int count);

#ifdef SDL_GLSYMBOL
		const struct DrawFunctions_t *DrawFunctions;
#endif
		GLuint cubic_vao; // Vertex Array Objects (VAO)
		GLuint cubic_vbo[4]; // Vertex Buffer Object (VBO) - C U B I
		GLuint cubic_ebo[4]; // Element Buffer Object - indexes vertices to basic elements, so we can re-use them
		GLuint cubic_shaderProgram;
		GLint attrib_position;
		GLint cubic_uniModel;
		GLint cubic_uniView;
		GLint cubic_uniProj;
		GLint cubic_matrix;
		GLint attrib_vertexNormal;    /* per vertex */
		GLint uniform_ambient;
		GLint uniform_light1Pos;
		GLint uniform_light2Pos;
		GLint uniform_light1Color;
		GLint uniform_light2Color;
};
