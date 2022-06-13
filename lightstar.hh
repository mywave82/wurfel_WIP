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
#ifdef SDL_GLSYMBOL
		wurfel_lightstar (const struct DrawFunctions_t *DrawFunctions);
#else
		wurfel_lightstar (void);
#endif
		~wurfel_lightstar (void);

		void render (bool mirror, float const *proj, float const *view, float const *light1, float const *camera, float Clock);

	private:
		static const struct vertix_t vertices[4];
		static const struct element_t elements[2];

#ifdef SDL_GLSYMBOL
		const struct DrawFunctions_t *DrawFunctions;
#endif
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
