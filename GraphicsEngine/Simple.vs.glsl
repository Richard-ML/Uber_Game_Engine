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
  vec4 eyeDir;
  vec3 textureCoord;
  flat uvec4 textureUnits;
  vec3 viewPoint;
  mat4 mv_Matrix;
}
vs_out;

void main() {

  gl_Position = ModelMatrix * in_Position;

  vs_out.worldPosition = ModelMatrix * in_Position;
  vs_out.eyePosition =
      ProjectionMatrix * ViewMatrix * ModelMatrix * in_Position;

  vs_out.textureCoord = vec3(in_TexCoord.x, in_TexCoord.y, in_TexCoord.z);
  vs_out.textureUnits = in_TextureUnits;

  gl_Position = ProjectionMatrix * ViewMatrix * gl_Position;

  vs_out.mv_Matrix = ViewMatrix * ModelMatrix;
  // vertex Position in view space..
  vec4 P = vs_out.mv_Matrix * in_Position;
  vs_out.viewPoint = P.xyz;
  vs_out.normal = normalize(vec4(mat3(vs_out.mv_Matrix) * in_Normal.xyz, 1.0f));
  // Calculate normal (N) and tangent (T) vectors in view space from incoming
  // object space vectors.
  vs_out.tangent =
      normalize(vec4(mat3(vs_out.mv_Matrix) * in_Tangent.xyz, 1.0f));

  // Calculate the bitangent vector (B) from the normal and tangent vectors
  vs_out.bitangent = vec4(cross(vs_out.normal.xyz, vs_out.tangent.xyz), 1.0f);

  // The view vector is the vector from the point of interest to the viewer,
  // which in view space is simply the negative of the position.
  // Calculate that and multiply it by the TBN matrix.
  vec3 V = -P.xyz;
  vs_out.eyeDir =
      normalize(vec4(dot(V, vs_out.tangent.xyz), dot(V, vs_out.bitangent.xyz),
                     dot(V, vs_out.normal.xyz), 1.0f));

  return;
}

// Saving this for later ;)
// glVertexAttribPointer(1, 1, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(sMeshVertex),
// (GLvoid*)offsetToTextureInfoInBytes);
// layout(location = 1) in uint vertexColor;
// byte components as RGBA values:
// vec4 color = vec4((vertexColor & 0x000000FF) >> 0, (vertexColor & 0x0000FF00)
// >> 8, (vertexColor & 0x00FF0000) >> 16, (vertexColor & 0xFF000000)>> 24);
