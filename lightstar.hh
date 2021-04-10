class wurfel_lightstar
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
		wurfel_lightstar (void);
		~wurfel_lightstar (void);

		void render (float const *proj, float const *view, float const *light1, float const *camera, float Clock);

	private:
		static const struct vertix_t vertices[4];
		static const struct element_t elements[2];

		GLuint vao; // Vertex Array Objects (VAO)
		GLuint vbo; // Vertex Buffer Object (VBO):
		GLuint ebo; // Element Buffer Object - indexes vertices to basic elements, so we can re-use them
		GLuint shaderProgram;
		GLint attrPosition;
		GLint uniModel;
		GLint uniView;
		GLint uniProj;
		GLint uniClock;
		//GLint uniMatrix;
};