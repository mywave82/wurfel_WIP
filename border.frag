#version 150 core
in vec3 Position;
out vec4 outColor;

vec3 GetCenter(vec3 input)
{
	input.x = floor(input.x);
	input.y = floor(input.y);
	input.z = floor(input.z);

	vec3 retval = vec3
	(
		input.x + sin(input.x + cos(input.y*4.3 * input.x) + sin(input.z*1.3 * input.y))*0.20,
		input.y + sin(input.y + cos(input.z*5.2 * input.y) + sin(input.x*2.3 * input.z))*0.20,
		input.z + sin(input.z + cos(input.x*7.5 * input.z) + sin(input.y*0.4 * input.x))*0.20
	);
	return retval;
}

void main()
{
	float fD = 100.0;            /* the nearest distance */
	vec3  fP;
	vec3  fPB;
	int fI;

	int fx, fy, fz;

	float sD = 100.0;            /* the second nearest distance */
	vec3  sP;

	/* find the nearest point */
	for (int x = -1; x <= 1; x++)
	for (int y = -1; y <= 1; y++)
	for (int z = -1; z <= 1; z++)
	{
		vec3 cP = GetCenter(Position + vec3(x, y, z));

#if 1
		vec3 temp = cP - Position;
		float cD = dot (temp, temp);
#else
		float cD = abs(distance (cP, Position));
#endif

		if (cD < fD)
		{
			fD = cD;
			fP = cP;
			fPB = floor (Position + vec3(x, y, z));
			fI = x+y*5+z*25;
		}
	}


	/* find the second nearest point */
	for (int x = -2; x <= 2; x++)
	for (int y = -2; y <= 2; y++)
	for (int z = -2; z <= 2; z++)
	/* avoid testing the closest point */
	if ((x+y*5+z*25 != fI))
	{
		vec3 cP = GetCenter(Position + vec3(x, y, z));
#if 1
		vec3 temp = cP - Position;
		float cD = dot (temp, temp);
#else
		float cD = abs(distance (cP, fP));
#endif
		{
			if (cD < sD)
			{
				sD = cD;
				sP = cP;
			}
		}
	}

	vec3 sNormalized = normalize(sP - fP); /* normalized vector pointing from nearest to second nearest point */
	float bD = abs(dot (0.5 * (fP + sP) - Position, sNormalized)); /*    *0.5   is the same as   /2.0    and we want the average position, and the distance from it */

	if (bD < 0.06)
	{
	//	/* the grey glue */
		outColor = vec4(0.3, 0.3, 0.3, 1.0); /* need some random noise */
	} else {
		/* the stones inbetween */
		outColor = vec4(fD, sD, bD, 1.0); /* need some random cobble noise */
	}
}
