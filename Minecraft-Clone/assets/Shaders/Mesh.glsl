

#type Vertex
#version 450 core

layout(location = 0) in vec3 v_Position;
layout(location = 1) in vec3 v_Normal;
layout(location = 2) in vec2 v_TexCoord;

layout(location = 0) out vec3 o_Position;
layout(location = 1) out vec3 o_Normal;
layout(location = 2) out vec2 o_TexCoord;

uniform mat4 u_ViewProjection;
void main() {
	o_Normal    = v_Normal;
    o_Position  = v_Position;
	o_TexCoord  = v_TexCoord;
	gl_Position = u_ViewProjection * vec4(v_Position, 1.0f);
}







#type Fragment
#version 450 core

layout(location = 0) in vec3 v_Position;
layout(location = 1) in vec3 v_Normal;
layout(location = 2) in vec2 v_TexCoord;

layout(location = 0) out vec4 o_Color;

uniform sampler2D u_Texture;

vec4 DirectionalLight(vec4 fragColor) {
	vec3 lightColor     = vec3(1.0f, 1.0f, 1.0f);
	vec3 lightDirection = normalize(-vec3(-0.2f, -1.0f, -0.3f));

	float ambientStrength  = 0.3f;
	float diffuseStrength  = 0.8f * max(dot(v_Normal, lightDirection), 0.0f);
//	float specularStrength = 0.5;

//	vec3 viewDir    = normalize(u_CameraPosition - v_Position);
//	vec3 reflectDir = reflect(-lightDirection, v_Normal);
//	float spec      = pow(max(dot(viewDir, reflectDir), 0.0), 32);

	vec3 ambient  = ambientStrength  * lightColor;
	vec3 diffuse  = diffuseStrength  * lightColor;
//    vec3 specular = specularStrength * spec * lightColor;

	return fragColor * vec4(ambient.xyz + diffuse.xyz, 1.0f);
}


void main() {
	o_Color = texture(u_Texture, v_TexCoord);
	o_Color = DirectionalLight(o_Color);
}




