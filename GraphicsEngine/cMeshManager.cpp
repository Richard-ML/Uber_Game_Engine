#include "cMeshManager.h"
#include "stdafx.h"
#include "global.h"


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


cMeshManager *cMeshManager::instance() {
	if (!s_cMeshManager)
		s_cMeshManager = new cMeshManager();
	return s_cMeshManager;
}

//////////////////////////////////////////////////////////////////
//////   loadPlyFileIntoGLBuffer Method - ply file loader   //////
/////////////////////////////////////////////////////////////////
/*
- Loads file from disk
- Copies information into VAO (Vertex Array Object)
*/

// TODO: I know it does not load into gl buffer yet...
bool cMeshManager::loadMeshFileIntoGLBuffer(std::string name, const char *path, float scale) {
	cMeshEntry entryOut; // not actually entry out.. so fix this TODO: 
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

	unsigned int vertexOffset = vertices.size();
	unsigned int indexOffset = indices.size();
	unsigned int numgVertices = mesh->mNumVertices;
	unsigned int numgIndices = 3 * mesh->mNumFaces; // assume triangles

	entryOut.BaseIndex = indexOffset;
	entryOut.BaseVertex = vertexOffset;
	entryOut.NumgVertices = numgVertices;
	entryOut.NumgIndices = numgIndices;

	// Fill gVertices positions
	vertices.resize(vertexOffset + numgVertices);
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
		cMeshVertex &vert1 = vertices[vertexOffset + i];
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
			tempTextureUnit0 = g_pTextureManager->mapTextureNameToMipmapLevel[textureStr.C_Str()];
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
		vert1.Position = glm::vec4( pos1.x, pos1.y, pos1.z, 1.f);

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
		vert1.TexCoord = glm::vec4(UVW.x, UVW.y, UVW.x, UVW.y);
		vert1.Normal = glm::vec4(n1.x, n1.y, n1.z, 1.f);
		vert1.Tangent = glm::vec4(t1.x, t1.y, t1.z, 1.f);
	}


	indices.resize(indexOffset + numgIndices);
	for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
		unsigned int baseInd = 3 * i;
		// Assume the model has only triangles.
		indices[indexOffset + baseInd] = mesh->mFaces[i].mIndices[0];
		indices[indexOffset + baseInd + 1] = mesh->mFaces[i].mIndices[1];
		indices[indexOffset + baseInd + 2] = mesh->mFaces[i].mIndices[2];

		triFace *tempTriFace = new triFace();

		tempTriFace->v1 =
			glm::vec3(vertices[indices[indexOffset + baseInd]].Position.x,
				vertices[indices[indexOffset + baseInd]].Position.y,
				vertices[indices[indexOffset + baseInd]].Position.z);
		tempTriFace->v2 =
			glm::vec3(vertices[indices[indexOffset + baseInd + 1]].Position.x,
				vertices[indices[indexOffset + baseInd + 1]].Position.y,
				vertices[indices[indexOffset + baseInd + 1]].Position.z);
		tempTriFace->v3 =
			glm::vec3(vertices[indices[indexOffset + baseInd + 2]].Position.x,
				vertices[indices[indexOffset + baseInd + 2]].Position.y,
				vertices[indices[indexOffset + baseInd + 2]].Position.z);
		tempVecTriFace.push_back(tempTriFace);
	}

	// Will add a way to send mesh to physics engine to generate aabb and stuff

	// TODO: Create the mesh's AABB based on its MAX and MIN extents
	// AABB* aabb = new AABB(min, max, tempVecTriFace); // Voxel grid cell inside
	// of the world generation will contain these AABBs which point to the corresponding
	// trifaces MAJOR enhancement..

	meshFaces.push_back(tempVecTriFace);
	m_MapMeshNameTocMeshEntry[name] = entryOut;
	return true;
}

void cMeshManager::loadWorld()
{
//	for (int ncW = 0; ncW < 64; ncW++)
//	{
//		for (int ncD = 0; ncD < 64; ncD++)
//		{
//			float offsetX = (128 * ncW);
//			float offsetZ = (128 * ncD);
//
//
//
//
//
//
//
//
//
//
//
//		// cMeshEntry meshEntry = 	g_pMeshManager->m_MapMeshNameTocMeshEntry[ g_pMeshManager->m_mapRGBToMeshName[ g_pTextureManager->worldTiles[ncW][ncD] ] ]
//		//std::make_tuple(g_vec_WorldTiles[ncW][ncD][layer][0], g_vec_WorldTiles[ncW][ncD][layer][1], g_vec_WorldTiles[ncW][ncD][layer][2])
//		}
//	}
}

