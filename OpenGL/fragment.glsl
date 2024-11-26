#version 330 core

in vec3 FragPos;
in vec3 FragNormal;
in vec3 FragColor; 

out vec4 FinalColor; 

uniform vec3 lightColor;  // �ֺ� ����
uniform vec3 lightPos;  // ���� ��ġ
uniform vec3 viewPos;  // ī�޶� ��ġ

void main(void) {
	float ambientLight = 0.5f;  // �ֺ� ���� ���
	vec3 ambient = ambientLight * lightColor;

	vec3 normalVector = normalize(FragNormal);
	vec3 lightDir = normalize(lightPos - FragPos);
	float diffuseLight = max(dot(normalVector, lightDir), 0.0f);
	vec3 diffuse = diffuseLight * lightColor;

	int shininess = 64;
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, normalVector);
	float specularLight = max(dot(viewDir, reflectDir), 0.0f);
	specularLight = pow(specularLight, shininess);
	vec3 specular = specularLight * lightColor;

	vec3 result = (ambient + diffuse + specular) * FragColor;

	FinalColor = vec4(result, 1.0f);	
}
