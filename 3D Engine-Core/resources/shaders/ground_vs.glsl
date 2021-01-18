#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec3 FragPos;
out vec2 TexCoords;

//uniform mat4 MVP;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    vec4 pos = projection * view * model * vec4(aPos, 1.0);
    gl_Position = pos.xyww;
	//gl_Position = MVP * vec4(aPos, 1.0);
	FragPos = vec3(model * vec4(aPos, 1.0));
	TexCoords = aTexCoords;
}

