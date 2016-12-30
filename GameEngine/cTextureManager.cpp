#include "cTextureManager.h"
#include "global.h"
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/**
File:
Author(s):
        Name: Richard Mills-Laursen
        Role(s): Student
        E-mail: R_MillsLaursen@FanshaweOnline.ca | Revolut1on1ze@Hotmail.ca

    Name: Michael Feeney
    E-mail: MFeeney@FanshaweC.on.ca
        Role(s): Coordinator & Professor
        Course(s): INFO-6044-01 - Game Engine Framework/Patterns
                           INFO-6028-01 - Graphics 1
                           INFO-6019-01 - Physics & Simulation 1


        Name: Ben McIlmoyle
        E-Mail: B_McIlmoyle@FanshaweOnline.ca
        Role: Professor
        Course: INFO-6016-01 - Network Programming

        Name: James Lucas
        E-Mail: J_Lucas@FanshaweOnline.ca
        Role: Professor
        Course: INFO-6017-01 - Artificial Intelligence

        Name: Oscar Lara
        E-Mail: O_Lara@FanshaweOnline.ca
        Role: Professor
        Course: INFO-6046-01 - Media Fundamentals

Status: Version 1.7 Alpha
(c) Copyright(s): Fanshawe College

Description:

*/

cTextureManager *cTextureManager::instance() {
  if (!s_cTextureManager)
    s_cTextureManager = new cTextureManager();
  return s_cTextureManager;
}
//    _                    _  _____            _
//   | |  ___    __ _   __| ||_   _|___ __  __| |_  _   _  _ __  ___
//   | | / _ \  / _` | / _` |  | | / _ \\ \/ /| __|| | | || '__|/ _ \
//   | || (_) || (_| || (_| |  | ||  __/ >  < | |_ | |_| || |  |  __/
//   |_| \___/  \__,_| \__,_|  |_| \___|/_/\_\ \__| \__,_||_|   \___|
//
void cTextureManager::loadTexture(rapidxml::xml_node<> *textureNode) {

  std::string path = textureNode->first_attribute("path")->value();
  FREE_IMAGE_FORMAT imgFormat =
      FreeImage_GetFileType(path.c_str(), 0); // Get current format
  FIBITMAP *imagen = FreeImage_Load(imgFormat, path.c_str());
  imagen = FreeImage_ConvertTo32Bits(imagen);
  int width = FreeImage_GetWidth(imagen);
  int height = FreeImage_GetHeight(imagen);

  GLubyte *textura = new GLubyte[4 * width * height];
  char *pixeles = (char *)FreeImage_GetBits(imagen);
  // FreeImage loads in BGR format, so you need to swap some bytes(Or use
  // GL_BGR).

  for (int j = 0; j < width * height; j++) {
    textura[j * 4 + 0] = pixeles[j * 4 + 2];
    textura[j * 4 + 1] = pixeles[j * 4 + 1];
    textura[j * 4 + 2] = pixeles[j * 4 + 0];
    textura[j * 4 + 3] = pixeles[j * 4 + 3];
    //	std::cout<<j<<":
    //"<<textura[j*4+0]<<"**"<<textura[j*4+1]<<"**"<<textura[j*4+2]<<"**"<<textura[j*4+3]<<std::endl;
  }
  // Now generate the OpenGL texture object
  // pointer to the image data
  // BYTE* bits = FreeImage_GetBits(temp);
  gUniform_TextureID.push_back(0);

  glGenTextures(1, &gUniform_TextureID.back());
  g_pMapTextureNameToID[textureNode->first_attribute("name")->value()] =
      gNextTextureID;
  glActiveTexture(GL_TEXTURE0 + gNextTextureID);
  gNextTextureID++;
  glBindTexture(GL_TEXTURE_2D, gUniform_TextureID.back());
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, (GLvoid *)textura);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void cTextureManager::loadTextureMipmap(rapidxml::xml_node<> *textureNode) {
  std::string pathDif = textureNode->first_attribute("pathDif")->value();
  std::string pathNrm = textureNode->first_attribute("pathNrm")->value();
  std::string pathSpec = textureNode->first_attribute("pathSpec")->value();
  FREE_IMAGE_FORMAT imgFormat = FreeImage_GetFileType(
      pathDif.c_str(),
      0); // Get current format (assume all textures are the same format)

  FIBITMAP *imagenDif = FreeImage_Load(imgFormat, pathDif.c_str());
  imagenDif = FreeImage_ConvertTo32Bits(imagenDif);

  FIBITMAP *imagenNrm = FreeImage_Load(imgFormat, pathNrm.c_str());
  imagenNrm = FreeImage_ConvertTo32Bits(imagenNrm);

  FIBITMAP *imagenSpec = FreeImage_Load(imgFormat, pathSpec.c_str());
  imagenSpec = FreeImage_ConvertTo32Bits(imagenSpec);

  int width =
      FreeImage_GetWidth(imagenDif); // Assume images are the same size..
  int height = FreeImage_GetHeight(imagenDif);

  GLubyte *textureDif = new GLubyte[4 * width * height];
  GLubyte *textureNrm = new GLubyte[4 * width * height];
  GLubyte *textureSpec = new GLubyte[4 * width * height];

  char *pixelsDif = (char *)FreeImage_GetBits(imagenDif);
  char *pixelsNrm = (char *)FreeImage_GetBits(imagenNrm);
  char *pixelsSpec = (char *)FreeImage_GetBits(imagenSpec);
  // FreeImage loads in BGR format, so you need to swap some bytes(Or use
  // GL_BGR).

  for (int j = 0; j < width * height; j++) {
    textureDif[j * 4 + 0] = pixelsDif[j * 4 + 2];
    textureDif[j * 4 + 1] = pixelsDif[j * 4 + 1];
    textureDif[j * 4 + 2] = pixelsDif[j * 4 + 0];
    textureDif[j * 4 + 3] = pixelsDif[j * 4 + 3];

    textureNrm[j * 4 + 0] = pixelsNrm[j * 4 + 2];
    textureNrm[j * 4 + 1] = pixelsNrm[j * 4 + 1];
    textureNrm[j * 4 + 2] = pixelsNrm[j * 4 + 0];
    textureNrm[j * 4 + 3] = pixelsNrm[j * 4 + 3];

    textureSpec[j * 4 + 0] = pixelsSpec[j * 4 + 2];
    textureSpec[j * 4 + 1] = pixelsSpec[j * 4 + 1];
    textureSpec[j * 4 + 2] = pixelsSpec[j * 4 + 0];
    textureSpec[j * 4 + 3] = pixelsSpec[j * 4 + 3];
  }
  // int tempMipmapLevel = gCurrentMipmapLevel;
  std::string name = textureNode->first_attribute("name")->value();
  gMap_TextureNameToMipmapLevel[name] = gCurrentMipmapLevel;

  // Assume the texture is already bound to the GL_TEXTURE_2D target
  glTexSubImage2D(GL_TEXTURE_2D, // 2D texture
                  0,             // Level 0
                  (width * gCurrentMipmapLevel),
                  (width * gCurrentMipmapLevel), // Offset 0, 0
                  width, width,     // 1024 x 1024 texels, replace entire image
                  GL_RGBA,          // Four channel data
                  GL_UNSIGNED_BYTE, // Floating point data
                  (GLvoid *)textureDif); // Pointer to data
  delete[] textureDif;

  // Assume the texture is already bound to the GL_TEXTURE_2D target
  glTexSubImage2D(GL_TEXTURE_2D, // 2D texture
                  0,             // Level 1
                  width * (gCurrentMipmapLevel + 1),
                  width * (gCurrentMipmapLevel + 1), // Offset 0, 0
                  width, width,     // 1024 x 1024 texels, replace entire image
                  GL_RGBA,          // Four channel data
                  GL_UNSIGNED_BYTE, // Floating point data
                  (GLvoid *)textureNrm); // Pointer to data
  delete[] textureNrm;

  // Assume the texture is already bound to the GL_TEXTURE_2D target
  glTexSubImage2D(GL_TEXTURE_2D, // 2D texture
                  0,             // Level 1
                  width * (gCurrentMipmapLevel + 2),
                  width * (gCurrentMipmapLevel + 2), // Offset 0, 0
                  width, width,     // 1024 x 1024 texels, replace entire image
                  GL_RGBA,          // Four channel data
                  GL_UNSIGNED_BYTE, // Floating point data
                  (GLvoid *)textureSpec); // Pointer to data
  delete[] textureSpec;

  gCurrentMipmapLevel += 3;
  //	int MaxTextureImageUnits;
  //  glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &MaxTextureImageUnits);
}

void cTextureManager::loadWorldTilesFromImage(rapidxml::xml_node<> *worldNode) {
  // std::vector<std::vector <std::vector<std::tuple<float, float, float>>>>
  // vecWorldTiles;
  g_vec_WorldTiles.resize(512);
  for (int i = 0; i < 512; ++i) {
    g_vec_WorldTiles[i].resize(512);

    for (int j = 0; j < 512; ++j)
      g_vec_WorldTiles[i][j].resize(3);
  }
  int curLayer = 0;

  // Iterate over each world node
  for (rapidxml::xml_node<> *cWorldLayer_node =
           worldNode->first_node("WorldTileLayer");
       cWorldLayer_node; cWorldLayer_node = cWorldLayer_node->next_sibling()) {
    int numTilesInLayer = 0;

    std::string path = cWorldLayer_node->first_attribute("path")->value();
    FREE_IMAGE_FORMAT imgFormat =
        FreeImage_GetFileType(path.c_str(), 0); // Get current image format
    FIBITMAP *imagen = FreeImage_Load(imgFormat, path.c_str());
    imagen = FreeImage_ConvertTo32Bits(imagen);
    // imagen = FreeImage_ConvertTo32Bits(imagen);

    int width = FreeImage_GetWidth(imagen);
    int height = FreeImage_GetHeight(imagen);

    for (int ncW = 0; ncW < width; ncW++) {
      for (int ncD = 0; ncD < height; ncD++) {

        RGBQUAD color;
        FreeImage_GetPixelColor(imagen, ncW, ncD, &color);
        float r, g, b;
        r = (float)color.rgbRed;
        g = (float)color.rgbGreen;
        b = (float)color.rgbBlue;
        bool isWhite = bool(b == 255.0f && g == 255.0f && r == 255.0f);
        bool isMagic = bool(b == 245.0f && g == 0.0f && r == 245.0f);
        bool isSpider = bool(b == 0.0f && g == 64.0f && r == 128.0f);
        if (isSpider)
          g_pEntityManager->createSpiderAtPos(128.0f * ncW, 0, 128.0f * ncD);
        else if (!isWhite && !isMagic) {
          numTilesInLayer++;
          g_vec_WorldTiles[ncW][ncD][curLayer] = glm::vec3(r, g, b);
          // std::cout << ncW << " " << ncD << " " << r << " " << g << " " << b
          // << std::endl;
        }
        // if (!isWhite)
        // g_multimap_WorldTilesRGB.insert(std::pair<std::pair<int, int>,
        // std::tuple<float, float, float>>(std::make_pair(std::make_pair(ncW,
        // ncD), std::make_tuple(r, g, b))));
      }
    }
    std::cout << numTilesInLayer << std::endl;
    curLayer++;
  }
  g_pMeshManager->loadWorldTiles(
      g_pMeshManager->m_MapMeshNameTocMeshEntry
          [worldNode->first_attribute("name")->value()]);
}

GLuint cTextureManager::loadCubeMap(rapidxml::xml_node<> *cubeNode) {
  int width, height;
  gUniform_TextureID.push_back(0);

  glGenTextures(1, &gUniform_TextureID.back());
  g_pMapTextureNameToID["Skybox"] = gNextTextureID;

  glActiveTexture(GL_TEXTURE0 + gNextTextureID);
  gNextTextureID++;
  glBindTexture(GL_TEXTURE_CUBE_MAP, gUniform_TextureID.back());

  int nc = 0;
  for (rapidxml::xml_node<> *face_node = cubeNode->first_node("Face");
       face_node; face_node = face_node->next_sibling()) {
    FREE_IMAGE_FORMAT imgFormat = FreeImage_GetFileType(
        face_node->first_attribute("path")->value(), 0); // Get current format
    FIBITMAP *imagen =
        FreeImage_Load(imgFormat, face_node->first_attribute("path")->value());

    imagen = FreeImage_ConvertTo32Bits(imagen);

    width = FreeImage_GetWidth(imagen);
    height = FreeImage_GetHeight(imagen);

    GLubyte *textura = new GLubyte[4 * width * height];
    char *pixeles = (char *)FreeImage_GetBits(imagen);
    // FreeImage loads in BGR format, so you need to swap some bytes(Or use
    // GL_BGR).

    for (int j = 0; j < width * height; j++) {
      textura[j * 4 + 0] = pixeles[j * 4 + 2];
      textura[j * 4 + 1] = pixeles[j * 4 + 1];
      textura[j * 4 + 2] = pixeles[j * 4 + 0];
      textura[j * 4 + 3] = pixeles[j * 4 + 3];
      //	std::cout<<j<<":
      //"<<textura[j*4+0]<<"**"<<textura[j*4+1]<<"**"<<textura[j*4+2]<<"**"<<textura[j*4+3]<<std::endl;
    }

    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + nc, 0, GL_RGBA, width, height,
                 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid *)textura);
    nc++;
  }
  glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
  // Set up texture maps
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
  return gUniformId_Texture1;
}
