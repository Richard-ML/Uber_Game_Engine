#include "stdafx.h"
#include "cAnimatedMesh.h"
#include "global.h"
void cAnimatedMesh::initializeSceneHandle(FbxScene * pScene)
{
	m_pScene = pScene;
}
void cAnimatedMesh::loadPoses()
{
	/// <summary> Get the amount of poses in this scene. </summary>
	int poseCount = m_pScene->GetPoseCount();
	/// <summary> Add each pose to the vector of poses. </summary>
#ifdef PRINT_ANIMATED_MESH_DEBUG_INFO
	std::stringstream output;
	output << "\n\n<Poses>\n";
#endif // PRINT_ANIMATED_MESH_DEBUG_INFO
	for (int nc = 0; nc < poseCount; ++nc)
	{
		FbxPose* pPose = m_pScene->GetPose(nc);
		/// <summary> Print out some details about this pose in the console. </summary>
		std::string poseName = pPose->GetName();

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

///-------------------------------------------------------------------------------------------------
/// <summary>	Loads the meshes. </summary>
///
/// <remarks>	Richard, 4/19/2017. </remarks>
///
/// <param name="pNode">	[in,out] If non-null, the node. </param>
///-------------------------------------------------------------------------------------------------

void cAnimatedMesh::loadMeshes(FbxNode* pNode)
{
	/// <summary> Recursively parse the node hierarchy and load any meshes. </summary>
	for (int nc = 0; nc < pNode->GetChildCount(); nc++)
	{
		FbxNode* pNodeChild = pNode->GetChild(nc);
		FbxMesh * pMeshNode = pNodeChild->GetMesh();
		if (pMeshNode != 0)
			processFBXMeshNode(pNodeChild);
		
		loadMeshes(pNodeChild);
	}
}

///-------------------------------------------------------------------------------------------------
/// <summary>	Processes the FBX mesh node. </summary>
///
/// <remarks>	Richard, 4/19/2017. </remarks>
///
/// <param name="pMeshNode">	[in,out] If non-null, the mesh node. </param>
///
/// <returns>	True if it succeeds, false if it fails. </returns>
///-------------------------------------------------------------------------------------------------

bool cAnimatedMesh::processFBXMeshNode(FbxNode * pMeshNode)
{
	FbxMesh* pMesh = pMeshNode->GetMesh();
	/// <summary> Create and initialize a new Mesh Entry. </summary>
	cMeshEntry meshEntry;
	meshEntry.BaseIndex = 0;
	meshEntry.BaseVertex = 0;
	meshEntry.NumgIndices = 0;

	/// <summary> Create a new Skin Mesh object. </summary>
	sSkinMeshAnimation* pSkinMeshAnimation = new sSkinMeshAnimation();
	m_vec_pSkinMeshAnimations.push_back(pSkinMeshAnimation);

	pSkinMeshAnimation->pFbxMesh = pMesh;
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

	/// <summary>	Get control points (vertices for a mesh) </summary>
	FbxVector4* fbxControlPoints = pMesh->GetControlPoints();

	/// <summary> Load Vertices </summary>
	for (int ncPolygon = 0; ncPolygon < pMesh->GetPolygonCount(); ncPolygon++) {
		// For each vertex in the polygon ( assume triangles ) 
		for (unsigned ncVertex = 0; ncVertex < 3; ncVertex++) {


			FbxVector4 vert = pMesh->GetControlPointAt((ncPolygon * 3) + ncVertex);
			sMeshVertex &vert1 = g_pMeshManager->vertices[vertexOffset + (ncPolygon * 3) + ncVertex];

			vert1.textureUnits = g_pTextureManager->mapTextureNameToMipmapLevel["Skeleton"];

			FbxVector4 fbxVertex = fbxControlPoints[(ncPolygon * 3) + ncVertex];

			/// <summary> Get vertex position. </summary>
			vert1.Position = glm::vec4((float)fbxVertex.mData[0], (float)fbxVertex.mData[1], (float)fbxVertex.mData[2], 1.f);

			/// <summary> Get the normal </summary>
			FbxVector4 fbxNormal;
			pMesh->GetPolygonVertexNormal(ncPolygon, ncVertex, fbxNormal);
			fbxNormal.Normalize();
			vert1.Normal = glm::vec4((float)fbxNormal.mData[0], (float)fbxNormal.mData[1], (float)fbxNormal.mData[2], (float)fbxNormal.mData[3]);
	
			/// <summary> Get texture coordinates </summary>
			FbxStringList lUVSetNameList;
			pMesh->GetNode()->GetMesh()->GetUVSetNames(lUVSetNameList);
			const char* lUVSetName = lUVSetNameList.GetStringAt(0);
			FbxVector2 lUVValue;
			bool mapped; 


			const FbxGeometryElementUV* lUVElement = pMesh->GetNode()->GetMesh()->GetElementUV(lUVSetName);
			fbxsdk_2015_1::FbxLayerElement::EReferenceMode mode = lUVElement->GetReferenceMode();
			
			lUVValue = lUVElement->GetDirectArray().GetAt(ncVertex);
			vert1.TexCoord = glm::vec4((float)lUVValue.mData[0], (float)lUVValue.mData[1], 1.0f, 1.0f);

#define NO_TEX_COORDS
#ifdef NO_TEX_COORDS
				if (!lUVElement)
					continue;
				// only support mapping mode eByPolygonVertex and eByControlPoint
				if (lUVElement->GetMappingMode() != FbxGeometryElement::eByPolygonVertex &&
					lUVElement->GetMappingMode() != FbxGeometryElement::eByControlPoint)
					continue;
				//index array, where holds the index referenced to the uv data
				const bool lUseIndex = lUVElement->GetReferenceMode() != FbxGeometryElement::eDirect;
				const int lIndexCount = (lUseIndex) ? lUVElement->GetIndexArray().GetCount() : 0;
				//iterating through the data by polygon
				const int lPolyCount = pMesh->GetPolygonCount();
				if (lUVElement->GetMappingMode() == FbxGeometryElement::eByControlPoint)
				{
							FbxVector2 lUVValue;
							//get the index of the current vertex in control points array
							int lPolyVertIndex = pMesh->GetPolygonVertex(ncPolygon, ncVertex);
							//the UV index depends on the reference mode
							int lUVIndex = lUseIndex ? lUVElement->GetIndexArray().GetAt(lPolyVertIndex) : lPolyVertIndex;
							lUVValue = lUVElement->GetDirectArray().GetAt(lUVIndex);
							vert1.TexCoord = glm::vec4((float)lUVValue.mData[0], (float)lUVValue.mData[1], 1.0f, 1.0f);
				}
				else if (lUVElement->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
				{
					int lPolyIndexCounter = 0;
							if (lPolyIndexCounter < lIndexCount)
							{
								FbxVector2 lUVValue;
								//the UV index depends on the reference mode
								int lUVIndex = lUseIndex ? lUVElement->GetIndexArray().GetAt(lPolyIndexCounter) : lPolyIndexCounter;
								lUVValue = lUVElement->GetDirectArray().GetAt(lUVIndex);
								vert1.TexCoord = glm::vec4((float)lUVValue.mData[0], (float)lUVValue.mData[1], 1.0f, 1.0f);
								lPolyIndexCounter++;
					}
				}
#endif
			}
	}

	for (int nc = 0; nc < pMesh->GetPolygonVertexCount(); nc++)
	{
		g_pMeshManager->indices[indexOffset + nc] = pMesh->GetPolygonVertices()[nc];
	}

	//g_pMeshManager->meshFaces.push_back(tempVecsTriangleFace);
	pSkinMeshAnimation->meshEntryKey = pMeshNode->GetName();
	g_pMeshManager->m_MapMeshNameTocMeshEntry[pSkinMeshAnimation->meshEntryKey] = meshEntry;

	// LOAD ANIMATION DATA ---------------------------------------------------BEGIN-----

	/// <summary> Get the first animation stack. </summary>
	FbxAnimStack* pAnimStack = m_pScene->GetSrcObject<FbxAnimStack>(0);
	// set its blend mode to Additive
	//lAnimLayer->BlendMode.Set(FbxAnimLayer::eBlendAdditive);

	/// <summary>  Get the animation's name </summary>
	pSkinMeshAnimation->animationName = pAnimStack->GetName();
	/// <summary>  Get take info using the animation's name </summary>
	FbxTakeInfo* pTakeInfo = m_pScene->GetTakeInfo(pAnimStack->GetName());

	/// <summary> Get the animation's duration. </summary>
	FbxTime start = pTakeInfo->mLocalTimeSpan.GetStart();
	FbxTime end = pTakeInfo->mLocalTimeSpan.GetStop();
	pSkinMeshAnimation->duration = end.GetFrameCount(FbxTime::eFrames24) - start.GetFrameCount(FbxTime::eFrames24) + 1;

	/// <summary> Get the amount of Deformers. </summary>
	int deformerCount = pMesh->GetDeformerCount();
	/// <summary> Iterate through Deformer Nodes to get Skin Cluster information. </summary>
	for (unsigned int ncDeformerIndex = 0; ncDeformerIndex < deformerCount; ++ncDeformerIndex)
	{
		/// <summary> Verify that this Deformer is a Skin. </summary>
		FbxSkin* pSkin = reinterpret_cast<FbxSkin*>(pMesh->GetDeformer(ncDeformerIndex, FbxDeformer::eSkin));

		if (!pSkin)
			continue;

		/// <summary>	 Get the amount of Clusters in this Skin. </summary>
		int clusterCount = pSkin->GetClusterCount();
		pSkinMeshAnimation->vec_pClusters.reserve(clusterCount);
		/// <summary> Process Cluster's Data.  </summary>
		for (unsigned int clusterIndex = 0; clusterIndex < clusterCount; ++clusterIndex)
		{
			/// <summary> Get current Cluster from Skin using index. </summary>
			FbxCluster* pFBXCluster = pSkin->GetCluster(clusterIndex);

			/// <summary>  Create new cluster add it to the vector of clusters for this mesh.  </summary>
			sCluster* pCluster = new sCluster();
			pSkinMeshAnimation->vec_pClusters.push_back(pCluster);

			/// <summary>  Get Joint's name. </summary>
			pCluster->name = pFBXCluster->GetLink()->GetName();

			/// <summary> Add handle to the FBX Cluster </summary>
			pCluster->pFBXCluster = pFBXCluster;

			FbxVector4 translation = pMeshNode->GetGeometricTranslation(FbxNode::eSourcePivot);
			FbxVector4 rotation = pMeshNode->GetGeometricRotation(FbxNode::eSourcePivot);
			FbxVector4 scale = pMeshNode->GetGeometricScaling(FbxNode::eSourcePivot);
			FbxAMatrix geometryTransform(translation, rotation, scale);


			FbxAMatrix transformMatrix;
			FbxAMatrix transformLinkMatrix;

			pFBXCluster->GetTransformMatrix(transformMatrix);
			pFBXCluster->GetTransformLinkMatrix(transformLinkMatrix);
			pCluster->transform = transformLinkMatrix.Inverse() * transformMatrix * geometryTransform;

			/// <summary> Associate each Cluster with its Control Points. </summary>
			int indicesCount = pFBXCluster->GetControlPointIndicesCount();
			pCluster->vec_pControlPoints.reserve(indicesCount);
			for (unsigned int index = 0; index < indicesCount; ++index)//
			{
				float blendingWeight = (float)pFBXCluster->GetControlPointWeights()[index];
				/// <summary> If the blending weight is greater than the minimum </summary>
				if (blendingWeight > 0.1f)
				{
					/// <summary> Create new Control Point. </summary>
					sControlPoint * pControlPoint = new sControlPoint();
					/// <summary>  Add Blending Data to Control Point.  </summary>
					pControlPoint->blendingWeight = blendingWeight;
					/// <summary> Get the index of this FBX control point </summary> 
					pControlPoint->controlPointIndex = pFBXCluster->GetControlPointIndices()[index];

					FbxVector4 fbxVertex = pMesh->GetControlPointAt(pControlPoint->controlPointIndex);
					pControlPoint->transform.SetT(fbxVertex);

					pControlPoint->intialPosition = glm::vec4((float)fbxVertex.mData[0], (float)fbxVertex.mData[1], (float)fbxVertex.mData[2], 1.f);
					/// <summary> Add the Control Point to this Cluster's Control Point array </summary>
					pCluster->vec_pControlPoints.push_back(pControlPoint);
				}
			}
		}
	}
	// LOAD ANIMATION DATA ---------------------------------------------------END-------

	sBoundingBox boundingBox;
	boundingBox.scale = (glm::vec3(9.0f, 12.5f, 7.0f)); // NOTE: HARDCODED bounding box for the skeleton TODO: Use FBX bounding box extents
	g_pMeshManager->m_MapMeshNameToAABB["Skeleton"] = boundingBox;

	return true;
}

///-------------------------------------------------------------------------------------------------
/// <summary>	Updates the mesh's animation based on the given deltaTime. </summary>
///
/// <remarks>	Richard, 4/19/2017. </remarks>
///
/// <param name="deltaTime">	The delta time. </param>
///-------------------------------------------------------------------------------------------------

void cAnimatedMesh::update(float deltaTime, eCharacterBehavioralState behavioralState)
{
	/// <summary> Frame Time. </summary>
	FbxTime currTime;




	for each(sSkinMeshAnimation* pSkinMesh in m_vec_pSkinMeshAnimations)
	{
		float animationBegin = 0.0f;
		float animationEnd = pSkinMesh->duration;
		/// <summary> Update the skin mesh's internal time. </summary>
		pSkinMesh->time += deltaTime * 20.0f;
		//if (pSkinMesh->time >= pSkinMesh->duration)
		//	pSkinMesh->time = 0;


		switch (behavioralState)
		{
		case IDLE:
			animationBegin = 0.0f;
			animationEnd = 100.0f;
			break;
		case WALK:
			animationBegin = 2397.0f;
			animationEnd = 2432.0f;
			break;
		case WALK_BACKWARDS:
			animationBegin = 1337.0f; // 1337 I know.. ;)
			animationEnd = 1372.0f;
			break;
		case RUN:
			animationBegin = 1651.0f;
			animationEnd = 1673.0f;
			break;
		case JUMP:
			// Actual jump..
			animationBegin = 1676.0f;
			animationEnd = 1716.0f;
			// Falling
			animationBegin = 1511.0f;
			animationEnd = 1551.0f;
			break;
		case CROUCH:
			break;
		default:
			animationBegin = 0.0f;
			animationEnd = 100.0f;
			break;
		}
		if (pSkinMesh->time < animationBegin)
			pSkinMesh->time = animationBegin;
		if (pSkinMesh->time >= animationEnd)
			pSkinMesh->time = animationBegin;

		currTime.SetFrame(pSkinMesh->time, FbxTime::eFrames30);
		///  <summary> Offset to first vertex in the vertex buffer. </summary>
		int vertexOffset = g_pMeshManager->m_MapMeshNameTocMeshEntry[pSkinMesh->meshEntryKey].BaseVertex;

		FbxAMatrix currentTransformOffset = pSkinMesh->pFbxMesh->GetNode()->EvaluateGlobalTransform(currTime);

		FbxVector4 translation = pSkinMesh->pFbxMesh->GetNode()->GetGeometricTranslation(FbxNode::eSourcePivot); // Geometric translation
		FbxVector4 rotation = pSkinMesh->pFbxMesh->GetNode()->GetGeometricRotation(FbxNode::eSourcePivot); // Geometric rotation
		FbxVector4 scale = pSkinMesh->pFbxMesh->GetNode()->GetGeometricScaling(FbxNode::eSourcePivot); // Geometric scaling
		FbxAMatrix geometryTransform(translation, rotation, scale);

		if (pSkinMesh->vec_pClusters.size() > 0)
		{
			for each(sCluster* pCluster in pSkinMesh->vec_pClusters)
			{
				FbxAMatrix currentTransform = currentTransformOffset.Inverse() * pCluster->pFBXCluster->GetLink()->EvaluateGlobalTransform(currTime);
				pCluster->transform = currentTransform * geometryTransform;

				FbxAMatrix clusterInitPos;

				/// <summary>	The cluster mode. All the links must have the same link mode. </summary>
				FbxCluster::ELinkMode clusterMode = ((FbxSkin*)pSkinMesh->pFbxMesh->GetDeformer(0, FbxDeformer::eSkin))->GetCluster(0)->GetLinkMode();

				FbxAMatrix referenceGlobalInitPosition;
				FbxAMatrix referenceGlobalCurrentPosition;
				FbxAMatrix associateGlobalInitPosition;
				FbxAMatrix associateGlobalCurrentPosition;
				FbxAMatrix clusterGlobalInitPosition;
				FbxAMatrix clusterGlobalCurrentPosition;
				FbxAMatrix clusterGeometry;
				FbxAMatrix clusterRelativeInitPosition;
				FbxAMatrix clusterRelativeCurrentPositionInverse;
				FbxAMatrix pVertexTransformMatrix;
				if (clusterMode == FbxCluster::eAdditive && pCluster->pFBXCluster->GetAssociateModel())
				{
					pCluster->pFBXCluster->GetTransformAssociateModelMatrix(associateGlobalInitPosition);

					FbxAMatrix lAssociateGeometry(translation, rotation, scale);
					associateGlobalInitPosition *= lAssociateGeometry;
					associateGlobalCurrentPosition = pCluster->pFBXCluster->GetAssociateModel()->EvaluateGlobalTransform(currTime);
					pCluster->pFBXCluster->GetTransformMatrix(referenceGlobalInitPosition);
					referenceGlobalInitPosition *= geometryTransform;
					pCluster->pFBXCluster->GetTransformLinkMatrix(clusterGlobalInitPosition);

					FbxAMatrix 	clusterGeometry(
						pCluster->pFBXCluster->GetLink()->GetGeometricTranslation(FbxNode::eSourcePivot),
						pCluster->pFBXCluster->GetLink()->GetGeometricRotation(FbxNode::eSourcePivot),
						pCluster->pFBXCluster->GetLink()->GetGeometricScaling(FbxNode::eSourcePivot)

					);
					clusterGlobalInitPosition *= clusterGeometry;

					clusterGlobalCurrentPosition = pCluster->pFBXCluster->GetLink()->EvaluateGlobalTransform(currTime);
					pVertexTransformMatrix = referenceGlobalInitPosition.Inverse() * associateGlobalInitPosition * associateGlobalCurrentPosition.Inverse() *
						clusterGlobalCurrentPosition * clusterGlobalInitPosition.Inverse() * referenceGlobalInitPosition;
				}
				else
				{
					pCluster->pFBXCluster->GetTransformMatrix(referenceGlobalInitPosition);
					referenceGlobalInitPosition *= geometryTransform;
					/// <summary> Get the link initial global position and the link current global position. </summary>
					pCluster->pFBXCluster->GetTransformLinkMatrix(clusterGlobalInitPosition);
					clusterGlobalCurrentPosition = pCluster->pFBXCluster->GetLink()->EvaluateGlobalTransform(currTime);
					/// <summary> Compute the initial position of the link relative to the reference. </summary>
					clusterRelativeInitPosition = clusterGlobalInitPosition.Inverse() * referenceGlobalInitPosition;
					/// <summary> Compute the current position of the link relative to the reference. </summary>
					clusterRelativeCurrentPositionInverse = referenceGlobalCurrentPosition.Inverse() * clusterGlobalCurrentPosition;
					/// <summary> Compute the shift of the link relative to the reference. </summary>
					pVertexTransformMatrix = clusterRelativeCurrentPositionInverse * clusterRelativeInitPosition;
				}
				/// <summary> Apply the Cluster's transform to all of its Control Points </summary>
				for each(sControlPoint* pControlPoint in pCluster->vec_pControlPoints)
				{
						FbxVector4 fbxVertex = pVertexTransformMatrix.MultT(pControlPoint->transform.GetT());
						g_pMeshManager->vertices[vertexOffset + pControlPoint->controlPointIndex].Position = glm::vec4((float)fbxVertex.mData[0], (float)fbxVertex.mData[1], (float)fbxVertex.mData[2], 1.f);

				}
			}
		}
		int bytesInVertexArray = g_pMeshManager->vertices.size() * sizeof(sMeshVertex);
		glBufferData(GL_ARRAY_BUFFER, bytesInVertexArray, &g_pMeshManager->vertices[0],
			GL_STATIC_DRAW);
	}
}

///-------------------------------------------------------------------------------------------------
/// <summary>	Draws the meshes inside of a cAnimatedMesh object. </summary>
///
/// <remarks>	Richard, 4/19/2017. </remarks>
///
/// <param name="transform">	The world transform. </param>
///-------------------------------------------------------------------------------------------------

void cAnimatedMesh::draw(glm::mat4 transform)
{
	for each(sSkinMeshAnimation* pSkinMesh in m_vec_pSkinMeshAnimations)
	{

		if (pSkinMesh->vec_pClusters.size() == 0)
		{
			// This does not currently work xD
			glm::mat4 tempTransform = transform;
			FbxAMatrix baseFbxTransform = pSkinMesh->pFbxMesh->GetNode()->EvaluateGlobalTransform(pSkinMesh->time);
			FbxVector4 offset = baseFbxTransform.GetT();
			tempTransform[3] += glm::vec4((float)offset.mData[0], (float)offset.mData[1], (float)offset.mData[2], 0.f);
			glUniformMatrix4fv(
				gUniformId_ModelMatrix, 1, GL_FALSE,
				glm::value_ptr(tempTransform));

			glDrawElementsBaseVertex(
				GL_TRIANGLES, g_pMeshManager->m_MapMeshNameTocMeshEntry[pSkinMesh->meshEntryKey].NumgIndices, GL_UNSIGNED_INT,
				(void *)(sizeof(unsigned int) *  g_pMeshManager->m_MapMeshNameTocMeshEntry[pSkinMesh->meshEntryKey].BaseIndex),
				g_pMeshManager->m_MapMeshNameTocMeshEntry[pSkinMesh->meshEntryKey].BaseIndex);

			glUniformMatrix4fv(
				gUniformId_ModelMatrix, 1, GL_FALSE,
				glm::value_ptr(transform));
		}
		else {
			glDrawElementsBaseVertex(
				GL_TRIANGLES, g_pMeshManager->m_MapMeshNameTocMeshEntry[pSkinMesh->meshEntryKey].NumgIndices, GL_UNSIGNED_INT,
				(void *)(sizeof(unsigned int) *  g_pMeshManager->m_MapMeshNameTocMeshEntry[pSkinMesh->meshEntryKey].BaseIndex),
				g_pMeshManager->m_MapMeshNameTocMeshEntry[pSkinMesh->meshEntryKey].BaseIndex);
		}
	}

}

