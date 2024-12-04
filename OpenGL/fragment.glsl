#version 330 core

in vec3 FragPos;
in vec3 FragNormal;
in vec2 FragTexCoord;
in vec4 FragColor; 

out vec4 FinalColor; 

uniform vec3 lightColor;  // 주변 조명
uniform vec3 lightPos;  // 조명 위치
uniform vec3 viewPos;  // 카메라 위치
uniform bool useTexture;
uniform sampler2D outTexture; 

void main(void) {
	float ambientLight = 0.5f;  // 주변 조명 계수
	vec3 ambient = ambientLight * lightColor;

	vec3 normalVector = normalize(FragNormal);
	vec3 lightDir = normalize(lightPos - FragPos);
	float diffuseLight = max(dot(normalVector, lightDir), 0.0f);
	vec3 diffuse = diffuseLight * lightColor;

	int shininess = 128;
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, normalVector);
	float specularLight = max(dot(viewDir, reflectDir), 0.0f);
	specularLight = pow(specularLight, shininess);
	vec3 specular = specularLight * lightColor;

	FinalColor = useTexture ? texture(outTexture, FragTexCoord) * vec4((ambient + diffuse + specular), 1.0f) * FragColor : vec4((ambient + diffuse + specular), 1.0f) * FragColor;
}
