#version 420 core
layout(triangles) in;

layout(triangle_strip, max_vertices = 3) out;
// layout(triangle_strip, max_vertices = 3) out;
out gl_PerVertex {
  vec4 gl_Position;
  float gl_PointSize;
};
in gl_PerVertex {
  vec4 gl_Position;
  float gl_PointSize;
}
gl_in[];
out GS_OUT {
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
gs_out;

in VS_OUT {
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
gs_in[];

// per frame
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
uniform vec4 EyePosition;
// per item
uniform mat4 ModelMatrix;
uniform mat4 ModelMatrixOrientation;

void main() {

  for (int i = 0; i < gl_in.length(); i++) {
    gl_Position = gl_in[i].gl_Position;
    gl_PointSize = gl_in[i].gl_PointSize;

    gs_out.eyePosition = gs_in[i].eyePosition;
    gs_out.worldPosition = gs_in[i].worldPosition;
    gs_out.normal = gs_in[i].normal;
    gs_out.tangent = gs_in[i].tangent;
    gs_out.bitangent = gs_in[i].bitangent;
    gs_out.textureCoord = gs_in[i].textureCoord;
    gs_out.textureUnits = gs_in[i].textureUnits;
    gs_out.viewPoint = gs_in[i].viewPoint;
    gs_out.mv_Matrix = gs_in[i].mv_Matrix;
    EmitVertex();
  }
  EndPrimitive();
}
