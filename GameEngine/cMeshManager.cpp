#include "cMeshManager.h"

#include <algorithm>
// ASSIMP is isolated here
#include "global.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <iostream>
#include <limits>
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

// The PIMPL idiom aka Compilation Firewall
// Purpose: Encapsulate private member variables. Reduces make-time,
// compile-time, and the Fragile Binary Interface Problem.
class cMeshManager_Impl : public cMeshManager {
  // Boilerplate
  friend class cMeshManager;

public:
  //	static std::map<std::tuple<float, float, float>, aiMesh>
  //m_map_RGBToMesh;
};
inline const cMeshManager_Impl *cMeshManager::impl() const {
  return static_cast<const cMeshManager_Impl *>(this);
}
inline cMeshManager_Impl *cMeshManager::impl() {
  return static_cast<cMeshManager_Impl *>(this);
}
// Initialize vectors
;

cMeshManager *cMeshManager::instance() {
  if (!s_cMeshManager)
    s_cMeshManager = new cMeshManager();
  return s_cMeshManager;
}
// bool cMeshManager::loadMeshTileFileIntoTileMap(const char * path, float
// scale, float r, float g, float b) {
//	Assimp::Importer importer;
//	unsigned int flags = 0;
//	flags |= aiProcess_GenSmoothNormals;
//	const aiScene* scene = importer.ReadFile(path, flags);
//	if (!scene) {
//		fprintf(stderr, importer.GetErrorString());
//		return false;
//	}
//
//	this->impl()->m_map_RGBToMesh[std::make_tuple(r, g, b)] =
//*scene->mMeshes[0];
// }

void cMeshManager::loadWorldTiles(cMeshEntry &entryOut) {
  entryOut.BaseIndex = 0;
  entryOut.BaseVertex = 0;
  entryOut.NumgIndices = 0;

  unsigned int vertexOffset = gVertices.size();
  entryOut.BaseVertex = vertexOffset;

  unsigned int indexOffset = gIndices.size();
  entryOut.BaseIndex = indexOffset;

  unsigned int sumNumVertices = 0;
  unsigned int sumNumIndices = 0;

  std::vector<triFace *> tempVecTriFace;
  tempVecTriFace.reserve(2'310'720);
  // gAABBs.reserve(4980736);

  gVertices.reserve(3'310'720);
  gIndices.reserve(2'310'720);

  float scale = 1.0f; // TODO: actual scale here
  int numTiles = 0;
  std::cout << "Loading world data..\n";
  float distanceX = glm::distance(g_pAreaInfo->minPos.x, g_pAreaInfo->maxPos.x);
  float distanceZ = glm::distance(g_pAreaInfo->minPos.z, g_pAreaInfo->maxPos.z);

  for (int ncW = 0; ncW < 512; ncW++) {
    for (int ncD = 0; ncD < 512; ncD++) {
      for (int layer = 0; layer < 3; layer++) {
        glm::vec3 min = glm::vec3(std::numeric_limits<float>::max());
        glm::vec3 max = glm::vec3(std::numeric_limits<float>::min());
        // TODO: Stuff and things
        if (g_vec_WorldTiles[ncW][ncD][layer] != glm::vec3(0.0f, 0.0f, 0.0f)) {
          numTiles++;
          cMeshEntry *mesh = g_map_RGBToMesh[std::make_tuple(
              g_vec_WorldTiles[ncW][ncD][layer][0],
              g_vec_WorldTiles[ncW][ncD][layer][1],
              g_vec_WorldTiles[ncW][ncD][layer][2])];
          gVertices.resize(gVertices.size() + mesh->NumgVertices);
          sumNumIndices += mesh->NumgIndices;
          sumNumVertices += mesh->NumgVertices;
          float offsetX = (128.0f * ncW);
          float offsetZ = (128.0f * ncD);
          AABB *aabb = new AABB();
          aabb->offsetToTriFaces = tempVecTriFace.size();
          aabb->numFaces = mesh->NumgIndices / 3;
          for (unsigned int i = 0; i < (unsigned)mesh->NumgVertices; i++) {
            glm::vec4 UVW = gVertices.at(mesh->BaseVertex + i).TexCoord;
            glm::vec4 pos1 = gVertices.at(mesh->BaseVertex + i).Position;
            glm::vec4 n1 = gVertices.at(mesh->BaseVertex + i).Normal;
            glm::vec4 t1 = gVertices.at(mesh->BaseVertex + i).Tangent;
            glm::uvec4 textureUnits =
                gVertices.at(mesh->BaseVertex + i).textureUnits;
            cMeshVertex &vert1 = gVertices[vertexOffset + i];
            vert1.textureUnits = textureUnits;
            Set(vert1.Position, pos1.x + offsetX, pos1.y, pos1.z + offsetZ,
                1.f);
            // std::cout << pos1.x + offsetX << " " << pos1.y <<  " " << pos1.z
            // + offsetZ << std::endl; // Used for debugging
            Set(vert1.TexCoord, UVW.x, UVW.y, UVW.x, UVW.y);
            Set(vert1.Normal, n1.x, n1.y, n1.z, 1.f);
            Set(vert1.Tangent, t1.x, t1.y, t1.z, t1.w);
            // EXTENTS USED FOR AABB
            if (vert1.Position.x < min.x)
              min.x = vert1.Position.x;

            if (vert1.Position.x > max.x)
              max.x = vert1.Position.x;

            if (vert1.Position.z < min.z)
              min.z = vert1.Position.z;

            if (vert1.Position.z > max.z)
              max.z = vert1.Position.z;

            if (vert1.Position.y < min.y)
              min.y = vert1.Position.y;

            if (vert1.Position.y > max.y)
              max.y = vert1.Position.y;
          }

          // Fill face gIndices
          gIndices.resize(gIndices.size() + mesh->NumgIndices);
          for (unsigned int i = 0; i < (unsigned)mesh->NumgIndices / 3; i++) {
            unsigned int baseInd = 3 * i;
            // Assume the model has only triangles! __for now__
            gIndices[indexOffset + baseInd] =
                vertexOffset + gIndices[mesh->BaseIndex + baseInd];
            gIndices[indexOffset + baseInd + 1] =
                vertexOffset + gIndices[mesh->BaseIndex + baseInd + 1];
            gIndices[indexOffset + baseInd + 2] =
                vertexOffset + gIndices[mesh->BaseIndex + baseInd + 2];

            triFace *tempTriFace = new triFace();
            tempTriFace->v1 = glm::vec3(
                gVertices[gIndices[indexOffset + baseInd]].Position.x,
                gVertices[gIndices[indexOffset + baseInd]].Position.y,
                gVertices[gIndices[indexOffset + baseInd]].Position.z);
            tempTriFace->v2 = glm::vec3(
                gVertices[gIndices[indexOffset + baseInd + 1]].Position.x,
                gVertices[gIndices[indexOffset + baseInd + 1]].Position.y,
                gVertices[gIndices[indexOffset + baseInd + 1]].Position.z);
            tempTriFace->v3 = glm::vec3(
                gVertices[gIndices[indexOffset + baseInd + 2]].Position.x,
                gVertices[gIndices[indexOffset + baseInd + 2]].Position.y,
                gVertices[gIndices[indexOffset + baseInd + 2]].Position.z);
            tempVecTriFace.push_back(tempTriFace);
          }
          // std::cout << ncW << " " << ncD << std::endl;
          indexOffset += mesh->NumgIndices;
          vertexOffset += mesh->NumgVertices;

          aabb->halfWidths = (max - min) * glm::vec3(0.5);
          aabb->center = min + aabb->halfWidths;
          aabb->nameOfMesh =
              "TestWorld"; // TODO: pass as arg (only one world at the moment)

          int numXCells =
              (int)glm::round(((distanceX + gVoxelSize) / gVoxelSize) + 1);
          int numZCells =
              (int)glm::round(((distanceZ + gVoxelSize) / gVoxelSize) + 1);
          for (int VOXEL_ROW = 0; VOXEL_ROW < numZCells; VOXEL_ROW++) {
            float depthOfAABB = max.z - min.z;
            float rowOffset = glm::distance(
                (float)(gVoxelSize * VOXEL_ROW),
                glm::distance(g_pAreaInfo->minPos.z, min.z + depthOfAABB));

            if (std::abs(rowOffset) < ((gVoxelSize) + depthOfAABB))
              for (int VOXEL_COL = 0; VOXEL_COL < numXCells; VOXEL_COL++) {
                float widthOfAABB = max.x - min.x;
                // Each cell has a width and length of 1.25* voxel cell size..
                // (they overlap) Trifaces can reside in multiple cells weww!
                float colOffset = glm::distance(
                    (float)(gVoxelSize * VOXEL_COL),
                    glm::distance(g_pAreaInfo->minPos.x, min.x + widthOfAABB));
                if (std::abs(colOffset) < ((gVoxelSize) + widthOfAABB))
                  g_multimap_VoxelGrid.insert(
                      std::pair<std::pair<int, int>, AABB *>(
                          std::make_pair(VOXEL_ROW, VOXEL_COL), aabb));
              }
          }
        }
      }
    }
  }
  std::cout << "Voxel cell size is " << gVoxelSize << "x" << gVoxelSize
            << " units." << std::endl;
  std::cout << "Area boundaries are x" << std::to_string(g_pAreaInfo->minPos.x)
            << " y" << std::to_string(g_pAreaInfo->minPos.y) << " z"
            << std::to_string(g_pAreaInfo->minPos.z) << " - x"
            << std::to_string(g_pAreaInfo->maxPos.x) << " y"
            << std::to_string(g_pAreaInfo->maxPos.y) << " z"
            << std::to_string(g_pAreaInfo->maxPos.z) << std::endl;
  std::cout << "Number of tiles in world: " << numTiles << std::endl;
  gMeshFaces.push_back(tempVecTriFace);
  entryOut.NumgIndices = sumNumIndices;
  entryOut.NumgVertices = sumNumVertices;
}

//    _                    _  __  __             _      _____  _  _        ___
//    _           ____  _      ____           __   __
//   | |  ___    __ _   __| ||  \/  |  ___  ___ | |__  |  ___|(_)| |  ___ |_ _|
//   _ __  | |_  ___   / ___|| |    | __ )  _   _  / _| / _|  ___  _ __
//   | | / _ \  / _` | / _` || |\/| | / _ \/ __|| '_ \ | |_   | || | / _ \ | | |
//   '_ \ | __|/ _ \ | |  _ | |    |  _ \ | | | || |_ | |_  / _ \| '__|
//   | || (_) || (_| || (_| || |  | ||  __/\__ \| | | ||  _|  | || ||  __/ | | |
//   | | || |_| (_) || |_| || |___ | |_) || |_| ||  _||  _||  __/| |
//   |_| \___/  \__,_| \__,_||_|  |_| \___||___/|_| |_||_|    |_||_|
//   \___||___||_| |_| \__|\___/  \____||_____||____/  \__,_||_|  |_|   \___||_|
//
//////////////////////////////////////////////////////////////////
//////   loadPlyFileIntoGLBuffer Method - ply file loader   //////
/////////////////////////////////////////////////////////////////
/*
- Loads file from disk
- Copies information into VAO (Vertex Array Object)
*/
bool cMeshManager::loadMeshFileIntoGLBuffer(const char *path,
                                            cMeshEntry &entryOut, float scale,
                                            bool isStationary) {
  entryOut.BaseIndex = 0;
  entryOut.BaseVertex = 0;
  entryOut.NumgIndices = 0;

  Assimp::Importer importer;
  unsigned int flags = 0;
  flags |= aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace;

  const aiScene *scene = importer.ReadFile(path, flags);
  if (!scene) {
    fprintf(stderr, importer.GetErrorString());
    return false;
  }
  const aiMesh *mesh =
      scene->mMeshes[0]; // In this simple example code we always use the 1rst
                         // mesh (in OBJ files there is often only one anyway)

  unsigned int vertexOffset = gVertices.size();
  unsigned int indexOffset = gIndices.size();
  unsigned int numgVertices = mesh->mNumVertices;
  unsigned int numgIndices = 3 * mesh->mNumFaces; // assume triangles

  entryOut.BaseIndex = indexOffset;
  entryOut.BaseVertex = vertexOffset;
  entryOut.NumgVertices = numgVertices;
  entryOut.NumgIndices = numgIndices;

  // Fill gVertices positions
  gVertices.resize(vertexOffset + numgVertices);
  std::vector<triFace *> tempVecTriFace;
  // tempVecTriFace.reserve(mesh->mNumFaces);

  // Extents for Voxel Grid and AABB
  glm::vec3 min(0.0f);
  glm::vec3 max(0.0f);
  for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
    /// aiColor4t<float>* color = mesh->mColors[i];
    aiVector3D UVW;
    aiVector3D pos1 = mesh->mVertices[i];
    pos1 *= scale;
    aiVector3D n1 = mesh->mNormals[i];
    aiVector3D t1 = mesh->mTangents[i];
    cMeshVertex &vert1 = gVertices[vertexOffset + i];
    GLuint tempTextureUnit0 = 0;
    if (mesh->mMaterialIndex >= 0) {
      aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
      // We assume a convention for sampler names in the shader. Each diffuse
      // texture should be named
      // as 'texture_diffuseN' where N is a sequential number ranging from 1 to
      // MAX_SAMPLER_NUMBER.
      // Same applies to other texture as the following list summarizes:
      // Diffuse: texture_diffuseN
      // Specular: texture_specularN
      // Normal: texture_normalN
      aiString textureStr;
      material->GetTexture(aiTextureType_DIFFUSE, 0, &textureStr);
      tempTextureUnit0 = gMap_TextureNameToMipmapLevel[textureStr.C_Str()];
      std::string textureResult = textureStr.C_Str();
      // 1. Diffuse maps
      // tempTextureInfo.x = (material->mProperties, aiTextureType_DIFFUSE,
      // "texture_diffuse");
      // textures.insert(textures.end(), diffuseMaps.begin(),
      // diffuseMaps.end());
      // 2. Specular maps
      // vector<Texture> specularMaps = this->loadMaterialTextures(material,
      // aiTextureType_SPECULAR, "texture_specular");
      // textures.insert(textures.end(), specularMaps.begin(),
      // specularMaps.end());
    }

    if (mesh->mNumUVComponents[0] != 0)
      UVW = mesh->mTextureCoords[0][i]; // Assume only 1 set of UV coordinates;
                                        // AssImp supports 8 UV sets.
    else
      UVW = aiVector3D(0.0f, 0.0f, 0.0f);
    Set(vert1.Position, pos1.x, pos1.y, pos1.z, 1.f);

    if (pos1.x < min.x)
      min.x = pos1.x;

    if (pos1.x > max.x)
      max.x = pos1.x;

    if (pos1.z < min.z)
      min.z = pos1.z;

    if (pos1.z > max.z)
      max.z = pos1.z;

    if (pos1.y < min.y)
      min.y = pos1.y;

    if (pos1.y > max.y)
      max.y = pos1.y;

    // vert1.TextureUnits = tempTextureInfo;
    vert1.textureUnits = glm::uvec4(0);
    vert1.textureUnits.x = tempTextureUnit0;
    // vert1.textureUnits.x = tempTextureUnit0;
    Set(vert1.TexCoord, UVW.x, UVW.y, UVW.x, UVW.y);
    Set(vert1.Normal, n1.x, n1.y, n1.z, 1.f);
    Set(vert1.Tangent, t1.x, t1.y, t1.z, 1.f);
  }

  //
  ////Generate Voxel Grid Cells Each Cell will be 1000+-(250) x 1000+-(250)
  ///units
  //
  float distanceX = glm::distance(g_pAreaInfo->minPos.x, g_pAreaInfo->maxPos.x);
  float distanceZ = glm::distance(g_pAreaInfo->minPos.z, g_pAreaInfo->maxPos.z);
  // g_pAreaInfo->minPos.z = glm::min(g_pAreaInfo->minPos.z, min.z);
  // g_pAreaInfo->minPos.x = glm::min(g_pAreaInfo->minPos.x, min.x);
  // g_pAreaInfo->maxPos.z = glm::max(g_pAreaInfo->maxPos.z, max.z);
  // g_pAreaInfo->maxPos.x = glm::max(g_pAreaInfo->maxPos.x, max.x);
  int numXCells = (int)glm::round(((distanceX + gVoxelSize) / gVoxelSize) + 1);
  int numZCells =
      (int)glm::round(((distanceZ + gVoxelSize) / gVoxelSize) + 1); // TODO: Fix

  // if (isStationary)
  //{
  //	std::cout << "Processing " << mesh->mNumFaces << " faces into voxel grid
  //cells.. This won't take long.." << std::endl;
  //}
  // Fill face gIndices
  gIndices.resize(indexOffset + numgIndices);
  for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
    unsigned int baseInd = 3 * i;
    // Assume the model has only triangles.
    gIndices[indexOffset + baseInd] = mesh->mFaces[i].mIndices[0];
    gIndices[indexOffset + baseInd + 1] = mesh->mFaces[i].mIndices[1];
    gIndices[indexOffset + baseInd + 2] = mesh->mFaces[i].mIndices[2];

    triFace *tempTriFace = new triFace();

    tempTriFace->v1 =
        glm::vec3(gVertices[gIndices[indexOffset + baseInd]].Position.x,
                  gVertices[gIndices[indexOffset + baseInd]].Position.y,
                  gVertices[gIndices[indexOffset + baseInd]].Position.z);
    tempTriFace->v2 =
        glm::vec3(gVertices[gIndices[indexOffset + baseInd + 1]].Position.x,
                  gVertices[gIndices[indexOffset + baseInd + 1]].Position.y,
                  gVertices[gIndices[indexOffset + baseInd + 1]].Position.z);
    tempTriFace->v3 =
        glm::vec3(gVertices[gIndices[indexOffset + baseInd + 2]].Position.x,
                  gVertices[gIndices[indexOffset + baseInd + 2]].Position.y,
                  gVertices[gIndices[indexOffset + baseInd + 2]].Position.z);
    if ((std::string)path == "assets/Tiger.obj")
      playerMeshFaces.push_back(tempTriFace);
    tempVecTriFace.push_back(tempTriFace);
  }
  // Get the min max of all the vertices in the triface to resolve it's voxel
  // cell(s)
  // if (isStationary)
  //{
  //
  //	glm::vec3 mini;
  //	mini.x = glm::min(tempTriFace->v1.x, tempTriFace->v2.x);
  //	mini.x = glm::min(mini.x, tempTriFace->v3.x);
  //
  //	mini.z = glm::min(tempTriFace->v1.z, tempTriFace->v2.z);
  //	mini.z = glm::min(mini.z, tempTriFace->v3.z);
  //
  //	mini.y = glm::min(tempTriFace->v1.y, tempTriFace->v2.y);
  //	mini.y = glm::min(mini.y, tempTriFace->v3.y);
  //
  //	glm::vec3 maxi;
  //	maxi.x = glm::max(tempTriFace->v1.x, tempTriFace->v2.x);
  //	maxi.x = glm::max(maxi.x, tempTriFace->v3.x);
  //
  //	maxi.z = glm::max(tempTriFace->v1.z, tempTriFace->v2.z);
  //	maxi.z = glm::max(maxi.z, tempTriFace->v3.z);
  //
  //	maxi.y = glm::max(tempTriFace->v1.y, tempTriFace->v2.y);
  //	maxi.y = glm::max(maxi.y, tempTriFace->v3.y);
  //
  //	=
  //	float widthOfTri = (maxi.x - mini.x) / 2;
  //	float depthOfTri = (maxi.z - mini.z) / 2;
  //	// Sooo the bounds are dist(distance(X/Z), mini [+/-] (width/depth))
  //
  //	for (int VOXEL_ROW = 0; VOXEL_ROW < numZCells; VOXEL_ROW++)
  //	{
  //		float rowOffset = glm::distance((float)(gVoxelSize * VOXEL_ROW),
  //glm::distance(g_pAreaInfo->minPos.z, mini.z + depthOfTri));
  //
  //		if (std::abs(rowOffset) < ((gVoxelSize * 1.25) + depthOfTri))
  //			for (int VOXEL_COL = 0; VOXEL_COL < numXCells;
  //VOXEL_COL++)
  //			{
  //				//Each cell offsetX = VOXEL_ROW * 1000   offsetZ = VOXEL_COL
  //* 1000
  //				// Each cell has a width and length of 1500.. (they overlap)
  //Trifaces can reside in multiple cells weww!
  //				float colOffset = glm::distance((float)(gVoxelSize *
  //VOXEL_COL), glm::distance(g_pAreaInfo->minPos.x, mini.x + widthOfTri));
  //				if (std::abs(colOffset) < ((gVoxelSize * 1.25) +
  //widthOfTri))
  //				{
  //					g_multimap_VoxelGrid.insert(std::pair<std::pair<int,
  //int>, triFace*>(std::make_pair(VOXEL_ROW, VOXEL_COL), tempTriFace));
  //					//std::cout << "Triface coordinates range x: " << mini.x
  //<< " to " << maxi.x << " z: " << mini.z << " to " << maxi.z << "\nIs inside
  //\nvoxelCell[ " << VOXEL_COL << " , " << VOXEL_ROW << " ]'s Range x: " <<
  //					//	(int)colOffset-750 << " to " << (int)colOffset+750
  //<< " z: " << (int)rowOffset-750 << " to " << (int)rowOffset + 750 <<
  //std::endl;
  //				}
  //			}
  //	}
  //} // END OF if (isStationary) (CHECK XML DOC TO SEE IF A MESH IS STATIONARY)

  //}
  // if (isStationary)
  //{
  //	std::cout << "Voxel grid now contains " << g_multimap_VoxelGrid.size()
  //<< " pointers to triFaces in " << numXCells << "x" << numZCells << "
  //cells.." << std::endl;
  // std::cout << "Voxel cell size is " << gVoxelSize << "x" << gVoxelSize << "
  // units." << std::endl;
  // std::cout << "Area boundaries are x" <<
  // std::to_string(g_pAreaInfo->minPos.x) << " y" <<
  // std::to_string(g_pAreaInfo->minPos.y) << " z" <<
  // std::to_string(g_pAreaInfo->minPos.z) << " - x" <<
  // std::to_string(g_pAreaInfo->maxPos.x) << " y" <<
  // std::to_string(g_pAreaInfo->maxPos.y) << " z" <<
  // std::to_string(g_pAreaInfo->maxPos.z) << std::endl;
  gDistanceX = distanceX;
  gDistanceZ = distanceZ;
  gNumXCells = numXCells;
  gNumZCells = numZCells;
  //}

  // Create the meshes AABB based on its MAX and MIN extents
  // AABB* aabb = new AABB(min, max, tempVecTriFace); // Voxel grid cell inside
  // of the world generation will contain these AABBs instead of just all of the
  // trifaces MAJOR enhancement

  gMeshFaces.push_back(tempVecTriFace);
  return true;
}