#include <stdint.h>

class wurfel_dice
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
		wurfel_dice (const struct DrawFunctions_t *DrawFunctions);
#else
		wurfel_dice (void);
#endif
		~wurfel_dice (void);

		void render (bool mirror, float const *proj, float const *view, float const *light1, float spin/*, float scale, float trax, float tray, float traz*/);

	private:
		static const struct vertix_t vertices[1569];
		static const struct element_t elements[3102];

#ifdef SDL_GLSYMBOL
		const struct DrawFunctions_t *DrawFunctions;
#endif
		GLuint vao; // Vertex Array Objects (VAO)
		GLuint vbo; // Vertex Buffer Object (VBO):
		GLuint ebo; // Element Buffer Object - indexes vertices to basic elements, so we can re-use them
		GLuint shaderProgram;
		GLint attrib_position;
		GLint attrib_vertexNormal;    /* per vertex */
		GLint uniModel;
		GLint uniView;
		GLint uniProj;
		GLint uniMatrix;
		GLint uniform_ambient;
		GLint uniform_lightPos;
		GLint uniform_lightColor;
};
