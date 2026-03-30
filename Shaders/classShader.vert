#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTex;
layout(location = 3) in vec3 m_Tan;
layout(location = 4) in vec3 b_Tan;
out vec3 fragPos;
out vec3 normCoord;
out vec2 texCoord; //pass this to frag shader
out mat3 TBN;
uniform mat4 projection;
uniform mat4 transform;
uniform mat4 view;
void main() {
	fragPos = vec3(transform * vec4(aPos, 1.0));
	gl_Position = projection * view * transform * vec4(aPos, 1.0);
	texCoord = aTex;

	mat3 modelMat = mat3(transpose(inverse(transform)));
	normCoord = modelMat * aNormal;

	vec3 T = normalize(modelMat * m_Tan);
	
	vec3 N = normalize(normCoord);

	T = normalize(T - dot(T, N) * N);
	vec3 B = cross(N, T);

	TBN = mat3(T,B,N);

}