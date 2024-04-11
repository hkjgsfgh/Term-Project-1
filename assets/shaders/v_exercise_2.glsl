#version 460 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
out vec3 color;
uniform float time;

void main()
{
	float delta = sin(time) * 0.5;
	gl_Position = vec4(aPos.x + delta, aPos.y, aPos.z, 1.0);
	color = aColor;
}