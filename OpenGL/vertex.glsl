#version 330 core

layout (location = 0) in vec3 VertexPos; 
layout (location = 1) in vec3 VertexNormal; 
layout (location = 2) in vec3 VertexColor; 

out vec3 FragPos;
out vec3 FragNormal;
out vec3 FragColor;

uniform mat4 modelTransform;
uniform mat4 normalTransform;
uniform mat4 viewTransform;
uniform mat4 projTransform;

void main(void) {
	gl_Position = projTransform * viewTransform * modelTransform * vec4(VertexPos, 1.0f);

	FragPos = vec3(modelTransform * vec4(VertexPos, 1.0f));
	FragNormal = mat3(normalTransform) * VertexNormal;
	FragColor = VertexColor;
}
