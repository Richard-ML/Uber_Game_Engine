#version 410 core
layout(lines) in;

layout(line_strip, max_vertices = 32) out;
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
  vec3 textureCoord;
}
gs_out;

in VS_OUT {
  vec4 eyePosition;
  vec4 worldPosition;
  vec4 normal;
  vec4 tangent;
  vec4 bitangent;
  vec3 textureCoord;
}
gs_in[];

// per frame
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
uniform vec4 EyePosition;
// per item
uniform mat4 ModelMatrix;
uniform mat4 ModelMatrixOrientation;

// const float MAGNITUDE = 10.0f;
//
// void GenerateLine(int index)
//{ // Source : http://learnopengl.com/#!Advanced-OpenGL/Geometry-Shader
//    //gl_Position = gl_in[index].gl_Position;
//    //EmitVertex();
//	gl_Position =  ProjectionMatrix * ViewMatrix *
//vec4(gs_in[index].worldPosition);
//    EmitVertex();
//    gl_Position = vec4(gs_in[index].worldPosition) + vec4(gs_in[index].normal)
//    * MAGNITUDE;
//    EmitVertex();
//
//    EndPrimitive();
//}

void main() {

  for (int i = 0; i < gl_in.length(); i++) {
    gl_Position = gl_in[i].gl_Position;
    gl_PointSize = gl_in[i].gl_PointSize;

    gs_out.eyePosition = gs_in[i].eyePosition;
    gs_out.worldPosition = gs_in[i].worldPosition;
    gs_out.normal = gs_in[i].normal;
    gs_out.textureCoord = gs_in[i].textureCoord;

    EmitVertex();
  }
  EndPrimitive();
  // GenerateLine(0); // First vertex normal
  // GenerateLine(1); // Second vertex normal
  // GenerateLine(2); // Third vertex normal
}
