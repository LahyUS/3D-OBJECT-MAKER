#version 330 core

out vec4 FragColor;

in vec3 Normal;  
in vec3 FragPos;  
in vec2 TexCoords;

struct Material{
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};
  
struct Light{
	vec3 position;
	vec3 direction;
	vec3 color;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

uniform vec3 viewPos;
uniform Material material;
uniform Light light;

void main()
{
	
	vec3 lightDir = normalize(light.position - FragPos);

	// ambient
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords)).rgb * light.color;
	
	// diffuse 
	vec3 norm = normalize(Normal);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords)).rgb * light.color;

	//specular
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
	vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords)).rgb * light.color;
			
	// attenuation;
	float distance = length(light.position - FragPos);
	float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic *(distance*distance));

	//result
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;
	vec3 result = (ambient + diffuse + specular);
	FragColor = vec4(result, 0.8);
} 