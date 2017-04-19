#include "stdafx.h"
#include "cAnimatedMesh.h"
#include "global.h"
void cAnimatedMesh::initializeSceneHandle(FbxScene * pScene)
{
	m_pScene = pScene;
}
void cAnimatedMesh::loadPoses()
{
	// Get the amount of poses in this scene
	int poseCount = m_pScene->GetPoseCount();
	// Add each pose to the vector of poses
#ifdef PRINT_ANIMATED_MESH_DEBUG_INFO
	std::stringstream output;
	output << "\n\n<Poses>\n";
#endif // PRINT_ANIMATED_MESH_DEBUG_INFO
	for (int nc = 0; nc < poseCount; ++nc)
	{
		FbxPose* currPose = m_pScene->GetPose(nc);
		m_vec_pPoses.Add(currPose);
		// Print out some details about this pose in the console
		std::string poseName = currPose->GetName();

#ifdef PRINT_ANIMATED_MESH_DEBUG_INFO
		if (poseName == "")
			output << "\t" << "ERROR: UNNAMED POSE" << "\n";
		else
			output << "\t" << poseName;
		if (nc != poseCount - 1)
			output << "\n";
#endif // PRINT_ANIMATED_MESH_DEBUG_INFO
	}
#ifdef PRINT_ANIMATED_MESH_DEBUG_INFO
	output << "\n</Poses>\n";
	printf(output.str().c_str());
#endif // PRINT_ANIMATED_MESH_DEBUG_INFO
}
void cAnimatedMesh::loadMeshes()
{
	FbxNode* pRootNode = m_pScene->GetRootNode();
	for (int nc = 0; nc < pRootNode->GetChildCount(); nc++)
	{
		FbxNode* pNode = pRootNode->GetChild(nc);
		FbxMesh * pMeshNode = pNode->GetMesh();
		if (pMeshNode != 0)
		{
			processFBXMeshNode(pNode);
		}
	}
}
bool cAnimatedMesh::processFBXMeshNode(FbxNode * pMeshNode)
{
	FbxMesh* pMesh = pMeshNode->GetMesh();
	/// <summary> Create and initialize a new Mesh Entry. </summary>
	cMeshEntry meshEntry;
	meshEntry.BaseIndex = 0;
	meshEntry.BaseVertex = 0;
	meshEntry.NumgIndices = 0;

	///-------------------------------------------------------------------------------------------------
	/// <summary>
	/// Use the current size of vertices and indices arrays to determine the offset to the current
	/// mesh's data.
	/// </summary>
	///-------------------------------------------------------------------------------------------------
	unsigned int vertexOffset = g_pMeshManager->vertices.size();
	unsigned int indexOffset = g_pMeshManager->indices.size();
	unsigned int numgVertices = pMesh->GetPolygonVertexCount();
	unsigned int numgIndices = pMesh->GetPolygonCount() * 3;


	meshEntry.BaseIndex = indexOffset;
	meshEntry.BaseVertex = vertexOffset;
	meshEntry.NumgVertices = numgVertices;
	meshEntry.NumgIndices = numgIndices;

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Allocate the array memory, by control point or by polygon vertex. </summary>
	///-------------------------------------------------------------------------------------------------
	g_pMeshManager->vertices.resize(vertexOffset + numgVertices);
	g_pMeshManager->indices.resize(indexOffset + numgIndices);
	//std::vector<sTriangleFace *> tempVecsTriangleFace;
	//tempVecsTriangleFace.reserve(pMesh->GetPolygonCount());

	FbxLayerElementUV* UVs = pMesh->GetLayer(0)->GetUVs();
	FbxLayerElementNormal * normals = pMesh->GetLayer(0)->GetNormals();

	double scale = 0.075;

	// Get control points (vertices for a mesh)
	FbxVector4* fbxControlPoints = pMesh->GetControlPoints();


	/// <summary> Load Vertices </summary>
	for (int ncPolygon = 0; ncPolygon < pMesh->GetPolygonCount(); ncPolygon++) {
		// For each vertex in the polygon ( assume triangles ) 
		for (unsigned ncVertex = 0; ncVertex < 3; ncVertex++) {
			FbxVector4 vert = pMesh->GetControlPointAt((ncPolygon * 3) + ncVertex);
			sMeshVertex &vert1 = g_pMeshManager->vertices[vertexOffset + (ncPolygon * 3) + ncVertex];

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

			FbxLayerElementUV * texCoords = pMesh->GetLayer(0)->GetUVs();
			FbxStringList fbxSL;
			//pMesh->GetUVSetNames(fbxSL);
			//for (int nc = 0; nc < fbxSL.GetCount(); nc++) {
			//	printf(fbxSL.GetStringAt(nc));
			//}
			int index = pMesh->GetTextureUVIndex(ncPolygon, ncVertex);
			vert1.TexCoord = glm::vec4((float)texCoords->mDirectArray->GetAt(index)[0], (float)texCoords->GetDirectArray().GetAt(index)[1], 1.0f, 0.0f);
		}
	}

	for (int nc = 0; nc < pMesh->GetPolygonVertexCount(); nc++)
	{
		g_pMeshManager->indices[indexOffset + nc] = pMesh->GetPolygonVertices()[nc];
	}

	//g_pMeshManager->meshFaces.push_back(tempVecsTriangleFace);
	g_pMeshManager->m_MapMeshNameTocMeshEntry[pMeshNode->GetName()] = meshEntry;

	sBoundingBox boundingBox;
	boundingBox.scale = (glm::vec3(9.0f, 12.5f, 7.0f)); // NOTE: This is not the boxes half-widths. 
														//															 This is the actual scale factor that is applied to the cube primitive. 
														//glm::vec3(max.x - min.x, (max.y - min.y), max.z - min.z);
														//boundingBox.position = glm::vec3(0.0f);
	g_pMeshManager->m_MapMeshNameToAABB["Skeleton"] = boundingBox;

	return true;
}

void cAnimatedMesh::draw(glm::mat4 transform)
{
	glm::mat4 projectionMatrix;
	glm::mat4 viewMatrix;
	gCamera->getProjectionMatrix(projectionMatrix);
	gCamera->getViewMatrix(viewMatrix);
	glUniformMatrix4fv(gUniformId_PojectionMatrix, 1, GL_FALSE,
		glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(gUniformId_ViewMatrix, 1, GL_FALSE,
		glm::value_ptr(viewMatrix));
	glm::vec4 eye4;
	gCamera->getEyePosition(eye4);
	glUniform4fv(gUniformId_EyePosition, 1, glm::value_ptr(eye4));
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_BLEND);
	// glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);

	glCullFace(GL_BACK); // GL_FRONT, GL_BACK, or GL_FRONT_AND_BACK
	glEnable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, // GL_FRONT_AND_BACK is the only thing
									 // you can pass here
		GL_FILL);          // GL_POINT, GL_LINE, or GL_FILL


	glUniform1i(gUniformId_Toggle_NormalAndSpecularMaps, true);
	//glUniform1i(gUniformId_Toggle_Lights, true);
	glBindTexture(GL_TEXTURE_2D, gUniformId_Texture0);

	glUniformMatrix4fv(
		gUniformId_ModelMatrix, 1, GL_FALSE,
		glm::value_ptr(glm::scale(transform, glm::vec3(1.0f))));
	glUniformMatrix4fv(gUniformId_ModelMatrixOrientation, 1, GL_FALSE,
		glm::value_ptr(glm::mat4()));
	glUniform4fv(gUniformId_ModelColor, 1,
		glm::value_ptr(glm::vec4(1.0f)));
	glUniform1f(gUniformId_Alpha, 1.0f);
	
	glUniform1i(gUniformId_Toggle_NormalAndSpecularMaps, true);
	//glUniform1i(gUniformId_Toggle_Lights, true);
	glBindTexture(GL_TEXTURE_2D, gUniformId_Texture0);

}

