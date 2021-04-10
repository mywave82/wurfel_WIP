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
		wurfel_dice (void);
		~wurfel_dice (void);

		void render (bool mirror, float const *proj, float const *view, float const *light1, float spin/*, float scale, float trax, float tray, float traz*/);

	private:
		static const struct vertix_t vertices[1569];
		static const struct element_t elements[3102];

		GLuint dice_vao; // Vertex Array Objects (VAO)
		GLuint dice_vbo; // Vertex Buffer Object (VBO):
		GLuint dice_ebo; // Element Buffer Object - indexes vertices to basic elements, so we can re-use them
		GLuint dice_shaderProgram;
		GLint attrib_position;
		GLint dice_uniModel;
		GLint dice_uniView;
		GLint dice_uniProj;
		GLint dice_matrix;
		GLint attrib_vertexNormal;    /* per vertex */
		GLint uniform_ambient;
		GLint uniform_lightPos;
		GLint uniform_lightColor;
};
