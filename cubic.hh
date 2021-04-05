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
		wurfel_cubic (void);
		~wurfel_cubic (void);

		void render (float const *proj, float const *view, float const *light1);

	private:
#define HEADER
#include "cubic-data.cpp"
#undef HEADER
		float translate[5][3];

		float measure_width(const struct vertix_t *v, const int count);

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
		GLint uniform_lightPos;
		GLint uniform_lightColor;
};
