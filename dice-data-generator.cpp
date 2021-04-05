#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <assert.h>
#include <math.h>
#include <stdio.h>

#define BORDER 0.1f
#define LEVELS 4
#define DEPTH 0.3
#define RADIUS 0.4

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

static const unsigned int hole_total_vertices (void)
{
	unsigned int retval = 0;
	unsigned int prev = 3;
	unsigned int prevprev = 3;

	/* we start with a single triangle, then for each level we double the count of verticals per circel-level */
	for (int i = 0; i < LEVELS; i++)
	{
		retval += prev;
		prevprev = prev;
		prev *= 2;
	}
	retval += prevprev; /* make an extra circle at the very end to solve normals not being even above the edge */
	return retval;
}

static const unsigned int hole_total_elements (void)
{
	unsigned int retval = 1; /* bottom */
	unsigned int vertices = 3;
	fprintf (stderr, "\n");
	fprintf (stderr, " base=%d\n", retval);
	for (int i = 1; i < LEVELS; i++)
	{
		retval += vertices * 3; /* each ring connects a square and a triangle to the previous circles nodes */
		fprintf (stderr, " ++ %d * 3 = %d  => retval=%d\n", vertices, vertices * 3, retval);
		vertices *= 2;
	}

	retval += vertices * 2; /* make an extra circle at the very end to solve normals not being even above the edge */
	fprintf (stderr, " +ring %d *2 = %d => retval=%d\n", vertices, vertices*2, retval);
	retval += 4 + vertices; /* border surrounding the hole */
	fprintf (stderr, " +BORDER=%d  => retval=%d \n", 4 + vertices, retval);

	return retval;
}

unsigned int side_total_vertices (const uint_fast16_t dots)
{
	return 4 + 16 +
		((dots&0x001)?hole_total_vertices():0) +
		((dots&0x002)?hole_total_vertices():0) +
		((dots&0x005)?hole_total_vertices():0) +
		((dots&0x008)?hole_total_vertices():0) +
		((dots&0x010)?hole_total_vertices():0) +
		((dots&0x020)?hole_total_vertices():0) +
		((dots&0x040)?hole_total_vertices():0) +
		((dots&0x080)?hole_total_vertices():0) +
		((dots&0x100)?hole_total_vertices():0);
}

unsigned int side_total_triangles (const uint_fast16_t dots)
{
	return 16 +
		((dots&0x001)?hole_total_elements():2) +
		((dots&0x002)?hole_total_elements():2) +
		((dots&0x005)?hole_total_elements():2) +
		((dots&0x008)?hole_total_elements():2) +
		((dots&0x010)?hole_total_elements():2) +
		((dots&0x020)?hole_total_elements():2) +
		((dots&0x040)?hole_total_elements():2) +
		((dots&0x080)?hole_total_elements():2) +
		((dots&0x100)?hole_total_elements():2);
}

void hole_make_ring (
	struct vertix_t *vertices, const int vindex, const int count,
	const float radius,
	const float centerx, const float centery, const float centerz,
	const float  xaxisx,  const float xaxisy, const float  xaxisz,
	const float  yaxisx,  const float yaxisy, const float  yaxisz,
	const float  depthx,  const float depthy, const float  depthz
)
{
	int i;
	fprintf (stderr, "ring... count=%d\n", count);
	for (i = 0; i < count; i++)
	{
		vertices[(vindex+i)].position[0] = centerx + sinf ( 2.0f * M_PI * (float)i / (float)count ) * xaxisx * radius + cosf ( 2.0f * M_PI * (float)i / (float)count ) * yaxisx * radius + depthx;
		vertices[(vindex+i)].position[1] = centery + sinf ( 2.0f * M_PI * (float)i / (float)count ) * xaxisy * radius + cosf ( 2.0f * M_PI * (float)i / (float)count ) * yaxisy * radius + depthy;
		vertices[(vindex+i)].position[2] = centerz + sinf ( 2.0f * M_PI * (float)i / (float)count ) * xaxisz * radius + cosf ( 2.0f * M_PI * (float)i / (float)count ) * yaxisz * radius + depthz;
		fprintf (stderr, "[%d/%d => %d] = {%f %f %f}\n", i+1, count, vindex + i, vertices[(vindex+i)].position[0], vertices[(vindex+i)].position[1], vertices[(vindex+i)].position[2]);
	}
}

void punch_hole (
	struct vertix_t *vertices,
	struct element_t *triangles,
	int *vindex,
	int *tindex,
	int p0, int p1, int p2, int p3,
	float depthx, float depthy, float depthz)
{
	float centerx = (vertices[p0].position[0] + vertices[p2].position[0]) / 2.0f;
	float centery = (vertices[p0].position[1] + vertices[p2].position[1]) / 2.0f;
	float centerz = (vertices[p0].position[2] + vertices[p2].position[2]) / 2.0f;

	float xaxisx = (vertices[p1].position[0] - vertices[p0].position[0]);
	float xaxisy = (vertices[p1].position[1] - vertices[p0].position[1]);
	float xaxisz = (vertices[p1].position[2] - vertices[p0].position[2]);

	float yaxisx = (vertices[p3].position[0] - vertices[p0].position[0]);
	float yaxisy = (vertices[p3].position[1] - vertices[p0].position[1]);
	float yaxisz = (vertices[p3].position[2] - vertices[p0].position[2]);
	int prevcount = 0;
	int previndex = *vindex;

	int thiscount = 0;

	for (int ring = 1; ring <= LEVELS; ring++)
	{
		float depth = /*cos((float)ring * M_PI_2 / LEVELS)*/ (float)(LEVELS-ring) / LEVELS * DEPTH;
		float radius = sin((float)ring * M_PI_2 / LEVELS) * RADIUS;
		thiscount = prevcount ? (prevcount) * 2 : 3;

		hole_make_ring (
			vertices, *vindex, thiscount,
			radius,
			centerx,         centery,         centerz,
			 xaxisx,          xaxisy,          xaxisz,
			 yaxisx,          yaxisy,          yaxisz,
			 depthx * depth,  depthy * depth,  depthz * depth);

		if (!prevcount)
		{
			triangles[(*tindex)].vertices[2] = (*vindex);
			triangles[(*tindex)].vertices[1] = (*vindex)+1;
			triangles[(*tindex)].vertices[0] = (*vindex)+2;
			(*tindex)++;

			fprintf (stderr, "AAAH(%d) %d %d %d\n", *tindex, (*vindex), (*vindex)+1, (*vindex)+2);
		} else {
			for (int i = 0; i < prevcount; i++)
			{
				triangles[(*tindex)].vertices[2] = previndex +                 i  + 0;
				triangles[(*tindex)].vertices[1] = (*vindex) + ((thiscount + 2*i  - 1) % thiscount);
				triangles[(*tindex)].vertices[0] = (*vindex) +               2*i  + 0;
				fprintf (stderr, "BBBH(%d) %d %d %d\n", *tindex, triangles[(*tindex)].vertices[0],
				                                                 triangles[(*tindex)].vertices[1],
				                                                 triangles[(*tindex)].vertices[2]);
				(*tindex)++;

				triangles[(*tindex)].vertices[2] = previndex + i    + 0;
				triangles[(*tindex)].vertices[1] = (*vindex) + i*2  + 0;
				triangles[(*tindex)].vertices[0] = (*vindex) + i*2  + 1;
				fprintf (stderr, " BBH(%d) %d %d %d\n", *tindex, triangles[(*tindex)].vertices[0],
				                                                 triangles[(*tindex)].vertices[1],
				                                                 triangles[(*tindex)].vertices[2]);
				(*tindex)++;

				triangles[(*tindex)].vertices[2] = previndex +     i  + 0;
				triangles[(*tindex)].vertices[1] = (*vindex) +   2*i  + 1;
				triangles[(*tindex)].vertices[0] = previndex + ((  i  + 1) % prevcount);
				fprintf (stderr, "  BH(%d) %d %d %d\n", *tindex, triangles[(*tindex)].vertices[0],
				                                                 triangles[(*tindex)].vertices[1],
				                                                 triangles[(*tindex)].vertices[2]);
				(*tindex)++;

			}
		}

		previndex = *vindex;
		(*vindex) += thiscount;
		prevcount = thiscount;
	}

	/* make an extra circle around the hole to fix normal transitions */
	{
		hole_make_ring (
			vertices, *vindex, thiscount,
			RADIUS*1.10,
			centerx,         centery,         centerz,
			 xaxisx,          xaxisy,          xaxisz,
			 yaxisx,          yaxisy,          yaxisz,
			   0.0f,            0.0f,            0.0f);

		for (int i = 0; i < prevcount; i++)
		{
			/* inner, outer, outer - 1 */
			triangles[(*tindex)].vertices[2] = previndex +                 i  + 0;
			triangles[(*tindex)].vertices[1] = (*vindex) + ((thiscount +   i  - 1) % thiscount);
			triangles[(*tindex)].vertices[0] = (*vindex) +                 i  + 0;
			fprintf (stderr, "___a(%d) %d %d %d\n", *tindex, triangles[(*tindex)].vertices[0],
			                                                 triangles[(*tindex)].vertices[1],
			                                                 triangles[(*tindex)].vertices[2]);
			(*tindex)++;

			/* inner, inner-1, outer-1 */
			triangles[(*tindex)].vertices[2] = previndex +              i + 0;
			triangles[(*tindex)].vertices[1] = previndex + (thiscount + i - 1) % thiscount;
			triangles[(*tindex)].vertices[0] = (*vindex) + (thiscount + i - 1) % thiscount;
			fprintf (stderr, " __b(%d) %d %d %d\n", *tindex, triangles[(*tindex)].vertices[0],
			                                                 triangles[(*tindex)].vertices[1],
			                                                 triangles[(*tindex)].vertices[2]);
			(*tindex)++;
		}

		previndex = *vindex;
		(*vindex) += thiscount;
	}

	int i = 0;

	triangles[(*tindex)].vertices[0] = p2;
	triangles[(*tindex)].vertices[1] = p3;
	triangles[(*tindex)].vertices[2] = previndex;
	fprintf (stderr, "North(%d) %d %d %d\n", *tindex, triangles[(*tindex)].vertices[0],
	                                                  triangles[(*tindex)].vertices[1],
	                                                  triangles[(*tindex)].vertices[2]);
	(*tindex)++;

	for (; i < ((1*prevcount) / 4); i++)
	{
		triangles[(*tindex)].vertices[0] = previndex+i;
		triangles[(*tindex)].vertices[1] = previndex+i+1;
		triangles[(*tindex)].vertices[2] = p2;
		fprintf (stderr, " (%d) %d %d %d\n", *tindex, triangles[(*tindex)].vertices[0],
		                                              triangles[(*tindex)].vertices[1],
		                                              triangles[(*tindex)].vertices[2]);
		(*tindex)++;
	}

	triangles[(*tindex)].vertices[0] = p1;
	triangles[(*tindex)].vertices[1] = p2;
	triangles[(*tindex)].vertices[2] = previndex + (1*prevcount/4);
	fprintf (stderr, "East(%d) %d %d %d\n", *tindex, triangles[(*tindex)].vertices[0],
	                                                 triangles[(*tindex)].vertices[1],
	                                                 triangles[(*tindex)].vertices[2]);
	(*tindex)++;

	for (; i < ((2*prevcount) / 4); i++)
	{
		triangles[(*tindex)].vertices[0] = previndex+i;
		triangles[(*tindex)].vertices[1] = previndex+i+1;
		triangles[(*tindex)].vertices[2] = p1;
		fprintf (stderr, " (%d) %d %d %d\n", *tindex, triangles[(*tindex)].vertices[0],
		                                              triangles[(*tindex)].vertices[1],
		                                              triangles[(*tindex)].vertices[2]);
		(*tindex)++;
	}

	triangles[(*tindex)].vertices[0] = p0;
	triangles[(*tindex)].vertices[1] = p1;
	triangles[(*tindex)].vertices[2] = previndex + (2*prevcount/4);
	fprintf (stderr, "South(%d) %d %d %d\n", *tindex, triangles[(*tindex)].vertices[0],
	                                                  triangles[(*tindex)].vertices[1],
	                                                  triangles[(*tindex)].vertices[2]);
	(*tindex)++;

	for (; i < ((3*prevcount) / 4); i++)
	{
		triangles[(*tindex)].vertices[0] = previndex+i;
		triangles[(*tindex)].vertices[1] = previndex+i+1;
		triangles[(*tindex)].vertices[2] = p0;
		fprintf (stderr, " (%d) %d %d %d\n", *tindex, triangles[(*tindex)].vertices[0],
		                                              triangles[(*tindex)].vertices[1],
		                                              triangles[(*tindex)].vertices[2]);
		(*tindex)++;
	}

	triangles[(*tindex)].vertices[0] = p3;
	triangles[(*tindex)].vertices[1] = p0;
	triangles[(*tindex)].vertices[2] = previndex + (3*prevcount/4);
	fprintf (stderr, "West(%d) %d %d %d\n", *tindex, triangles[(*tindex)].vertices[0],
	                                                 triangles[(*tindex)].vertices[1],
	                                                 triangles[(*tindex)].vertices[2]);
	(*tindex)++;

	for (; i < ((4*prevcount) / 4); i++)
	{
		triangles[(*tindex)].vertices[0] = previndex+i;
		triangles[(*tindex)].vertices[1] = previndex+((i+1)%prevcount);
		triangles[(*tindex)].vertices[2] = p3;
		fprintf (stderr, " (%d) %d %d %d\n", *tindex, triangles[(*tindex)].vertices[0],
		                                              triangles[(*tindex)].vertices[1],
		                                              triangles[(*tindex)].vertices[2]);
		(*tindex)++;
	}

}

void cube_fill(
	const uint_fast16_t dots,
	struct vertix_t *vertices,
	struct element_t *triangles,
	int *vindex,
	int *tindex,
	const float *_p0, const float *_p1, const float *_p2, const float *_p3)
{
	int p0, p1, p2, p3;
	int a[4][4];
	int i, j, n;

	/* outer extreme corners */
	p0 = (*vindex)++;
	p1 = (*vindex)++;
	p2 = (*vindex)++;
	p3 = (*vindex)++;

	vertices[p0].position[0] = _p0[0];
	vertices[p0].position[1] = _p0[1];
	vertices[p0].position[2] = _p0[2];

	vertices[p1].position[0] = _p1[0];
	vertices[p1].position[1] = _p1[1];
	vertices[p1].position[2] = _p1[2];

	vertices[p2].position[0] = _p2[0];
	vertices[p2].position[1] = _p2[1];
	vertices[p2].position[2] = _p2[2];

	vertices[p3].position[0] = _p3[0];
	vertices[p3].position[1] = _p3[1];
	vertices[p3].position[2] = _p3[2];

  /* make a 4x4 grid inside of the border area */
	for (i=0; i < 4; i++)
	{
		for (j=0; j < 4; j++)
		{
			for (n=0; n < 3; n++) /* x, y, z space */
			{
				vertices[(*vindex)].position[n] = vertices[p0].position[n] +
				                                 (vertices[p1].position[n] - vertices[p0].position[n]) * BORDER +
				                                 (vertices[p3].position[n] - vertices[p0].position[n]) * BORDER +
				                                 (vertices[p1].position[n] - vertices[p0].position[n]) * (1.0f - BORDER - BORDER) * i / 3 +
				                                 (vertices[p3].position[n] - vertices[p0].position[n]) * (1.0f - BORDER - BORDER) * j / 3;
			}
			/* store the index */
			a[j][i] = (*vindex)++;
		}
	}

	/* BORDER edge 1 */
	triangles[(*tindex)].vertices[0] = a[0][0];
	triangles[(*tindex)].vertices[1] = p0; 
	triangles[(*tindex)].vertices[2] = p1;
	(*tindex)++;

	triangles[(*tindex)].vertices[0] = p1;
	triangles[(*tindex)].vertices[1] = a[0][3];
	triangles[(*tindex)].vertices[2] = a[0][2];
	(*tindex)++;

	triangles[(*tindex)].vertices[0] = p1;
	triangles[(*tindex)].vertices[1] = a[0][2];
	triangles[(*tindex)].vertices[2] = a[0][1];
	(*tindex)++;

	triangles[(*tindex)].vertices[0] = p1;
	triangles[(*tindex)].vertices[1] = a[0][1];
	triangles[(*tindex)].vertices[2] = a[0][0];
	(*tindex)++;

	/* BORDER edge 2 */
	triangles[(*tindex)].vertices[0] = a[0][3];
	triangles[(*tindex)].vertices[1] = p1;
	triangles[(*tindex)].vertices[2] = p2;
	(*tindex)++;

	triangles[(*tindex)].vertices[0] = p2;
	triangles[(*tindex)].vertices[1] = a[3][3];
	triangles[(*tindex)].vertices[2] = a[2][3];
	(*tindex)++;

	triangles[(*tindex)].vertices[0] = p2;
	triangles[(*tindex)].vertices[1] = a[2][3];
	triangles[(*tindex)].vertices[2] = a[1][3];
	(*tindex)++;

	triangles[(*tindex)].vertices[0] = p2;
	triangles[(*tindex)].vertices[1] = a[1][3];
	triangles[(*tindex)].vertices[2] = a[0][3];
	(*tindex)++;

	/* BORDER edge 3 */
	triangles[(*tindex)].vertices[0] = a[3][3];
	triangles[(*tindex)].vertices[1] = p2;
	triangles[(*tindex)].vertices[2] = p3;
	(*tindex)++;

	triangles[(*tindex)].vertices[0] = p3;
	triangles[(*tindex)].vertices[1] = a[3][0];
	triangles[(*tindex)].vertices[2] = a[3][1];
	(*tindex)++;

	triangles[(*tindex)].vertices[0] = p3;
	triangles[(*tindex)].vertices[1] = a[3][1];
	triangles[(*tindex)].vertices[2] = a[3][2];
	(*tindex)++;

	triangles[(*tindex)].vertices[0] = p3;
	triangles[(*tindex)].vertices[1] = a[3][2];
	triangles[(*tindex)].vertices[2] = a[3][3];
	(*tindex)++;

	/* BORDER edge 4 */
	triangles[(*tindex)].vertices[0] = a[3][0];
	triangles[(*tindex)].vertices[1] = p3;
	triangles[(*tindex)].vertices[2] = p0;
	(*tindex)++;

	triangles[(*tindex)].vertices[0] = p0;
	triangles[(*tindex)].vertices[1] = a[0][0];
	triangles[(*tindex)].vertices[2] = a[1][0];
	(*tindex)++;

	triangles[(*tindex)].vertices[0] = p0;
	triangles[(*tindex)].vertices[1] = a[1][0];
	triangles[(*tindex)].vertices[2] = a[2][0];
	(*tindex)++;

	triangles[(*tindex)].vertices[0] = p0;
	triangles[(*tindex)].vertices[1] = a[2][0];
	triangles[(*tindex)].vertices[2] = a[3][0];
	(*tindex)++;

  /* inside the grid, makes flat or punched in surfaces */
	for (i=0; i < 3; i++)
	{
		for (j=0; j < 3; j++)
		{
			if (dots & (0x1<<(i+j*3)))
			{
				punch_hole (vertices, triangles, vindex, tindex, a[j][i], a[j][i+1], a[j+1][i+1], a[j+1][i],
					/* depth, by doing an average of the outer diagonal corners of the dice. Model is symetrical, so X,Y axis for that face will even out, while Z will be doubled up */
					(vertices[p0].position[0] + vertices[p2].position[0]) / -2.0f,
					(vertices[p0].position[1] + vertices[p2].position[1]) / -2.0f,
					(vertices[p0].position[2] + vertices[p2].position[2]) / -2.0f);
			} else {
				/* no dot */
				triangles[(*tindex)].vertices[0] = a[  j][i  ];
				triangles[(*tindex)].vertices[1] = a[j  ][i+1];
				triangles[(*tindex)].vertices[2] = a[j+1][i+1];
				(*tindex)++;

				triangles[(*tindex)].vertices[0] = a[  j][i  ];
				triangles[(*tindex)].vertices[1] = a[j+1][i+1];
				triangles[(*tindex)].vertices[2] = a[j+1][i  ];
				(*tindex)++;
			}
		}
	}
}

void generate_normals (const int count, const int elements_count)
{
	for (int i = 0; i < count; i++)
	{
		glm::vec3 norm(0.0f);
		for (int j=0; j < elements_count; j++)
		{
			if ((elements[j].vertices[0] == i) ||
			    (elements[j].vertices[1] == i) ||
			    (elements[j].vertices[2] == i))
			{
				fprintf (stderr, "use element %d\n", j);
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
				norm += glm::cross(e1, e2);
			}
		}
		assert (norm != glm::vec3(0.0f));
		norm = glm::normalize (norm);
		vertices[i].normal[0] = norm.x;
		vertices[i].normal[1] = norm.y;
		vertices[i].normal[2] = norm.z;
		fprintf (stderr, "vertex %d normal %f %f %f\n", i, norm.x, norm.y, norm.z);
	}
}

int main (int argc, char *argv[])
{
	int MAXVERTICALS = /* 8 +*/
		side_total_vertices (0x010) +  /* 1 */
		side_total_vertices (0x16d) +  /* 6 */
		side_total_vertices (0x044) +  /* 2 */
		side_total_vertices (0x155) +  /* 5 */
		side_total_vertices (0x054) +  /* 3 */
		side_total_vertices (0x145);
	int MAXTRIANGLES =
		side_total_triangles (0x010) +  /* 1 */
		side_total_triangles (0x16d) +  /* 6 */
		side_total_triangles (0x044) +  /* 2 */
		side_total_triangles (0x155) +  /* 5 */
		side_total_triangles (0x054) +  /* 3 */
		side_total_triangles (0x145);

	fprintf (stderr, "MAXVERTICALS=%d\n", MAXVERTICALS);
	fprintf (stderr, "MAXTRIANGLES=%d\n", MAXTRIANGLES);

	vertices = (struct vertix_t  *)malloc (sizeof (vertices[0]) * MAXVERTICALS);
	elements = (struct element_t *)malloc (sizeof (elements[0]) * MAXTRIANGLES);

	int vindex = 0;
	int tindex = 0;

	const static float cube_vertices_template[]
	{
	        -0.5f,  0.5f, -0.5f,
	         0.5f,  0.5f, -0.5f,
	         0.5f, -0.5f, -0.5f,
	        -0.5f, -0.5f, -0.5f,
	        -0.5f,  0.5f,  0.5f,
	         0.5f,  0.5f,  0.5f,
	         0.5f, -0.5f,  0.5f,
	        -0.5f, -0.5f,  0.5f,
	};

	cube_fill (0x010, vertices, elements, &vindex, &tindex, &cube_vertices_template[0*3], &cube_vertices_template[1*3], &cube_vertices_template[2*3], &cube_vertices_template[3*3]); /* 1 */
	cube_fill (0x16d, vertices, elements, &vindex, &tindex, &cube_vertices_template[7*3], &cube_vertices_template[6*3], &cube_vertices_template[5*3], &cube_vertices_template[4*3]); /* 6 */
	cube_fill (0x044, vertices, elements, &vindex, &tindex, &cube_vertices_template[3*3], &cube_vertices_template[2*3], &cube_vertices_template[6*3], &cube_vertices_template[7*3]); /* 2 */
	cube_fill (0x155, vertices, elements, &vindex, &tindex, &cube_vertices_template[1*3], &cube_vertices_template[0*3], &cube_vertices_template[4*3], &cube_vertices_template[5*3]); /* 5 */
	cube_fill (0x054, vertices, elements, &vindex, &tindex, &cube_vertices_template[2*3], &cube_vertices_template[1*3], &cube_vertices_template[5*3], &cube_vertices_template[6*3]); /* 3 */
	cube_fill (0x145, vertices, elements, &vindex, &tindex, &cube_vertices_template[0*3], &cube_vertices_template[3*3], &cube_vertices_template[7*3], &cube_vertices_template[4*3]); /* 4 */
	//elements_count = tindex;

	generate_normals (vindex, tindex);

	printf ("/* generated by %s */\n", argv[0]);
	printf ("\nconst struct wurfel_dice::vertix_t wurfel_dice::vertices[%d] =\n", vindex);
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
	printf ("\nconst struct wurfel_dice::element_t wurfel_dice::elements[%d] =\n", tindex);
	printf ("{\n");
	for (int i=0; i < tindex; i++)
	{
		printf ("\t/* %4d */ {%4d, %4d, %4d},\n", i,
			elements[i].vertices[0],
			elements[i].vertices[1],
			elements[i].vertices[2]);
	}
	printf ("};\n");

	return 0;
}
