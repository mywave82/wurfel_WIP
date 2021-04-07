#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <assert.h>
#include <math.h>
#include <stdio.h>

struct vertix_t
{
	float position[3];
	float normal[3];
};

struct element_t
{
	int vertices[3];
};

struct vertix_t *vertices;
struct element_t *elements;

/*
'C'

     *****  **
    **********
   **     ****
  **        **
  **
  **       **
   **     ***
    *********
     *****
*/

#define t 0.1f
#define d 0.3f
void C(struct vertix_t **vertix, int *vertices, struct element_t **element, int *elements)
{
#define top   (0.75f*t) /* the toe */
#define topL  (1.50f*t)
#define split (2.00f*t)

#define outerR 0.5f
#define innerR (((1.0f-top)/(2.0f))-(1.1f*t))

	const int innerdots_count = 50;
	const int outerdots_count = 44;

	float inner_X = outerR + outerR - top - innerR;
	float outer_X = outerR;

	float angle_tA1 = asinf ((split / 2.0f) / innerR);
	float angle_tA2 = asinf ((split / 2.0f) / outerR);

	float tA1_X = cosf (angle_tA1) * innerR + inner_X;
	float tA2_X = cosf (angle_tA2) * outerR + outer_X;
	float tA5_X = tA2_X - topL;

	float angle_tA5 = acosf ((outerR -(outerR + outerR - tA5_X)) / outerR);

	int tA1, tA2, tA3, tA4, tA5, tA6, tA7/* tA8 == tA1 */;

	int total_elements = ((outerdots_count - 1) * 2 + (innerdots_count - outerdots_count + 3)) * 2 /* front and back */ + (innerdots_count + outerdots_count + 3) * 2 /* sides */;
	int total_vertices = (innerdots_count + outerdots_count + 3) * 4;

	*elements = 0;
	*vertices = 0;

	*vertix = (struct vertix_t *)malloc (sizeof ((*vertix)[0]) * total_vertices);
	*element = (struct element_t *)malloc (sizeof ((*element)[0]) * total_elements);

	tA1 = (*vertices)++;
	(*vertix)[tA1].position[0] = tA1_X;
	(*vertix)[tA1].position[1] = sinf (angle_tA1) * innerR + 0.5;
	(*vertix)[tA1].position[2] = d/2.0f;

	tA2 = (*vertices)++;
	(*vertix)[tA2].position[0] = tA2_X;
	(*vertix)[tA2].position[1] = sinf (angle_tA2) * outerR + 0.5;
	(*vertix)[tA2].position[2] = d/2.0f;

	tA3 = (*vertices)++;
	(*vertix)[tA3].position[0] = tA2_X;
	(*vertix)[tA3].position[1] = 1.0f;
	(*vertix)[tA3].position[2] = d/2.0f;

	tA4 = (*vertices)++;
	(*vertix)[tA4].position[0] = tA2_X - top;
	(*vertix)[tA4].position[1] = 1.0f;
	(*vertix)[tA4].position[2] = d/2.0f;


	tA5 = (*vertices);
	for (int i=0; i < outerdots_count; i++)
	{
		int index = (*vertices)++;
		(*vertix)[index].position[0] = cos(angle_tA5 + i * (2.0f * M_PI - angle_tA5 - angle_tA2) / ((float)outerdots_count - 1.0f)) * outerR + outer_X;
		(*vertix)[index].position[1] = sin(angle_tA5 + i * (2.0f * M_PI - angle_tA5 - angle_tA2) / ((float)outerdots_count - 1.0f)) * outerR + 0.5;
		(*vertix)[index].position[2] = d/2.0f;
	}
	tA6 = (*vertices) - 1;

	tA7 = (*vertices);
	for (int i=0; i < innerdots_count - 1; i++)
	{
		int index = (*vertices)++;
		(*vertix)[index].position[0] = cos(2.0f * M_PI - angle_tA1 - i * (2.0f * M_PI - angle_tA1 * 2.0f) / ((float)innerdots_count - 1.0f)) * innerR + inner_X;
		(*vertix)[index].position[1] = sin(2.0f * M_PI - angle_tA1 - i * (2.0f * M_PI - angle_tA1 * 2.0f) / ((float)innerdots_count - 1.0f)) * innerR + 0.5;
		(*vertix)[index].position[2] = d/2.0f;
	}

/* front faces */
	(*element)[(*elements)].vertices[0] = tA5;
	(*element)[(*elements)].vertices[1] = tA3;
	(*element)[(*elements)].vertices[2] = tA4;
	(*elements)++;

	(*element)[(*elements)].vertices[0] = tA5;
	(*element)[(*elements)].vertices[1] = tA2;
	(*element)[(*elements)].vertices[2] = tA3;
	(*elements)++;

	(*element)[(*elements)].vertices[0] = tA5;
	(*element)[(*elements)].vertices[1] = tA1;
	(*element)[(*elements)].vertices[2] = tA2;
	(*elements)++;

	(*element)[(*elements)].vertices[0] = tA5;
	(*element)[(*elements)].vertices[1] = (*vertices) - 1;
	(*element)[(*elements)].vertices[2] = tA1;
	(*elements)++;

	for (int i = 1; i < innerdots_count - outerdots_count; i++)
	{
		(*element)[(*elements)].vertices[0] = tA5;
		(*element)[(*elements)].vertices[1] = (*vertices) - i - 1;
		(*element)[(*elements)].vertices[2] = (*vertices) - i;
		(*elements)++;
	}

	for (int i = 0; i < (outerdots_count - 1); i++)
	{
		(*element)[(*elements)].vertices[0] = tA5 + i;
		(*element)[(*elements)].vertices[1] = tA7 + (outerdots_count - i) - 2;
		(*element)[(*elements)].vertices[2] = tA7 + (outerdots_count - i) - 1;
		(*elements)++;

		(*element)[(*elements)].vertices[0] = tA7 + (outerdots_count - i) - 2;
		(*element)[(*elements)].vertices[1] = tA5 + i;
		(*element)[(*elements)].vertices[2] = tA5 + i + 1;
		(*elements)++;
	}

/* back faces */
	int m = *vertices;

	for (int i = 0; i < m; i++)
	{
		int index = (*vertices)++;
		(*vertix)[index].position[0] = (*vertix)[i].position[0];
		(*vertix)[index].position[1] = (*vertix)[i].position[1];
		(*vertix)[index].position[2] = -d/2.0f;
	}

	int n = *elements;
	for (int i = 0; i < n; i++)
	{
		assert (*elements < total_elements);
		int index = (*elements)++;
		(*element)[index].vertices[0] = (*element)[i].vertices[0] + m;
		(*element)[index].vertices[1] = (*element)[i].vertices[2] + m;
		(*element)[index].vertices[2] = (*element)[i].vertices[1] + m;
	}

/* edges */
	for (int i = 0; i < (m*2); i++) // duplicate again, to make sharp normals
	{
		int index = (*vertices)++;
		(*vertix)[index].position[0] = (*vertix)[i].position[0];
		(*vertix)[index].position[1] = (*vertix)[i].position[1];
		(*vertix)[index].position[2] = (*vertix)[i].position[2];
	}

	for (int i = 0; i < m; i++)
	{
		assert (*elements < total_elements);
		(*element)[(*elements)].vertices[0] = ((tA1 + i)%m)     + m*2;
		(*element)[(*elements)].vertices[1] = ((tA1 + i)%m) + m + m*2;
		(*element)[(*elements)].vertices[2] = ((tA2 + i)%m)     + m*2;
		(*elements)++;

		assert (*elements < total_elements);
		(*element)[(*elements)].vertices[0] = ((tA2 + i)%m)     + m*2;
		(*element)[(*elements)].vertices[1] = ((tA1 + i)%m) + m + m*2;
		(*element)[(*elements)].vertices[2] = ((tA2 + i)%m) + m + m*2;
		(*elements)++;
	}
#undef top
#undef topL
#undef split
#undef outerR
#undef innerR
}

void U(struct vertix_t **vertix, int *vertices, struct element_t **element, int *elements)
{
#define teethW 1.50
#define left   1.50
#define right  1.00
#define top    0.75
#define teethR 0.10
#define bigR   3.00
#define gap    1.50
#define bigS   20

	int i;

	int total_elements = (16 + bigS * 2) * 2 + (18 + bigS - 1 + bigS - 1) * 2;
	int total_vertices = (20 + (bigS - 2) * 2) * 4 /* front and sides do not share vertices, since we want sharp normals at those edges */;

	int tA1,  tA2,  tA3,  tA4,
	    tA5,  tA6,  tA7,  tA8,
	    tA9,  tA10, tA11, tA12,
	    tA13, tA14, tA15, tA16,
	    tA17, tA18, tA19, tA20;

	*vertices = 0;
	*elements = 0;

	*vertix = (struct vertix_t *)malloc (sizeof ((*vertix)[0]) * total_vertices);
	*element = (struct element_t *)malloc (sizeof ((*element)[0]) * total_elements);

	tA1 = (*vertices)++;
	(*vertix)[tA1].position[0] = 0.0f;
	(*vertix)[tA1].position[1] = 1.0f;
	(*vertix)[tA1].position[2] = d/2.0f;

	tA2 = (*vertices)++;
	(*vertix)[tA2].position[0] = 0.0f;
	(*vertix)[tA2].position[1] = 1.0f - top * t;
	(*vertix)[tA2].position[2] = d/2.0f;

	tA3 = (*vertices)++;
	(*vertix)[tA3].position[0] = teethW * t - teethR * t;
	(*vertix)[tA3].position[1] = 1.0f - top * t;
	(*vertix)[tA3].position[2] = d/2.0f;

	tA4 = (*vertices)++;
	(*vertix)[tA4].position[0] = teethW * t;
	(*vertix)[tA4].position[1] = 1.0f - top * t - teethR * t;
	(*vertix)[tA4].position[2] = d/2.0f;

	tA5 = (*vertices)++;
	(*vertix)[tA5].position[0] = teethW * t;
	(*vertix)[tA5].position[1] = bigR * t;
	(*vertix)[tA5].position[2] = d/2.0f;

	for (i = 1; i < (bigS - 1); i++)
	{
		float Cx = (teethW * t +
		            teethW * t + left * t + teethW * t + gap * t + teethW * t + right * t) / 2.0f;
		float Cy = bigR * t;
		float Rx = (left * t + teethW * t + gap * t + teethW * t + right * t) / 2.0f;
		float Ry = bigR * t;

		int I = (*vertices)++;
		(*vertix)[I].position[0] = cosf (M_PI + (float)i * M_PI / ((float)bigS - 1.0f)) * Rx + Cx;
		(*vertix)[I].position[1] = sinf (M_PI + (float)i * M_PI / ((float)bigS - 1.0f)) * Ry + Cy;
		(*vertix)[I].position[2] = d/2.0f;
	}

	tA6 = (*vertices)++;
	(*vertix)[tA6].position[0] = teethW * t + left * t + teethW * t + gap * t + teethW * t + right * t;
	(*vertix)[tA6].position[1] = bigR * t;
	(*vertix)[tA6].position[2] = d/2.0f;

	tA7 = (*vertices)++;
	(*vertix)[tA7].position[0] = teethW * t + left * t + teethW * t + gap * t + teethW * t + right * t;
	(*vertix)[tA7].position[1] = 1.0f - top * t - teethR * t;
	(*vertix)[tA7].position[2] = d/2.0f;

	tA8 = (*vertices)++;
	(*vertix)[tA8].position[0] = teethW * t + left * t + teethW * t + gap * t + teethW * t + right * t + teethR * t;
	(*vertix)[tA8].position[1] = 1.0f - top * t;
	(*vertix)[tA8].position[2] = d/2.0f;

	tA9 = (*vertices)++;
	(*vertix)[tA9].position[0] = teethW * t + left * t + teethW * t + gap * t + teethW * t + right * t + teethW * t;
	(*vertix)[tA9].position[1] = 1.0f - top * t;
	(*vertix)[tA9].position[2] = d/2.0f;

	tA10 = (*vertices)++;
	(*vertix)[tA10].position[0] = teethW * t + left * t + teethW * t + gap * t + teethW * t + right * t + teethW * t;
	(*vertix)[tA10].position[1] = 1.0f;
	(*vertix)[tA10].position[2] = d/2.0f;

	tA11 = (*vertices)++;
	(*vertix)[tA11].position[0] = teethW * t + left * t + teethW * t + gap * t;
	(*vertix)[tA11].position[1] = 1.0f;
	(*vertix)[tA11].position[2] = d/2.0f;

	tA12 = (*vertices)++;
	(*vertix)[tA12].position[0] = teethW * t + left * t + teethW * t + gap * t;
	(*vertix)[tA12].position[1] = 1.0f - top * t;
	(*vertix)[tA12].position[2] = d/2.0f;

	tA13 = (*vertices)++;
	(*vertix)[tA13].position[0] = teethW * t + left * t + teethW * t + gap * t + teethW * t - teethR * t;
	(*vertix)[tA13].position[1] = 1.0f - top * t;
	(*vertix)[tA13].position[2] = d/2.0f;

	tA14 = (*vertices)++;
	(*vertix)[tA14].position[0] = teethW * t + left * t + teethW * t + gap * t + teethW * t;
	(*vertix)[tA14].position[1] = 1.0f - top * t - teethR * t;
	(*vertix)[tA14].position[2] = d/2.0f;

	tA15 = (*vertices)++;
	(*vertix)[tA15].position[0] = teethW * t + left * t + teethW * t + gap * t + teethW * t;
	(*vertix)[tA15].position[1] = bigR * t;
	(*vertix)[tA15].position[2] = d/2.0f;

	for (i = 1; i < (bigS - 1); i++)
	{
		float Cx = (0.0f + teethW * t + left * t + teethW * t + gap * t + teethW * t +
		            0.0f + teethW * t + left * t) / 2.0f;
		float Cy = bigR * t;
		float Rx = (teethW * t + gap * t + teethW * t) / 2.0f;
		float Ry = bigR * t - t;

		int I = (*vertices)++;
		(*vertix)[I].position[0] = cosf (2 * M_PI - (float)i * M_PI / ((float)bigS - 1.0f)) * Rx + Cx;
		(*vertix)[I].position[1] = sinf (2 * M_PI - (float)i * M_PI / ((float)bigS - 1.0f)) * Ry + Cy;
		(*vertix)[I].position[2] = d/2.0f;
	}


	tA16 = (*vertices)++;
	(*vertix)[tA16].position[0] = teethW * t + left * t;
	(*vertix)[tA16].position[1] = bigR * t;
	(*vertix)[tA16].position[2] = d/2.0f;

	tA17 = (*vertices)++;
	(*vertix)[tA17].position[0] = teethW * t + left * t;
	(*vertix)[tA17].position[1] = 1.0f - top * t - teethR * t;
	(*vertix)[tA17].position[2] = d/2.0f;

	tA18 = (*vertices)++;
	(*vertix)[tA18].position[0] = teethW * t + left * t + teethR * t;
	(*vertix)[tA18].position[1] = 1.0f - top * t;
	(*vertix)[tA18].position[2] = d/2.0f;

	tA19 = (*vertices)++;
	(*vertix)[tA19].position[0] = teethW * t + left * t + teethW * t;
	(*vertix)[tA19].position[1] = 1.0f - top * t;
	(*vertix)[tA19].position[2] = d/2.0f;

	tA20 = (*vertices)++;
	(*vertix)[tA20].position[0] = teethW * t + left * t + teethW * t;
	(*vertix)[tA20].position[1] = 1.0f;
	(*vertix)[tA20].position[2] = d/2.0f;


	(*element)[(*elements)].vertices[0] = tA20;
	(*element)[(*elements)].vertices[1] = tA1;
	(*element)[(*elements)].vertices[2] = tA2;
	(*elements)++;

	(*element)[(*elements)].vertices[0] = tA20;
	(*element)[(*elements)].vertices[1] = tA2;
	(*element)[(*elements)].vertices[2] = tA3;
	(*elements)++;

	(*element)[(*elements)].vertices[0] = tA20;
	(*element)[(*elements)].vertices[1] = tA3;
	(*element)[(*elements)].vertices[2] = tA18;
	(*elements)++;

	(*element)[(*elements)].vertices[0] = tA20;
	(*element)[(*elements)].vertices[1] = tA18;
	(*element)[(*elements)].vertices[2] = tA19;
	(*elements)++;

	(*element)[(*elements)].vertices[0] = tA18;
	(*element)[(*elements)].vertices[1] = tA3;
	(*element)[(*elements)].vertices[2] = tA4;
	(*elements)++;

	(*element)[(*elements)].vertices[0] = tA18;
	(*element)[(*elements)].vertices[1] = tA4;
	(*element)[(*elements)].vertices[2] = tA17;
	(*elements)++;

	(*element)[(*elements)].vertices[0] = tA17;
	(*element)[(*elements)].vertices[1] = tA4;
	(*element)[(*elements)].vertices[2] = tA5;
	(*elements)++;

	(*element)[(*elements)].vertices[0] = tA17;
	(*element)[(*elements)].vertices[1] = tA5;
	(*element)[(*elements)].vertices[2] = tA16;
	(*elements)++;

	for (i=0; i < bigS; i++)
	{
		(*element)[(*elements)].vertices[0] = tA16 - i;
		(*element)[(*elements)].vertices[1] = tA5 + i;
		(*element)[(*elements)].vertices[2] = tA5 + i + 1;
		(*elements)++;

		(*element)[(*elements)].vertices[0] = tA16 - i;
		(*element)[(*elements)].vertices[1] = tA5 + i + 1;
		(*element)[(*elements)].vertices[2] = tA16 - i - 1;
		(*elements)++;
	}


	(*element)[(*elements)].vertices[0] = tA7;
	(*element)[(*elements)].vertices[1] = tA14;
	(*element)[(*elements)].vertices[2] = tA15;
	(*elements)++;

	(*element)[(*elements)].vertices[0] = tA7;
	(*element)[(*elements)].vertices[1] = tA15;
	(*element)[(*elements)].vertices[2] = tA6;
	(*elements)++;

	(*element)[(*elements)].vertices[0] = tA8;
	(*element)[(*elements)].vertices[1] = tA13;
	(*element)[(*elements)].vertices[2] = tA14;
	(*elements)++;

	(*element)[(*elements)].vertices[0] = tA8;
	(*element)[(*elements)].vertices[1] = tA14;
	(*element)[(*elements)].vertices[2] = tA7;
	(*elements)++;

	(*element)[(*elements)].vertices[0] = tA10;
	(*element)[(*elements)].vertices[1] = tA11;
	(*element)[(*elements)].vertices[2] = tA12;
	(*elements)++;

	(*element)[(*elements)].vertices[0] = tA10;
	(*element)[(*elements)].vertices[1] = tA12;
	(*element)[(*elements)].vertices[2] = tA13;
	(*elements)++;

	(*element)[(*elements)].vertices[0] = tA10;
	(*element)[(*elements)].vertices[1] = tA13;
	(*element)[(*elements)].vertices[2] = tA8;
	(*elements)++;

	(*element)[(*elements)].vertices[0] = tA10;
	(*element)[(*elements)].vertices[1] = tA8;
	(*element)[(*elements)].vertices[2] = tA9;
	(*elements)++;

/* back faces */
	int m = *vertices;

	for (int i = 0; i < m; i++)
	{
		int index = (*vertices)++;
		(*vertix)[index].position[0] = (*vertix)[i].position[0];
		(*vertix)[index].position[1] = (*vertix)[i].position[1];
		(*vertix)[index].position[2] = -d / 2.0f;
	}

	int n = *elements;
	for (int i = 0; i < n; i++)
	{
		assert (*elements < total_elements);
		int index = (*elements)++;
		(*element)[index].vertices[0] = (*element)[i].vertices[0] + m;
		(*element)[index].vertices[1] = (*element)[i].vertices[2] + m;
		(*element)[index].vertices[2] = (*element)[i].vertices[1] + m;
	}

/* edges */
	for (int i = 0; i < (m*2); i++) // duplicate again, to make sharp normals
	{
		int index = (*vertices)++;
		(*vertix)[index].position[0] = (*vertix)[i].position[0];
		(*vertix)[index].position[1] = (*vertix)[i].position[1];
		(*vertix)[index].position[2] = (*vertix)[i].position[2];
	}

	for (int i = 0; i < m; i++)
	{
		assert (*elements < total_elements);
		(*element)[(*elements)].vertices[0] = ((tA1 + i)%m)     + m*2;
		(*element)[(*elements)].vertices[1] = ((tA1 + i)%m) + m + m*2;
		(*element)[(*elements)].vertices[2] = ((tA2 + i)%m)     + m*2;
		(*elements)++;

		assert (*elements < total_elements);
		(*element)[(*elements)].vertices[0] = ((tA2 + i)%m)     + m*2;
		(*element)[(*elements)].vertices[1] = ((tA1 + i)%m) + m + m*2;
		(*element)[(*elements)].vertices[2] = ((tA2 + i)%m) + m + m*2;
		(*elements)++;
	}

#undef teethW
#undef left
#undef right
#undef top
#undef teethR
#undef bigR
#undef gap
#undef bigS
}

void B(struct vertix_t **vertix, int *vertices, struct element_t **element, int *elements)
{
#define teethW  1.50
#define right   0.00 /* right vertix offset for outer ring */
#define outerX  3.00
#define innerX  1.75
#define left    1.50
#define middle  2.00
#define bottom 0.75
#define top    0.75
#define center 1.00 // 0.75
#define teethR 0.10
#define bigB   11

	int i;

	int tA1,  tA2,  tA3,  tA4,
	    tA5,  tA6,  tA7,  tA8,
	    tA9,  tA10, tA11;
	int tB1,  tB2,  tB3,  tB4;
	int tC1,  tC2,  tC3,  tC4;

	float R = (0.5f + center * t / 2.0f) / 2.0f;
	float K = 0.25f - (center * t) / 4.0f;

	float angle = acos (K / R);

	int total_elements = (17 + (bigB - 1) * 4 )* 2 + /* fronts */
	                     (12 + 3 + 3 + bigB - 1 + bigB - 2 + bigB - 2 + bigB - 2) * 2; /* sides*/
	int total_vertices = (11 + bigB * 4) * 4 /* front and sides do not share vertices, since we want sharp normals at those edges */;

	*vertices = 0;
	*elements = 0;

	*vertix = (struct vertix_t *)malloc (sizeof ((*vertix)[0]) * total_vertices);
	*element = (struct element_t *)malloc (sizeof ((*element)[0]) * total_elements);

	tA1 = (*vertices)++;
	(*vertix)[tA1].position[0] = 0.0f;
	(*vertix)[tA1].position[1] = 1.0f;
	(*vertix)[tA1].position[2] = d/2.0f;

	tA2 = (*vertices)++;
	(*vertix)[tA2].position[0] = 0.0f;
	(*vertix)[tA2].position[1] = 1.0f - top * t;
	(*vertix)[tA2].position[2] = d/2.0f;

	tA3 = (*vertices)++;
	(*vertix)[tA3].position[0] = (teethW - teethR) * t;
	(*vertix)[tA3].position[1] = 1.0f - top * t;
	(*vertix)[tA3].position[2] = d/2.0f;

	tA4 = (*vertices)++;
	(*vertix)[tA4].position[0] = teethW * t;
	(*vertix)[tA4].position[1] = 1.0f - (top + teethR ) * t;
	(*vertix)[tA4].position[2] = d/2.0f;

	tA5 = (*vertices)++;
	(*vertix)[tA5].position[0] = teethW * t;
	(*vertix)[tA5].position[1] = (bottom + teethR) * t;
	(*vertix)[tA5].position[2] = d/2.0f;

	tA6 = (*vertices)++;
	(*vertix)[tA6].position[0] = (teethW - teethR) * t;
	(*vertix)[tA6].position[1] = bottom * t;
	(*vertix)[tA6].position[2] = d/2.0f;

	tA7 = (*vertices)++;
	(*vertix)[tA7].position[0] = 0.0f;
	(*vertix)[tA7].position[1] = bottom * t;
	(*vertix)[tA7].position[2] = d/2.0f;

	tA8 = (*vertices)++;
	(*vertix)[tA8].position[0] = 0.0f;
	(*vertix)[tA8].position[1] = 0.0f;
	(*vertix)[tA8].position[2] = d/2.0f;

	tA9 = (*vertices); /* we let the code below generate the start point */ 
	for (i = 0; i < (bigB - 1); i++)
	{
		float Cx = (teethW + left + middle + right) * t;
		float Cy = (0.5f + (center * t) / 2.0f ) / 2.0f;
		float Rx = outerX * t;
		float Ry = R;

		int I = (*vertices)++;
		(*vertix)[I].position[0] = cosf (M_PI * 1.5f + (float)i * (M_PI - angle) / ((float)bigB - 1.0f)) * Rx + Cx;
		(*vertix)[I].position[1] = sinf (M_PI * 1.5f + (float)i * (M_PI - angle) / ((float)bigB - 1.0f)) * Ry + Cy;
		(*vertix)[I].position[2] = d/2.0f;
	}

	tA10 = (*vertices); /* we let the code below generate the center point */ 
	for (i = 0; i < (bigB - 1); i++)
	{
		float Cx = (teethW + left + middle + right) * t;
		float Cy = (1.0 + 0.5f - (center * t) / 2.0f ) / 2.0f;
		float Rx = outerX * t;
		float Ry = R;

		int I = (*vertices)++;
		(*vertix)[I].position[0] = cosf ((M_PI) * 1.5f + angle + ((float)i * ((M_PI) - angle)) / ((float)bigB - 1.0f)) * Rx + Cx;
		(*vertix)[I].position[1] = sinf ((M_PI) * 1.5f + angle + ((float)i * ((M_PI) - angle)) / ((float)bigB - 1.0f)) * Ry + Cy;
		(*vertix)[I].position[2] = d/2.0f;
	}

	tA11 = (*vertices)++;
	(*vertix)[tA11].position[0] = (teethW + left + middle + right) * t;
	(*vertix)[tA11].position[1] = 1.0f;
	(*vertix)[tA11].position[2] = d/2.0f;

	tB1 = (*vertices)++;
	(*vertix)[tB1].position[0] = (teethW + left) * t;
	(*vertix)[tB1].position[1] = 1.0f - top * t;
	(*vertix)[tB1].position[2] = d/2.0f;

	tB2 = (*vertices)++;
	(*vertix)[tB2].position[0] = (teethW + left)* t;
	(*vertix)[tB2].position[1] = 0.5f + center * t / 2.0f;
	(*vertix)[tB2].position[2] = d/2.0f;

	tB3 = (*vertices)++;
	(*vertix)[tB3].position[0] = (teethW + left + middle)* t;
	(*vertix)[tB3].position[1] = 0.5f + center * t / 2.0f;
	(*vertix)[tB3].position[2] = d/2.0f;

	for (i = 1; i < (bigB - 1); i++)
	{
		float Cx = (teethW + left + middle) * t;
		float Cy = (1.0f - top * t + 0.5f + (center * t) / 2.0f ) / 2.0f;
		float Rx = innerX * t;
		float Ry = (1.0f - (top + bottom + center ) * t) / 4.0f; //R;

		int I = (*vertices)++;
		(*vertix)[I].position[0] = cosf (M_PI * 1.5f + (float)i * M_PI / ((float)bigB - 1.0f)) * Rx + Cx;
		(*vertix)[I].position[1] = sinf (M_PI * 1.5f + (float)i * M_PI / ((float)bigB - 1.0f)) * Ry + Cy;
		(*vertix)[I].position[2] = d/2.0f;
	}

	tB4 = (*vertices)++;
	(*vertix)[tB4].position[0] = (teethW + left + middle)* t;
	(*vertix)[tB4].position[1] = 1.0f - top * t;
	(*vertix)[tB4].position[2] = d/2.0f;

	tC1 = (*vertices)++;
	(*vertix)[tC1].position[0] = (teethW + left)* t;
	(*vertix)[tC1].position[1] = 0.5f - center * t / 2.0f;
	(*vertix)[tC1].position[2] = d/2.0f;

	tC2 = (*vertices)++;
	(*vertix)[tC2].position[0] = (teethW + left)* t;
	(*vertix)[tC2].position[1] = 0.0f + bottom * t;
	(*vertix)[tC2].position[2] = d/2.0f;

	tC3 = (*vertices)++;
	(*vertix)[tC3].position[0] = (teethW + left + middle)* t;
	(*vertix)[tC3].position[1] = 0.0f + bottom * t;
	(*vertix)[tC3].position[2] = d/2.0f;

	for (i = 1; i < (bigB - 1); i++)
	{
		float Cx = (teethW + left + middle) * t;
		float Cy = (0.0 + bottom * t + 0.5f - (center * t) / 2.0f ) / 2.0f;
		float Rx = innerX * t;
		float Ry = (1.0f - (top + bottom + center ) * t) / 4.0f; //R;

		int I = (*vertices)++;
		(*vertix)[I].position[0] = cosf (M_PI * 1.5f + (float)i * M_PI / ((float)bigB - 1.0f)) * Rx + Cx;
		(*vertix)[I].position[1] = sinf (M_PI * 1.5f + (float)i * M_PI / ((float)bigB - 1.0f)) * Ry + Cy;
		(*vertix)[I].position[2] = d/2.0f;
	}

	tC4 = (*vertices)++;
	(*vertix)[tC4].position[0] = (teethW + left + middle)* t;
	(*vertix)[tC4].position[1] = 0.5f - center * t / 2.0f;
	(*vertix)[tC4].position[2] = d/2.0f;

	// 1
	(*element)[(*elements)].vertices[0] = tA1;
	(*element)[(*elements)].vertices[1] = tA2;
	(*element)[(*elements)].vertices[2] = tA3;
	(*elements)++;

	// 2
	(*element)[(*elements)].vertices[0] = tA1;
	(*element)[(*elements)].vertices[1] = tA3;
	(*element)[(*elements)].vertices[2] = tB1;
	(*elements)++;

	// 3
	(*element)[(*elements)].vertices[0] = tA1;
	(*element)[(*elements)].vertices[1] = tB1;
	(*element)[(*elements)].vertices[2] = tA11;
	(*elements)++;

	// 4
	(*element)[(*elements)].vertices[0] = tA11;
	(*element)[(*elements)].vertices[1] = tB1;
	(*element)[(*elements)].vertices[2] = tB4;
	(*elements)++;

	// 5
	(*element)[(*elements)].vertices[0] = tA3;
	(*element)[(*elements)].vertices[1] = tA4;
	(*element)[(*elements)].vertices[2] = tB1;
	(*elements)++;

	// 6
	(*element)[(*elements)].vertices[0] = tA4;
	(*element)[(*elements)].vertices[1] = tB2;
	(*element)[(*elements)].vertices[2] = tB1;
	(*elements)++;

	// 7
	(*element)[(*elements)].vertices[0] = tA4;
	(*element)[(*elements)].vertices[1] = tC1;
	(*element)[(*elements)].vertices[2] = tB2;
	(*elements)++;

	// 8
	(*element)[(*elements)].vertices[0] = tA4;
	(*element)[(*elements)].vertices[1] = tA5;
	(*element)[(*elements)].vertices[2] = tC1;
	(*elements)++;

	// 9
	(*element)[(*elements)].vertices[0] = tC1;
	(*element)[(*elements)].vertices[1] = tA5;
	(*element)[(*elements)].vertices[2] = tC2;
	(*elements)++;

	// 10
	(*element)[(*elements)].vertices[0] = tB2;
	(*element)[(*elements)].vertices[1] = tC1;
	(*element)[(*elements)].vertices[2] = tB3;
	(*elements)++;

	// 11
	(*element)[(*elements)].vertices[0] = tB3;
	(*element)[(*elements)].vertices[1] = tC1;
	(*element)[(*elements)].vertices[2] = tC4;
	(*elements)++;

	// 12
	(*element)[(*elements)].vertices[0] = tA5;
	(*element)[(*elements)].vertices[1] = tA6;
	(*element)[(*elements)].vertices[2] = tC2;
	(*elements)++;

	// 13
	(*element)[(*elements)].vertices[0] = tA6;
	(*element)[(*elements)].vertices[1] = tA8;
	(*element)[(*elements)].vertices[2] = tC2;
	(*elements)++;

	// 14
	(*element)[(*elements)].vertices[0] = tC2;
	(*element)[(*elements)].vertices[1] = tA8;
	(*element)[(*elements)].vertices[2] = tA9;
	(*elements)++;

	// 15
	(*element)[(*elements)].vertices[0] = tC2;
	(*element)[(*elements)].vertices[1] = tA9;
	(*element)[(*elements)].vertices[2] = tC3;
	(*elements)++;

	// 16
	(*element)[(*elements)].vertices[0] = tA6;
	(*element)[(*elements)].vertices[1] = tA7;
	(*element)[(*elements)].vertices[2] = tA8;
	(*elements)++;

	// 17
	(*element)[(*elements)].vertices[0] = tA10;
	(*element)[(*elements)].vertices[1] = tB3;
	(*element)[(*elements)].vertices[2] = tC4;
	(*elements)++;

	for (i=0; i < (bigB-1); i++)
	{
		(*element)[(*elements)].vertices[0] = tA9 + i;
		(*element)[(*elements)].vertices[1] = tA9 + i + 1;
		(*element)[(*elements)].vertices[2] = tC3 + i;
		(*elements)++;

		(*element)[(*elements)].vertices[0] = tA9 + i + 1;
		(*element)[(*elements)].vertices[1] = tC3 + i + 1;
		(*element)[(*elements)].vertices[2] = tC3 + i;
		(*elements)++;
	}

	for (i=0; i < (bigB-1); i++)
	{
		(*element)[(*elements)].vertices[0] = tA10 + i;
		(*element)[(*elements)].vertices[1] = tA10 + i + 1;
		(*element)[(*elements)].vertices[2] = tB3 + i;
		(*elements)++;

		(*element)[(*elements)].vertices[0] = tA10 + i + 1;
		(*element)[(*elements)].vertices[1] = tB3 + i + 1;
		(*element)[(*elements)].vertices[2] = tB3 + i;
		(*elements)++;
	}

/* back faces */
	int m = *vertices;

	for (int i = 0; i < m; i++)
	{
		int index = (*vertices)++;
		(*vertix)[index].position[0] = (*vertix)[i].position[0];
		(*vertix)[index].position[1] = (*vertix)[i].position[1];
		(*vertix)[index].position[2] = -d/2.0f;
	}

	int n = *elements;
	for (int i = 0; i < n; i++)
	{
		int index = (*elements)++;
		(*element)[index].vertices[0] = (*element)[i].vertices[0] + m;
		(*element)[index].vertices[1] = (*element)[i].vertices[2] + m;
		(*element)[index].vertices[2] = (*element)[i].vertices[1] + m;
	}

/* edges */
	for (int i = 0; i < (m*2); i++) // duplicate again, to make sharp normals
	{
		int index = (*vertices)++;
		(*vertix)[index].position[0] = (*vertix)[i].position[0];
		(*vertix)[index].position[1] = (*vertix)[i].position[1];
		(*vertix)[index].position[2] = (*vertix)[i].position[2];
	}

	for (int i = 0; i <= tA11; i++)
	{
		assert (*elements < total_elements);
		(*element)[(*elements)].vertices[0] = tA1 + i                        + m*2;
		(*element)[(*elements)].vertices[1] = tA1 + i + m                    + m*2;
		(*element)[(*elements)].vertices[2] = tA1 + ((i + 1)%(tA11+1))       + m*2;
		(*elements)++;

		assert (*elements < total_elements);
		(*element)[(*elements)].vertices[0] = tA1 + ((i + 1)%(tA11+1))       + m*2;
		(*element)[(*elements)].vertices[1] = tA1 + i + m                    + m*2;
		(*element)[(*elements)].vertices[2] = tA1 + ((i + 1)%(tA11+1)) + m   + m*2;
		(*elements)++;
	}

	for (int i = 0; i < (tC1 - tB1); i++)
	{
		assert (*elements < total_elements);
		(*element)[(*elements)].vertices[0] = tB1 + i                        + m*2;
		(*element)[(*elements)].vertices[2] = tB1 + i + m                    + m*2;
		(*element)[(*elements)].vertices[1] = tB1 + ((i+1)%(tB4 - tA11))     + m*2;
		(*elements)++;

		assert (*elements < total_elements);
		(*element)[(*elements)].vertices[0] = tB1 + ((i+1)%(tB4 - tA11))     + m*2;
		(*element)[(*elements)].vertices[2] = tB1 + i + m                    + m*2;
		(*element)[(*elements)].vertices[1] = tB1 + ((i+1)%(tB4 - tA11)) + m + m*2;
		(*elements)++;
	}

	for (int i = 0; i < (m - tC1); i++)
	{
		assert (*elements < total_elements);
		(*element)[(*elements)].vertices[0] = tC1 + i                        + m*2;
		(*element)[(*elements)].vertices[2] = tC1 + i + m                    + m*2;
		(*element)[(*elements)].vertices[1] = tC1 + ((i+1)%(tC4 - tB4))      + m*2;
		(*elements)++;

		assert (*elements < total_elements);
		(*element)[(*elements)].vertices[0] = tC1 + ((i+1)%(tC4 - tB4))      + m*2;
		(*element)[(*elements)].vertices[2] = tC1 + i + m                    + m*2;
		(*element)[(*elements)].vertices[1] = tC1 + ((i+1)%(tC4 - tB4)) + m  + m*2;
		(*elements)++;
	}

#undef teethW
#undef right
#undef outerX
#undef innerX
#undef left
#undef middle
#undef bottom
#undef top
#undef center
#undef teethR
#undef bigB
}

void I(struct vertix_t **vertix, int *vertices, struct element_t **element, int *elements)
{
#define teethW  1.50
#define left    1.50
#define bottom  0.75
#define top     0.75
#define teethR  0.10

	int tA1,  tA2,  tA3,  tA4,
	    tA5,  tA6,  tA7,  tA8,
	    tA9,  tA10, tA11, tA12,
	    tA13, tA14, tA15, tA16;

	int total_vertices = 16 * 4 /* front and sides do not share vertices, since we want sharp normals at those edges */;
	int total_elements = 10 * 2 + 16 * 2;

	*elements = 0;
	*vertices = 0;

	*vertix = (struct vertix_t *)malloc (sizeof ((*vertix)[0]) * total_vertices);
	*element = (struct element_t *)malloc (sizeof ((*element)[0]) * total_elements);

	tA1 = (*vertices)++;
	(*vertix)[tA1].position[0] = 0.0f;
	(*vertix)[tA1].position[1] = 1.0f;
	(*vertix)[tA1].position[2] = d/2.0f;

	tA2 = (*vertices)++;
	(*vertix)[tA2].position[0] = 0.0f;
	(*vertix)[tA2].position[1] = 1.0f - top * t;
	(*vertix)[tA2].position[2] = d/2.0f;

	tA3 = (*vertices)++;
	(*vertix)[tA3].position[0] = (teethW - teethR) * t;
	(*vertix)[tA3].position[1] = 1.0f - top * t;
	(*vertix)[tA3].position[2] = d/2.0f;

	tA4 = (*vertices)++;
	(*vertix)[tA4].position[0] = teethW * t;
	(*vertix)[tA4].position[1] = 1.0f - (top + teethR ) * t;
	(*vertix)[tA4].position[2] = d/2.0f;

	tA5 = (*vertices)++;
	(*vertix)[tA5].position[0] = teethW * t;
	(*vertix)[tA5].position[1] = (bottom + teethR) * t;
	(*vertix)[tA5].position[2] = d/2.0f;

	tA6 = (*vertices)++;
	(*vertix)[tA6].position[0] = (teethW - teethR) * t;
	(*vertix)[tA6].position[1] = bottom * t;
	(*vertix)[tA6].position[2] = d/2.0f;

	tA7 = (*vertices)++;
	(*vertix)[tA7].position[0] = 0.0f;
	(*vertix)[tA7].position[1] = bottom * t;
	(*vertix)[tA7].position[2] = d/2.0f;

	tA8 = (*vertices)++;
	(*vertix)[tA8].position[0] = 0.0f;
	(*vertix)[tA8].position[1] = 0.0f;
	(*vertix)[tA8].position[2] = d/2.0f;

	tA9 = (*vertices)++;
	(*vertix)[tA9].position[0] = (teethW + left + teethW) * t;
	(*vertix)[tA9].position[1] = 0.0f;
	(*vertix)[tA9].position[2] = d/2.0f;

	tA10 = (*vertices)++;
	(*vertix)[tA10].position[0] = (teethW + left + teethW) * t;
	(*vertix)[tA10].position[1] = bottom * t;
	(*vertix)[tA10].position[2] = d/2.0f;

	tA11 = (*vertices)++;
	(*vertix)[tA11].position[0] = (teethW + left + teethR) * t;
	(*vertix)[tA11].position[1] = bottom * t;
	(*vertix)[tA11].position[2] = d/2.0f;

	tA12 = (*vertices)++;
	(*vertix)[tA12].position[0] = (teethW + left) * t;
	(*vertix)[tA12].position[1] = (bottom + teethR) * t;
	(*vertix)[tA12].position[2] = d/2.0f;

	tA13 = (*vertices)++;
	(*vertix)[tA13].position[0] = (teethW + left) * t;
	(*vertix)[tA13].position[1] = 1.0f - (top + teethR) * t;
	(*vertix)[tA13].position[2] = d/2.0f;

	tA14 = (*vertices)++;
	(*vertix)[tA14].position[0] = (teethW + left + teethR) * t;
	(*vertix)[tA14].position[1] = 1.0f - top * t;
	(*vertix)[tA14].position[2] = d/2.0f;

	tA15 = (*vertices)++;
	(*vertix)[tA15].position[0] = (teethW + left + teethW) * t;
	(*vertix)[tA15].position[1] = 1.0f - top * t;
	(*vertix)[tA15].position[2] = d/2.0f;

	tA16 = (*vertices)++;
	(*vertix)[tA16].position[0] = (teethW + left + teethW) * t;
	(*vertix)[tA16].position[1] = 1.0f;
	(*vertix)[tA16].position[2] = d/2.0f;


	// 1
	(*element)[(*elements)].vertices[0] = tA1;
	(*element)[(*elements)].vertices[1] = tA2;
	(*element)[(*elements)].vertices[2] = tA16;
	(*elements)++;

	// 2
	(*element)[(*elements)].vertices[0] = tA16;
	(*element)[(*elements)].vertices[1] = tA2;
	(*element)[(*elements)].vertices[2] = tA15;
	(*elements)++;

	// 3
	(*element)[(*elements)].vertices[0] = tA3;
	(*element)[(*elements)].vertices[1] = tA4;
	(*element)[(*elements)].vertices[2] = tA14;
	(*elements)++;

	// 4
	(*element)[(*elements)].vertices[0] = tA14;
	(*element)[(*elements)].vertices[1] = tA4;
	(*element)[(*elements)].vertices[2] = tA13;
	(*elements)++;

	// 5
	(*element)[(*elements)].vertices[0] = tA4;
	(*element)[(*elements)].vertices[1] = tA5;
	(*element)[(*elements)].vertices[2] = tA13;
	(*elements)++;

	// 6
	(*element)[(*elements)].vertices[0] = tA13;
	(*element)[(*elements)].vertices[1] = tA5;
	(*element)[(*elements)].vertices[2] = tA12;
	(*elements)++;

	// 7
	(*element)[(*elements)].vertices[0] = tA5;
	(*element)[(*elements)].vertices[1] = tA6;
	(*element)[(*elements)].vertices[2] = tA12;
	(*elements)++;

	// 8
	(*element)[(*elements)].vertices[0] = tA12;
	(*element)[(*elements)].vertices[1] = tA6;
	(*element)[(*elements)].vertices[2] = tA11;
	(*elements)++;

	// 9
	(*element)[(*elements)].vertices[0] = tA7;
	(*element)[(*elements)].vertices[1] = tA8;
	(*element)[(*elements)].vertices[2] = tA10;
	(*elements)++;

	// 10
	(*element)[(*elements)].vertices[0] = tA10;
	(*element)[(*elements)].vertices[1] = tA8;
	(*element)[(*elements)].vertices[2] = tA9;
	(*elements)++;

/* back faces */
	int m = *vertices;

	for (int i = 0; i < m; i++)
	{
		int index = (*vertices)++;
		(*vertix)[index].position[0] = (*vertix)[i].position[0];
		(*vertix)[index].position[1] = (*vertix)[i].position[1];
		(*vertix)[index].position[2] = -d/2.0f;
	}

	int n = *elements;
	for (int i = 0; i < n; i++)
	{
		int index = (*elements)++;
		(*element)[index].vertices[0] = (*element)[i].vertices[0] + m;
		(*element)[index].vertices[1] = (*element)[i].vertices[2] + m;
		(*element)[index].vertices[2] = (*element)[i].vertices[1] + m;
	}

/* edges */
	for (int i = 0; i < (m*2); i++) // duplicate again, to make sharp normals
	{
		int index = (*vertices)++;
		(*vertix)[index].position[0] = (*vertix)[i].position[0];
		(*vertix)[index].position[1] = (*vertix)[i].position[1];
		(*vertix)[index].position[2] = (*vertix)[i].position[2];
	}

	for (int i = 0; i < m; i++)
	{
		(*element)[(*elements)].vertices[0] = ((tA1 + i)%m)     + m*2;
		(*element)[(*elements)].vertices[1] = ((tA1 + i)%m) + m + m*2;
		(*element)[(*elements)].vertices[2] = ((tA2 + i)%m)     + m*2;
		(*elements)++;

		(*element)[(*elements)].vertices[0] = ((tA2 + i)%m)     + m*2;
		(*element)[(*elements)].vertices[1] = ((tA1 + i)%m) + m + m*2;
		(*element)[(*elements)].vertices[2] = ((tA2 + i)%m) + m + m*2;
		(*elements)++;
	}
#undef teethW
#undef left
#undef bottom
#undef top
#undef teethR
}



void generate_normals (const int count, const int elements_count)
{
	for (int i = 0; i < count; i++)
	{
//		fprintf (stderr, "(prepare vertex %d)\n", i);
		glm::vec3 norm(0.0f);
		for (int j=0; j < elements_count; j++)
		{
			if ((elements[j].vertices[0] == i) ||
			    (elements[j].vertices[1] == i) ||
			    (elements[j].vertices[2] == i))
			{
//				fprintf (stderr, "use element %d  %d %d %d    %f %f %f   %f %f %f   %f %f %f\n", j, elements[j].vertices[0], elements[j].vertices[1], elements[j].vertices[2], vertices[elements[j].vertices[0]].position[0], vertices[elements[j].vertices[0]].position[1], vertices[elements[j].vertices[0]].position[2], vertices[elements[j].vertices[1]].position[0], vertices[elements[j].vertices[1]].position[1], vertices[elements[j].vertices[1]].position[2], vertices[elements[j].vertices[2]].position[0], vertices[elements[j].vertices[2]].position[1], vertices[elements[j].vertices[2]].position[2]);
				glm::vec3 p0 = glm::vec3(vertices[elements[j].vertices[0]].position[0],
				                         vertices[elements[j].vertices[0]].position[1],
				                         vertices[elements[j].vertices[0]].position[2]);
				glm::vec3 p1 = glm::vec3(vertices[elements[j].vertices[1]].position[0],
				                         vertices[elements[j].vertices[1]].position[1],
				                         vertices[elements[j].vertices[1]].position[2]);
				glm::vec3 p2 = glm::vec3(vertices[elements[j].vertices[2]].position[0],
				                         vertices[elements[j].vertices[2]].position[1],
				                         vertices[elements[j].vertices[2]].position[2]);

				glm::vec3 e1 = p0 - p1;
				glm::vec3 e2 = p2 - p1;
				norm -= glm::cross(e1, e2);
			}
		}
		assert (norm != glm::vec3(0.0f));
		norm = glm::normalize (norm);
		vertices[i].normal[0] = norm.x;
		vertices[i].normal[1] = norm.y;
		vertices[i].normal[2] = norm.z;
//		fprintf (stderr, "vertex %d normal %f %f %f\n", i, norm.x, norm.y, norm.z);
	}
}

static void center_align (struct vertix_t *vertix, int vertices)
{
	float minx = 10.0f;
	float maxx = -10.0f;
//	float miny = 10.0f;
//	float maxy = -10.0f;

	float offsetx;
	float offsety = -0.5;

	for (int i = 0; i < vertices; i++)
	{
		if (vertix[i].position[0] < minx) minx = vertix[i].position[0];
		if (vertix[i].position[0] > maxx) maxx = vertix[i].position[0];
//		if (vertix[i].position[1] < miny) miny = vertix[i].position[1];
//		if (vertix[i].position[1] > maxy) maxy = vertix[i].position[1];
	}

	offsetx = (minx + maxx) / -2.0f;
//	offsety = (miny + maxy) / -2.0f;

	for (int i = 0; i < vertices; i++)
	{
		vertix[i].position[0] += offsetx;
		vertix[i].position[1] += offsety;
	}
}

void magic(void (*foo)(struct vertix_t **vertix, int *vertices, struct element_t **element, int *elements), char L)
{
	int vindex = 0;
	int tindex = 0;

	vertices = 0;
	elements = 0;
	vindex = 0;
	tindex = 0;
	foo(&vertices, &vindex, &elements, &tindex);
	center_align (vertices, vindex);
	generate_normals (vindex, tindex);

	printf ("#ifdef HEADER\n");
	printf ("static const struct vertix_t vertices_%c[%d];\n", L, vindex);
	printf ("static const struct element_t elements_%c[%d];\n", L, tindex);
	printf ("#else\n");

	printf ("\nconst struct wurfel_cubic::vertix_t wurfel_cubic::vertices_%c[%d] =\n", L, vindex);
	printf ("{\n");
	for (int i=0; i < vindex; i++)
	{
		printf ("\t/* %4d */ {{%f, %f, %f}, {%f, %f, %f}},\n", i,
			vertices[i].position[0],
			vertices[i].position[1],
			vertices[i].position[2],
			vertices[i].normal[0],
			vertices[i].normal[1],
			vertices[i].normal[2]);
	}
	printf ("};\n");
	printf ("\nconst struct wurfel_cubic::element_t wurfel_cubic::elements_%c[%d] =\n", L, tindex);
	printf ("{\n");
	for (int i=0; i < tindex; i++)
 	{
		printf ("\t/* %4d */ {%4d, %4d, %4d},\n", i,
			elements[i].vertices[0],
			elements[i].vertices[1],
			elements[i].vertices[2]);
	}
	printf ("};\n");
	printf ("#endif\n");

	free (elements);
	free (vertices);
	elements = 0;
	vertices = 0;
}

int main (int argc, char *argv[])
{

	printf ("/* generated by %s */\n", argv[0]);

	magic (C, 'C');
	magic (U, 'U');
	magic (B, 'B');
	magic (I, 'I');

	return 0;
}
