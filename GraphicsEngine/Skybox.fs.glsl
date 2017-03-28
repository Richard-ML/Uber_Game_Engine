#version 410 core

uniform samplerCube skybox;
uniform bool Toggle_Skybox_Textures;
in VS_OUT { vec3 tc; }
fs_in;

layout(location = 0) out vec4 color;

void main(void) {
if (Toggle_Skybox_Textures)
color = texture(skybox, fs_in.tc);
else
color = vec4(1.0f);
}
