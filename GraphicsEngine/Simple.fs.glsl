#version 450 core
#define MAX_LIGHTS 30
out vec4 out_Color;
// Input from vertex shader
in GS_OUT {
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
fs_in;
uniform int NUM_TEXTURES;
uniform sampler2D Texture0;
uniform sampler2D Texture1;
uniform sampler2D Texture2;
uniform sampler2D Texture3;

//uniform sampler2DMS msTexture0;// Multi-sample texture

uniform float Alpha;
uniform vec4 ModelColor;
uniform bool Toggle_NormalAndSpecularMaps;

uniform bool Toggle_Lights;
uniform bool Toggle_Textures;

uniform int NUM_LIGHTS;

uniform struct Light {
  bool IsEnabled;
  int TypeFlag;
  vec3 Position;
  vec3 Direction;
  float ConeAngle;
  vec3 Diffuse;
  vec3 Specular;
  vec3 Ambient;
  float SpecularPower;
  float AttenuationConst;
  float AttenuationLinear;
  float AttenuationQuad;
} lights[MAX_LIGHTS];

vec3 processPointLight(in vec3 norm, in vec3 pos, in int lightIndex);
vec3 processDirectionalLight(in vec3 norm, in vec3 pos, in int lightIndex);
vec3 processSpotLight(in vec3 norm, in vec3 pos, in int lightIndex);
float mip_map_level(in vec2 texture_coordinate);
void main(void) {
  vec3 norm = normalize(fs_in.normal.xyz);
  vec3 pos = fs_in.worldPosition.xyz;
  vec3 surfaceToCamera = fs_in.eyePosition.xyz - fs_in.worldPosition.xyz;
  vec3 tempColor = vec3(0.0f); // ModelColor.rgb;
  if (Toggle_Lights) {
    for (int lightIndex = 0; lightIndex < NUM_LIGHTS; lightIndex++)
      if (lights[lightIndex].IsEnabled)
        switch (lights[lightIndex].TypeFlag) {
        case 0: {
          tempColor += processPointLight(norm, pos, lightIndex);
          break;
        }
        case 1: {
          tempColor += processDirectionalLight(norm, pos, lightIndex);
          break;
        }
        case 2: {
          tempColor += processSpotLight(norm, pos, lightIndex);
          break;
        }
        }
  } else if (Toggle_Textures) {
    // (texCoordX  * spriteWidth / textureWidth) + texSourceX
    // float mipmapLevel = mip_map_level(fs_in.textureCoord.xy); //TODO: use
    // mipmap
    vec4 textureResult = textureLod(
        Texture0, ((fs_in.textureCoord.xy * (vec2(512) / vec2(7680))) +
                   (vec2(512) * fs_in.textureUnits.xy) / vec2(7680)),
        0);

    tempColor = textureResult.rgb;
  } else
    tempColor = ModelColor.rgb;

  //	tempColor *= clamp(tempColor, 0.0f, 1.0f);
  out_Color = vec4(tempColor, Alpha);
  return;
}

vec3 processPointLight(in vec3 norm, in vec3 pos, in int lightIndex) {
  float distance = length(lights[lightIndex].Position.xyz - pos.xyz);

  // Light Attenuation Computation
  float attenuation =
      1.0f / ((lights[lightIndex].AttenuationConst +
               lights[lightIndex].AttenuationLinear * distance +
               lights[lightIndex].AttenuationQuad * distance * distance));

  vec3 textureDifResult;
  vec3 textureSpecResult;
  if (Toggle_Textures) {
    vec3 textureNrmResult;
	if(Toggle_NormalAndSpecularMaps == true){
    textureDifResult =
        textureLod(Texture0,
                   ((fs_in.textureCoord.xy * (vec2(512) / vec2(7680))) +
                    (vec2(512) * (fs_in.textureUnits.xy)) / vec2(7680)),
                   0)
            .rgb;


    textureNrmResult =
        textureLod(Texture0,
 (fs_in.textureCoord.xy * (vec2(512) / vec2(7680))) +
                    (vec2(512) * (vec2(fs_in.textureUnits.x + 1, fs_in.textureUnits.y)) / vec2(7680) ),
                   0)
            .rgb;
    textureSpecResult = textureLod(Texture0,
 (fs_in.textureCoord.xy * (vec2(512) / vec2(7680))) +
                    (vec2(512) * (vec2(fs_in.textureUnits.x + 2, fs_in.textureUnits.y)) / vec2(7680) ),
                   0)
            .rgb;
    // Transform normal vector to range [-1,1]
    norm = (textureNrmResult * 2.0f - vec3(1.0f));
		}else{
	textureDifResult =  texture(Texture0,
                   fs_in.textureCoord.xy ).rgb;

				     if (texture(Texture0,
                   fs_in.textureCoord.xy ).a == 0.0) {
        discard;
	
    }

	}
  } else {
    textureDifResult = ModelColor.rgb;
    textureSpecResult = ModelColor.rgb;
  }
  // viewPoint, normal, tangent and bitangent are in view space so we need to
  // transform the light's position to view space too..
  vec3 L = normalize(mat3(fs_in.mv_Matrix) * lights[lightIndex].Position.xyz) -
           fs_in.viewPoint;
  vec3 lightDir = normalize(vec3(dot(L, fs_in.tangent.xyz),
                                 dot(L, fs_in.bitangent.xyz), dot(L, norm)));
  vec3 reflection = reflect(-L, norm);
  // Light Diffuse Computation
  vec3 diffuse = (max(0.0f, dot(norm, lightDir)) * lights[lightIndex].Diffuse *
                  textureDifResult);
  // Light Ambiance Computation
  vec3 ambient = lights[lightIndex].Ambient;
  // Light Specular Computation
  vec3 specular = vec3(0.0f, 0.0f, 0.0f);
  // if (dot(lightDir , norm ) > 0.0f)
  specular =
      pow(max(0.0f, dot(reflect(-lightDir, norm), fs_in.eyeDir.xyz)), 1.0) *
      textureSpecResult * lights[lightIndex].Specular *
      lights[lightIndex].SpecularPower;
  return min((ambient) + attenuation * (diffuse + specular), vec3(1.0));
}

vec3 processDirectionalLight(in vec3 norm, in vec3 pos, in int lightIndex) {

  vec3 textureDifResult;
  vec3 textureSpecResult;
  if (Toggle_Textures) {
    vec3 textureNrmResult;
				if(Toggle_NormalAndSpecularMaps == true){
    textureDifResult =
        textureLod(Texture0,
                   ((fs_in.textureCoord.xy * (vec2(512) / vec2(7680))) +
                    (vec2(512) * (fs_in.textureUnits.xy)) / vec2(7680)),
                   0)
            .rgb;

    textureNrmResult =
        textureLod(Texture0,
 (fs_in.textureCoord.xy * (vec2(512) / vec2(7680))) +
                    (vec2(512) * (vec2(fs_in.textureUnits.x + 1, fs_in.textureUnits.y)) / vec2(7680) ),
                   0)
            .rgb;
    textureSpecResult =
        textureLod(Texture0,
 (fs_in.textureCoord.xy * (vec2(512) / vec2(7680))) +
                    (vec2(512) * (vec2(fs_in.textureUnits.x + 2, fs_in.textureUnits.y)) / vec2(7680) ),
                   0)
            .rgb;
    // Transform normal vector to range [-1,1]
    norm = (textureNrmResult * 2.0f - vec3(1.0f));
		}else{
	textureDifResult =  texture(Texture0,
                   fs_in.textureCoord.xy ).rgb;
	}
  } else {
    textureDifResult = ModelColor.rgb;
    textureSpecResult = ModelColor.rgb;
  }

  vec3 lightDirection =
      normalize(mat3(fs_in.mv_Matrix) * lights[lightIndex].Position -
                mat3(fs_in.mv_Matrix) * vec3(pos));
  float lightDistance = length(lightDirection);
  lightDirection = lightDirection / lightDistance;
  if (dot(lightDirection,
          mat3(fs_in.mv_Matrix) * lights[lightIndex].Direction) > 0) {

    float diffuse = pow(max(1.0f, dot(reflect(lightDirection,
                                              mat3(fs_in.mv_Matrix) *
                                                  lights[lightIndex].Direction),
                                      fs_in.eyeDir.xyz)),
                        1.0);
    float specular = max(0.0, dot(norm, lightDirection));

    if (diffuse <= 0.1)
      specular = 0.0;
    else
      specular = pow(specular, 0.3); // sharpen the highlight

    vec3 scatteredLight =
        lights[lightIndex].Ambient +
        lights[lightIndex].Diffuse * diffuse * textureDifResult;
    vec3 reflectedLight = (specular * lights[lightIndex].Specular *
                           lights[lightIndex].SpecularPower) *
                          textureSpecResult;
    return min((scatteredLight + reflectedLight), vec3(1.0));
  }
  return vec3(0.0f);
}

vec3 processSpotLight(in vec3 norm, in vec3 pos, in int lightIndex) {

  vec3 textureDifResult;
  vec3 textureSpecResult;

  if (Toggle_Textures) {
    vec3 textureNrmResult;
				if(Toggle_NormalAndSpecularMaps == true){
    textureDifResult =
        textureLod(Texture0,
                   ((fs_in.textureCoord.xy * (vec2(512) / vec2(7680))) +
                    (vec2(512) * (fs_in.textureUnits.xy)) / vec2(7680)),
                   0)
            .rgb;

    textureNrmResult =
        textureLod(Texture0,
                   (fs_in.textureCoord.xy * (vec2(512) / vec2(7680))) +
                    (vec2(512) * (vec2(fs_in.textureUnits.x + 1, fs_in.textureUnits.y)) / vec2(7680) ),
                   0)
            .rgb;
    textureSpecResult =
        textureLod(Texture0,
                   (fs_in.textureCoord.xy * (vec2(512) / vec2(7680))) +
                    (vec2(512) * (vec2(fs_in.textureUnits.x + 2, fs_in.textureUnits.y)) / vec2(7680) ),
                   0)
            .rgb;
    // Transform normal vector to range [-1,1]
    norm = (textureNrmResult * 2.0f - vec3(1.0f));
	}else{
	textureDifResult =  texture(Texture0,
                   fs_in.textureCoord.xy ).rgb;
	}
  } else {
    textureDifResult = ModelColor.rgb;
    textureSpecResult = ModelColor.rgb;
  }

  vec3 lightDirection = mat3(fs_in.mv_Matrix) * lights[lightIndex].Position -
                        mat3(fs_in.mv_Matrix) * vec3(pos);
  float lightDistance = length(lightDirection);
  lightDirection = lightDirection / lightDistance;
  // Light Attenuation Computation
  float attenuation =
      1.0f /
      ((lights[lightIndex].AttenuationConst +
        lights[lightIndex].AttenuationLinear * lightDistance +
        lights[lightIndex].AttenuationQuad * lightDistance * lightDistance));

  float spotCos =
      dot(lightDirection,
          normalize(mat3(fs_in.mv_Matrix) *
                    lights[lightIndex].Direction)); // attenuate more, based on
                                                    // spot-relative position
  if (spotCos < lights[lightIndex].ConeAngle)
    attenuation = 0.0;
  else
    attenuation *= pow(spotCos, 1);
  vec3 halfVector = normalize(lightDirection.xyz);
  float diffuse = max(0.0, dot(norm, lightDirection));
  float specular = max(0.0, dot(norm, halfVector));
  if (diffuse == 0.0)
    specular = 0.0;
  else
    specular = pow(specular, 0.3);

  vec3 scatteredLight =
      lights[lightIndex].Ambient +
      (lights[lightIndex].Diffuse * textureDifResult) * diffuse;
  vec3 reflectedLight = lights[lightIndex].Specular * textureSpecResult *
                        specular * lights[lightIndex].SpecularPower;
  return min(attenuation * (scatteredLight + reflectedLight), vec3(1.0));
}