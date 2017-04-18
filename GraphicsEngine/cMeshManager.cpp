#include "cMeshManager.h"
#include "stdafx.h"
#include "global.h"


#include <sstream>
#include <fbxsdk.h>
// The PIMPL idiom aka Compilation Firewall
// Purpose: Encapsulate private member variables. Reduces make-time,
// compile-time, and the Fragile Binary Interface Problem.
class cMeshManager_Impl : public cMeshManager {
	// Boilerplate

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Manager for the meshes. </summary>
	///
	/// <remarks>	Richard, 4/12/2017. </remarks>
	///-------------------------------------------------------------------------------------------------

	friend class cMeshManager;

public:
	//	static std::map<std::tuple<float, float, float>, aiMesh>
	//m_map_RGBToMesh;
};

///-------------------------------------------------------------------------------------------------
/// <summary>	Gets the implementation. </summary>
///
/// <remarks>	Richard, 4/12/2017. </remarks>
///
/// <returns>	Null if it fails, else a pointer to a const cMeshManager_Impl. </returns>
///-------------------------------------------------------------------------------------------------

inline const cMeshManager_Impl *cMeshManager::impl() const {
	return static_cast<const cMeshManager_Impl *>(this);
}

///-------------------------------------------------------------------------------------------------
/// <summary>	Gets the implementation. </summary>
///
/// <remarks>	Richard, 4/12/2017. </remarks>
///
/// <returns>	Null if it fails, else a pointer to a cMeshManager_Impl. </returns>
///-------------------------------------------------------------------------------------------------

inline cMeshManager_Impl *cMeshManager::impl() {
	return static_cast<cMeshManager_Impl *>(this);
}

///-------------------------------------------------------------------------------------------------
/// <summary>	Gets the instance. </summary>
///
/// <remarks>	Richard, 4/12/2017. </remarks>
///
/// <returns>	Null if it fails, else a pointer to a cMeshManager. </returns>
///-------------------------------------------------------------------------------------------------

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
///-------------------------------------------------------------------------------------------------
/// <summary>	Loads mesh file into gl buffer. </summary>
///
/// <remarks>	Richard, 4/12/2017. </remarks>
///
/// <param name="name"> 	The name. </param>
/// <param name="path"> 	Full pathname of the file. </param>
/// <param name="scale">	The scale. </param>
///
/// <returns>	True if it succeeds, false if it fails. </returns>
///-------------------------------------------------------------------------------------------------

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
	std::vector<sTriangleFace *> tempVecsTriangleFace;
	// tempVecsTriangleFace.reserve(mesh->mNumFaces);

	// Extents for Voxel Grid and AABB
	glm::vec3 min(0.0f);
	glm::vec3 max(0.0f);
	for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
		/// aiColor4t<float>* color = mesh->mColors[i];
		aiVector3D UVW;
		aiVector3D pos1 = mesh->mVertices[i];
		pos1 *= scale;
		aiVector3D t1;
		aiVector3D n1 = mesh->mNormals[i];
		if (mesh->mTangents != NULL)
			t1 = mesh->mTangents[i];
		sMeshVertex &vert1 = vertices[vertexOffset + i];
		glm::ivec4 tempTextureUnit0 = glm::ivec4(0);
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
		vert1.Position = glm::vec4(pos1.x, pos1.y, pos1.z, 1.f);

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
		vert1.textureUnits = tempTextureUnit0;
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

		sTriangleFace *tempsTriangleFace = new sTriangleFace();

		tempsTriangleFace->v1 =
			glm::vec3(vertices[indices[indexOffset + baseInd]].Position.x,
				vertices[indices[indexOffset + baseInd]].Position.y,
				vertices[indices[indexOffset + baseInd]].Position.z);
		tempsTriangleFace->v2 =
			glm::vec3(vertices[indices[indexOffset + baseInd + 1]].Position.x,
				vertices[indices[indexOffset + baseInd + 1]].Position.y,
				vertices[indices[indexOffset + baseInd + 1]].Position.z);
		tempsTriangleFace->v3 =
			glm::vec3(vertices[indices[indexOffset + baseInd + 2]].Position.x,
				vertices[indices[indexOffset + baseInd + 2]].Position.y,
				vertices[indices[indexOffset + baseInd + 2]].Position.z);
		tempVecsTriangleFace.push_back(tempsTriangleFace);
	}

	meshFaces.push_back(tempVecsTriangleFace);
	m_MapMeshNameTocMeshEntry[name] = entryOut;

		sBoundingBox boundingBox;
		boundingBox.scale = (max - min); // NOTE: This is not the boxes half-widths. This is the actual scale factor that is applied to the cube primitive. 
										 //glm::vec3(max.x - min.x, (max.y - min.y), max.z - min.z);
		boundingBox.position = (max - min) / 2.0f;
		m_MapMeshNameToAABB[name] = boundingBox;



		if (name != "Skeleton" && name != "FloorTile")
		{
			std::cout << "Num Verts " << mesh->mNumVertices << std::endl;
			const unsigned int numIndices = mesh->mNumFaces * 3;
			const unsigned int numVertices = mesh->mNumVertices;
			g_pWorld->generatePhysicsMesh(name, &indices[indexOffset], &vertices[vertexOffset], numIndices, numVertices);
		}

		//g_pWorld->generateConvexHull(name, tempVecsTriangleFace);
	return true;
}

///-------------------------------------------------------------------------------------------------
/// <summary>	
/// Printing node details to the console
/// https://www.gamedev.net/resources/_/technical/graphics-programming-and-theory/how-to-work-with-fbx-sdk-r3582
/// http://docs.autodesk.com/FBX/2014/ENU/FBX-SDK-Documentation/ 
/// </summary>
/// BEGIN
///-------------------------------------------------------------------------------------------------

///-------------------------------------------------------------------------------------------------
/// <summary> Tab character ("\t") counter </summary>
///-------------------------------------------------------------------------------------------------
int tabs_count = 0;

///-------------------------------------------------------------------------------------------------
/// <summary>	Print the required number of tabs. </summary>
///
/// <remarks>	Richard, 4/17/2017. </remarks>
///-------------------------------------------------------------------------------------------------
void cMeshManager::printTabs() {
	for (int i = 0; i < tabs_count; i++)
		printf("\t");
}

///-------------------------------------------------------------------------------------------------
/// <summary>	Return a string-based representation based on the attribute type. </summary>
///
/// <remarks>	Richard, 4/17/2017. </remarks>
///
/// <param name="type">	The type. </param>
///
/// <returns>	The attribute type name as string. </returns>
///-------------------------------------------------------------------------------------------------
FbxString cMeshManager::getAttributeTypeName(FbxNodeAttribute::EType type) {
	switch (type) {
	case FbxNodeAttribute::eUnknown: return "UNKNOWN";
	case FbxNodeAttribute::eNull: return "NULL";
	case FbxNodeAttribute::eMarker: return "MARKER";
	case FbxNodeAttribute::eSkeleton: return "SKELETON";
	case FbxNodeAttribute::eMesh: return "MESH";
	case FbxNodeAttribute::eNurbs: return "NURBS";
	case FbxNodeAttribute::eNurbsCurve: return "NURBS_CURVE";
	case FbxNodeAttribute::eNurbsSurface: return "NURBS_SURFACE";
	case FbxNodeAttribute::ePatch: return "PATCH";
	case FbxNodeAttribute::eCamera: return "CAMERA";
	case FbxNodeAttribute::eCameraStereo: return "CAMERA_STEREO";
	case FbxNodeAttribute::eCameraSwitcher: return "CAMERA_SWITCHER";
	case FbxNodeAttribute::eLight: return "LIGHT";
	case FbxNodeAttribute::eOpticalReference: return "OPTICLAL_REFERENCE";
	case FbxNodeAttribute::eOpticalMarker: return "OPTICAL_MARKER";
	case FbxNodeAttribute::eTrimNurbsSurface: return "TRIM_NURBS_SURFACE";
	case FbxNodeAttribute::eBoundary: return "BOUNDARY";
	case FbxNodeAttribute::eShape: return "SHAPE";
	case FbxNodeAttribute::eLODGroup: return "LOD_GROUP";
	case FbxNodeAttribute::eSubDiv: return "SUB_DIVISION";
	default: return "DEFAULT_VALUE";
	}
}

///-------------------------------------------------------------------------------------------------
/// <summary>	Prints an attribute. </summary>
///
/// <remarks>	Richard, 4/17/2017. </remarks>
///
/// <param name="pAttribute">	[in,out] If non-null, the attribute. </param>
///-------------------------------------------------------------------------------------------------
void cMeshManager::printAttribute(FbxNodeAttribute* pAttribute) {
	if (!pAttribute) return;

	FbxString typeName = getAttributeTypeName(pAttribute->GetAttributeType());
	FbxString attrName = pAttribute->GetName();
	printTabs();
	/// <summary> Use Buffer() to retrieve character array of FBX string. </summary>
	printf("<attribute type='%s' name='%s'/>\n", typeName.Buffer(), attrName.Buffer());
}

///-------------------------------------------------------------------------------------------------
/// <summary>
/// Printing node details to the console. Prints the node, its attributes, and all its children
/// recursively.
/// </summary>
///
/// <remarks> Richard, 4/17/2017. </remarks>
///
/// <param name="pNode"> [in,out] If non-null, the node. </param>
///-------------------------------------------------------------------------------------------------
void cMeshManager::printNode(FbxNode* pNode) {
	printTabs();
	const char* nodeName = pNode->GetName();
	FbxDouble3 translation = pNode->LclTranslation.Get();
	FbxDouble3 rotation = pNode->LclRotation.Get();
	FbxDouble3 scale = pNode->LclScaling.Get();

	/// <summary> Print the properties of the node. </summary>
	printf("<node name='%s' translation='(%f, %f, %f)' rotation='(%f, %f, %f)' scaling='(%f, %f, %f)'>\n",
		nodeName,
		translation[0], translation[1], translation[2],
		rotation[0], rotation[1], rotation[2],
		scale[0], scale[1], scale[2]
	);
	std::string name = nodeName;
	if (name == "skeleton_footman")
	{
		processFBXMeshNode(pNode);
	}
	tabs_count++;

	/// <summary> Print each of the node's attributes. </summary>
	for (int i = 0; i < pNode->GetNodeAttributeCount(); i++)
		printAttribute(pNode->GetNodeAttributeByIndex(i));

	/// <summary> Print each of children with a recursive call to this function. </summary>
	for (int j = 0; j < pNode->GetChildCount(); j++)
		printNode(pNode->GetChild(j));

	tabs_count--;
	printTabs();
	printf("</node>\n");
}

bool cMeshManager::processFBXMeshNode(FbxNode* meshNode)
{
	FbxMesh* pMesh = meshNode->GetMesh();
	/// <summary> Create and initialize a new Mesh Entry. </summary>
	cMeshEntry meshEntry;
	meshEntry.BaseIndex = 0;
	meshEntry.BaseVertex = 0;
	meshEntry.NumgIndices = 0;

	unsigned int vertexOffset = vertices.size();
	unsigned int indexOffset = indices.size();
	unsigned int numgVertices = pMesh->GetPolygonVertexCount();
	unsigned int numgIndices = pMesh->GetPolygonCount() * 3;


	meshEntry.BaseIndex = indexOffset;
	meshEntry.BaseVertex = vertexOffset;
	meshEntry.NumgVertices = numgVertices;
	meshEntry.NumgIndices = numgIndices;

	// Fill gVertices positions
	vertices.resize(vertexOffset + pMesh->GetPolygonVertexCount());
	indices.resize(indexOffset + numgIndices);
	std::vector<sTriangleFace *> tempVecsTriangleFace;
	tempVecsTriangleFace.reserve(pMesh->GetPolygonCount());

     FbxLayerElementUV* UVs = pMesh->GetLayer(0)->GetUVs();
	 FbxLayerElementNormal * normals =  pMesh->GetLayer(0)->GetNormals();
	 
	double scale = 0.075;
	
		// Get control points (=vertices for a mesh)
		FbxVector4* fbxControlPoints = pMesh->GetControlPoints();


		/// <summary> Load Vertices </summary>
	for (int ncPolygon = 0; ncPolygon < pMesh->GetPolygonCount(); ncPolygon++) {
		// For each vertex in the polygon ( assume triangles ) 
		for (unsigned ncVertex = 0; ncVertex < 3; ncVertex++) {
			FbxVector4 vert = pMesh->GetControlPointAt((ncPolygon * 3) + ncVertex);
			sMeshVertex &vert1 = vertices[vertexOffset + (ncPolygon * 3) + ncVertex];

			vert1.textureUnits = g_pTextureManager->mapTextureNameToMipmapLevel["Skeleton"];
			//int fbxCornerIndex = pMesh->GetPolygonVertex(ncPolygon, ncVertex);

			FbxVector4 fbxVertex = fbxControlPoints[(ncPolygon * 3) + ncVertex];
			// Get vertex position
			vert1.Position = glm::vec4((float)fbxVertex.mData[0] * scale, (float)fbxVertex.mData[1] * scale, (float)fbxVertex.mData[2] * scale, 1.f);

			// Get normal
			FbxVector4 fbxNormal;
			pMesh->GetPolygonVertexNormal(ncPolygon, ncVertex, fbxNormal);
			fbxNormal.Normalize();
			vert1.Normal = glm::vec4((float)fbxNormal.mData[0], (float)fbxNormal.mData[1], (float)fbxNormal.mData[2], (float)fbxNormal.mData[3]);

			// Get texture coordinates
			FbxLayerElementUV * texCoords =	pMesh->GetLayer(0)->GetUVs();
			                                   //V Should be ncPolygon TEXTURE COORDINATE ISSUE HERE.
			int index = pMesh->GetTextureUVIndex(6, ncVertex);
			vert1.TexCoord = glm::vec4((float)texCoords->GetDirectArray().GetAt(index)[0],(float)texCoords->GetDirectArray().GetAt(index)[1],1.0f, 0.0f);
		}
	}
	
		for (int nc = 0; nc < pMesh->GetPolygonVertexCount(); nc++)
		{
			indices[indexOffset + nc] = pMesh->GetPolygonVertices()[nc];
		}


		
	meshFaces.push_back(tempVecsTriangleFace);
	m_MapMeshNameTocMeshEntry["Skeleton"] = meshEntry;

	sBoundingBox boundingBox;
	boundingBox.scale = (glm::vec3(9.0f, 12.5f, 7.0f)); // NOTE: This is not the boxes half-widths. This is the actual scale factor that is applied to the cube primitive. 
									 //glm::vec3(max.x - min.x, (max.y - min.y), max.z - min.z);
	//boundingBox.position = glm::vec3(0.0f);
	m_MapMeshNameToAABB["Skeleton"] = boundingBox;

	return true;
}

///-------------------------------------------------------------------------------------------------
/// <summary>	Loads FBX mesh file into gl buffer. </summary>
///
/// <remarks>	Richard, 4/12/2017. </remarks>
///
/// <param name="name"> 	The name. </param>
/// <param name="path"> 	Full pathname of the file. </param>
/// <param name="scale">	The scale. </param>
///
/// <returns>	True if it succeeds, false if it fails. </returns>
///-------------------------------------------------------------------------------------------------
bool cMeshManager::loadFBXMeshFileIntoGLBuffer(std::string name, const char * path, float scale)
{
	/// <summary> Create the SDK manager which handles memory management. </summary>
	FbxManager* pSdkManager = FbxManager::Create();
	/// <summary> Create the IO settings object. </summary>
	FbxIOSettings *pIO_settings = FbxIOSettings::Create(pSdkManager, IOSROOT);
	pSdkManager->SetIOSettings(pIO_settings);

	/// <summary> Create the importer. </summary>
	FbxImporter* pImporter = FbxImporter::Create(pSdkManager, "");

	/// <summary> Pass filename to the importer. </summary>
	if (!pImporter->Initialize(path, -1, pSdkManager->GetIOSettings())) {
		printf("Call to FbxImporter::Initialize() failed.\n");
		printf("Error returned: %s\n\n", pImporter->GetStatus().GetErrorString());
		exit(-1);
	}

	/// <summary> Create a new Scene. </summary>
	FbxScene* pScene = FbxScene::Create(pSdkManager, "myScene");

	/// <summary> Import contents of FBX file into the scene. </summary>
	pImporter->Import(pScene);

	/// <summary> The file is now loaded into the scene. So we no longer need the importer. </summary>
	pImporter->Destroy();

	/// <summary> Print the nodes of the scene and their attributes recursively.
	/// NOTE: We are not printing the root node because it should
	///		  not contain any attributes. </summary>
	FbxNode* pRootNode = pScene->GetRootNode();
	if (pRootNode) {
		for (int i = 0; i < pRootNode->GetChildCount(); i++)
			printNode(pRootNode->GetChild(i));
	}
	/// <summary> Destroy the SDK manager deleting all objects that belong to it. </summary>
	pSdkManager->Destroy();

	return true;
}

///-------------------------------------------------------------------------------------------------
/// <summary>	
/// Printing node details to console
/// https://www.gamedev.net/resources/_/technical/graphics-programming-and-theory/how-to-work-with-fbx-sdk-r3582
/// http://docs.autodesk.com/FBX/2014/ENU/FBX-SDK-Documentation/ 
/// </summary>
/// END
///-------------------------------------------------------------------------------------------------
