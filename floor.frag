#version 150 core
in vec3 Position;

out vec4 outColor;
uniform vec3 Color1;
uniform vec3 Color2;
uniform vec3 AvgColor;
uniform float Frequency;

uniform int Mode;

void main()
{
	if (Mode == 0)
	{
		outColor = vec4(0.0, 0.0, 0.0, 1.0);
		return;
	}

	vec3 color;
	vec2 fw = fwidth (Position.xy);
	vec2 fuzz = fw * Frequency * 2.0f;
	float fuzzMax = max(fuzz.s, fuzz.t);
	vec2 checkPos = fract(Position.xy * Frequency);
	//if (fuzzMax < 0.5)
	{
		vec2 p =          smoothstep(vec2(0.5), fuzz + vec2(0.5), checkPos) +
		           (1.0 - smoothstep(vec2(0.0), fuzz,             checkPos));

		color = mix(Color1, Color2, p.x * p.y + (1.0 - p.x) * (1.0 - p.y));

		color = mix(color, AvgColor, smoothstep (0.125, 0.5, fuzzMax));

		outColor = vec4 (color, 0.2);
	//} else {
	//	outColor = vec4(AvgColor, 1.0);
	}
}
