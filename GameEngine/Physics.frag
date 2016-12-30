#version 410 core
out vec4 out_Color;
// Input from geom shader
in GS_OUT {
  vec4 eyePosition;
  vec4 worldPosition;
  vec4 normal;
  vec4 tangent;
  vec4 bitangent;
  vec3 textureCoord;
}
fs_in;

uniform float Alpha;
uniform vec4 ModelColor;

void main(void) {
  out_Color = vec4(ModelColor.rgb, Alpha);
  return;
}
