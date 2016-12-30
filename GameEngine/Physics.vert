#version 410 core

layout(location = 0) in vec4 in_Position;
layout(location = 1) in vec4 in_TexCoord;
layout(location = 2) in vec4 in_Normal;
layout(location = 3) in vec4 in_Tangent;
layout(location = 4) in uvec4 in_TextureUnits;

// per frame
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
uniform vec4 EyePosition;
// per item
uniform mat4 ModelMatrix;
uniform mat4 ModelMatrixOrientation;

out VS_OUT {
  vec4 eyePosition;
  vec4 worldPosition;
  vec4 normal;
  vec4 tangent;
  vec4 bitangent;
  vec3 textureCoord;
}
vs_out;

void main() {
  gl_Position = ModelMatrix * in_Position;
  vs_out.worldPosition = ModelMatrix * in_Position;
  vs_out.eyePosition =
      ProjectionMatrix * ViewMatrix * ModelMatrix * in_Position;
  vs_out.normal = normalize(in_Normal);
  vs_out.textureCoord = vec3(in_TexCoord.x, in_TexCoord.y, in_TexCoord.z);
  gl_Position = ProjectionMatrix * ViewMatrix * gl_Position;
  return;
}