#include <stdint.h>

class wurfel_floor
{
	struct vertix_t
	{
		float position[3];
	};

	struct element_t
	{
		GLuint vertices[3];
	};

	public:
		wurfel_floor (void);
		~wurfel_floor (void);

		void render (bool mirror, float const *proj, float const *view);

	private:
		static const struct vertix_t vertices[4];
		static const struct element_t elements[2];

		GLuint vao; // Vertex Array Objects (VAO)
		GLuint vbo; // Vertex Buffer Object (VBO):
		GLuint ebo; // Element Buffer Object - indexes vertices to basic elements, so we can re-use them
		GLuint shaderProgram;
		GLint attrPosition;
		GLint attrib_vertexNormal;    /* per vertex */
		GLint uniModel;
		GLint uniView;
		GLint uniProj;
		GLint uniMatrix;
		GLint uniMode;
		GLint uniform_ambient;
		GLint uniform_lightPos;
		GLint uniform_lightColor;
};
