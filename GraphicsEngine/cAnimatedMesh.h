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

class cAnimatedMesh {
	FbxScene * m_pScene;
	std::string meshName;
	std::vector<cMeshEntry *> m_vec_pMeshEntries;
	FbxArray<FbxPose*> m_vec_pPoses;
	struct sAnimationStack {
		// Name of this animation set
		std::string name;
		// Current time progression of this animation set
		float time;
		// Whether this animation plays on repeat or not
		bool loop;
	};

public:
	void initializeSceneHandle(FbxScene * pScene);
	void loadPoses();
	void loadMeshes();

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