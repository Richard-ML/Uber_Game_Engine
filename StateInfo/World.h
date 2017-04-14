#pragma once
#include "stdafx.h"
#include "EnumerationConstants.h"

///-------------------------------------------------------------------------------------------------
/// <summary>	Editor for the world. </summary>
///
/// <remarks>	Richard, 4/12/2017. </remarks>
///-------------------------------------------------------------------------------------------------

class iWorldEditor {
public:
	virtual void selectTileAtPosition(glm::vec3 position) = 0;
	virtual void deselectTileAtPosition(glm::vec3 position) = 0;
	virtual void clearSelection() = 0;
	virtual void setActiveSelectionMode(eActiveWorldTileSelection tileSelection) = 0;
	virtual eActiveWorldTileSelection getActiveSelectionMode() = 0;
	virtual std::vector<sBoundingBox> getSelectionAABBs() = 0;
	virtual void generateObjectAtSlection() = 0;
};

///-------------------------------------------------------------------------------------------------
/// <summary>	A world interface. </summary>
///
/// <remarks>	Richard, 4/12/2017. </remarks>
///-------------------------------------------------------------------------------------------------

class iWorld : public iWorldEditor {
public:
	// Inherited via iWorldEditor
	virtual void selectTileAtPosition(glm::vec3 position) = 0;
	virtual void deselectTileAtPosition(glm::vec3 position) = 0;
	virtual void clearSelection() = 0;
	virtual void setActiveSelectionMode(eActiveWorldTileSelection tileSelection) = 0;
	virtual eActiveWorldTileSelection getActiveSelectionMode() = 0;
	virtual std::vector<sBoundingBox> getSelectionAABBs() = 0;
	virtual void generateObjectAtSlection() = 0;

	virtual bool generateConvexHull(std::string meshName, std::vector<sTriangleFace*> triangleFaces) = 0;
	virtual bool generatePhysicsMesh(std::string meshName, unsigned int * indices, sMeshVertex * vertices, int numVertices, int numIndices) = 0;
};

