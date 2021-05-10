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
		/* the shiny flat part */
		static const struct vertix_t vertices1[4];
		static const struct element_t elements1[2];

		/* the frame */
		static const struct vertix_t vertices2[40];
		static const struct element_t elements2[74];


		GLuint vao[2]; // Vertex Array Objects (VAO)
		GLuint vbo[2]; // Vertex Buffer Object (VBO):
		GLuint ebo[2]; // Element Buffer Object - indexes vertices to basic elements, so we can re-use them
		GLuint shaderProgram[2];
		GLint attrPosition[2];
		GLint uniModel[2];
		GLint uniView[2];
		GLint uniProj[2];

		/* floor */
		GLint uniMode;

		/* border */
		GLint uniMatrix;
};
