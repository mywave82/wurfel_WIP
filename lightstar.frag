#version 150 core
in vec3 Position;
in vec3 _WorldPosition;
out vec4 outColor;

uniform float Clock; /* between 0 and 59.999999, counts seconds within a minute */

#define M_PI 3.1415926535897932384626433832795

float flare(float Angle)
{
	Angle = Angle*2.17 + 4.0 + sin(Angle*2.6) + sin(Angle*7.1 + 12.34);

	return pow(abs(sin(Angle)), 6.0);
}


void main()
{
	// Pytagoras theorem to calculate length from origo
	float L = sqrt (Position.x * Position.x + Position.y * Position.y);
	float angle = atan (Position.x, Position.y);


	vec4 ColorTemp = vec4(0.0, 0.0, 0.0, 0.0);

	float angleA = angle + 4 * M_PI + mod(Clock * 2 * M_PI / 3, 2 * M_PI);
	      angleA = mod(angleA, 2 * M_PI);

	float angleB = angle + 4.0 * M_PI - mod(Clock * 2 * M_PI / 5, 2 * M_PI);
	      angleB = mod(angleB, 2 * M_PI);

	float angleC = angle + 4.0 * M_PI + mod(Clock * 2 * M_PI / 10, 2 * M_PI);
	      angleC = mod(angleC, 2 * M_PI);


	vec4 ColorD = vec4(
		1.0,
		1.0,
		clamp(1.0 - L*1.3, 0.0, 1.0),
		clamp(1.0 - L*3.0, 0.0, 1.0)
	);

	vec4 ColorE = vec4(
		1.0,
		1.0,
		0.0,
		flare (angleA) *
			clamp( 
				(1.0-clamp(L, 0.0, 1.0))*2.0,
				0.0, 1.0
			)
	);

	vec4 ColorF = vec4(
		1.0,
		1.0,
		0.5,
		flare (angleB) *
			clamp( 
				(0.9-clamp(L, 0.0, 1.0))*2.2,
				0.0, 1.0
			)
	);

	vec4 ColorG = vec4(
		1.0,
		0.8,
		0.8,
		flare (angleC) *
			clamp( 
				(0.8-clamp(L, 0.0, 1.0))*2.3,
				0.0, 1.0
			)
	);

	
	ColorTemp = vec4(
		0.0,
		0.0,
		0.0,
		0.0);

	ColorTemp = vec4(
		ColorTemp.r + ColorE.r * ColorE.a,
		ColorTemp.g + ColorE.g * ColorE.a,
		ColorTemp.b + ColorE.b * ColorE.a,
		clamp(ColorTemp.a + ColorE.a, 0.0, 1.0));

	ColorTemp = vec4(
		ColorTemp.r + ColorF.r * ColorF.a,
		ColorTemp.g + ColorF.g * ColorF.a,
		ColorTemp.b + ColorF.b * ColorF.a,
		clamp(ColorTemp.a + ColorF.a, 0.0, 1.0));

	ColorTemp = vec4(
		ColorTemp.r + ColorG.r * ColorG.a,
		ColorTemp.g + ColorG.g * ColorG.a,
		ColorTemp.b + ColorG.b * ColorG.a,
		clamp(ColorTemp.a + ColorG.a, 0.0, 1.0));

	ColorTemp = vec4(
		ColorTemp.r + ColorD.r * ColorD.a,
		ColorTemp.g + ColorD.g * ColorD.a,
		ColorTemp.b + ColorD.b * ColorD.a,
		clamp(ColorTemp.a + ColorD.a, 0.0, 1.0));

	outColor = ColorTemp;
}
