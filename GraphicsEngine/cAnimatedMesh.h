#pragma once
#include "stdafx.h"
#include <fbxsdk.h>
///-------------------------------------------------------------------------------------------------
/// <summary>	A mesh entry. </summary>
/// <todo> This should be in a base mesh class. </todo>
/// <remarks>	Richard, 4/12/2017. </remarks>
///-------------------------------------------------------------------------------------------------
#define PRINT_ANIMATED_MESH_DEBUG_INFO

struct cMeshEntry {
	int NumgIndices;
	int NumgVertices;
	int BaseIndex;
	int BaseVertex;
};
// TODO: Rename to cAnimatedMesh
class cAnimatedMesh {
	FbxScene * m_pScene;

	///-------------------------------------------------------------------------------------------------
	/// <summary>	A Control Point. (Vertex Handle) </summary>
	///
	/// <remarks>	Richard, 4/19/2017. </remarks>
	///-------------------------------------------------------------------------------------------------
	struct sControlPoint {
		float blendingWeight;
		int controlPointIndex;
		FbxAMatrix transform;
		glm::vec4  intialPosition;
	};

	///-------------------------------------------------------------------------------------------------
	/// <summary>	A Cluster of Control Points. (AKA. Joint/Bone) </summary>
	///
	/// <remarks>	Richard, 4/19/2017. </remarks>
	///-------------------------------------------------------------------------------------------------
	struct sCluster {
		std::vector<sControlPoint*> vec_pControlPoints;
		std::string name;
		FbxCluster* pFBXCluster;
		FbxAMatrix transform;
	};


	///-------------------------------------------------------------------------------------------------
	/// <summary>	A skin mesh. </summary>
	///
	/// <remarks>	Richard, 4/19/2017. </remarks>
	///-------------------------------------------------------------------------------------------------
	struct sSkinMeshAnimation {
		std::string animationName;
		std::string meshName; // Friendly name given via XML
		std::string meshEntryKey; // Internal name of mesh used to fetch cMeshEntry
		float duration;
		std::vector<sCluster*> vec_pClusters;
		/// <summary> Current time progression of this animation set. </summary>
		float time;
		/// <summary> Whether this animation plays on repeat or not. </summary>
		bool loop;

		FbxMesh* pFbxMesh;
	};

	std::vector<sSkinMeshAnimation*> m_vec_pSkinMeshAnimations;
public:
	void initializeSceneHandle(FbxScene * pScene);
	void loadPoses();
	void loadMeshes(FbxNode* pNode);
	///-------------------------------------------------------------------------------------------------
	/// <summary>	Process the fbx mesh node. </summary>
	///
	/// <remarks>	Richard, 4/18/2017. </remarks>
	///
	/// <param name="meshNode">	[in,out] If non-null, the mesh node. </param>
	///
	/// <returns>	True if it succeeds, false if it fails. </returns>
	///-------------------------------------------------------------------------------------------------
	bool processFBXMeshNode(FbxNode* pMeshNode);



	///-------------------------------------------------------------------------------------------------
	/// <summary>	Updates the mesh. </summary>
	///
	/// <remarks>	Richard, 4/18/2017. </remarks>
	///
	/// <param name="deltaTime">	The delta time. </param>
	///-------------------------------------------------------------------------------------------------
	void update(float deltaTime);

	void draw(glm::mat4 transform);

};