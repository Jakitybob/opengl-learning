#version 330 core
layout (location = 0) in vec3 aPos;    // attribute position 0
layout (location = 1) in vec3 aColor;  // attribute postion 1

out vec3 vertexColor;

void main()
{
	gl_Position = vec4(aPos.x, aPos.y * -1, aPos.z, 1.0);
	vertexColor = aColor;
}