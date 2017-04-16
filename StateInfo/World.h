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

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Select tile at position. </summary>
	///
	/// <remarks>	Richard, 4/15/2017. </remarks>
	///
	/// <param name="position">	The position. </param>
	///-------------------------------------------------------------------------------------------------

	virtual void selectTileAtPosition(glm::vec3 position) = 0;

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Deselect tile at position. </summary>
	///
	/// <remarks>	Richard, 4/15/2017. </remarks>
	///
	/// <param name="position">	The position. </param>
	///-------------------------------------------------------------------------------------------------

	virtual void deselectTileAtPosition(glm::vec3 position) = 0;

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Clears the selection. </summary>
	///
	/// <remarks>	Richard, 4/15/2017. </remarks>
	///-------------------------------------------------------------------------------------------------

	virtual void clearSelection() = 0;

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Sets active selection mode. </summary>
	///
	/// <remarks>	Richard, 4/15/2017. </remarks>
	///
	/// <param name="tileSelection">	The tile selection mode enum. </param>
	///-------------------------------------------------------------------------------------------------

	virtual void setActiveSelectionMode(eActiveWorldTileSelection tileSelection) = 0;

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Gets active selection mode. </summary>
	///
	/// <remarks>	Richard, 4/15/2017. </remarks>
	///
	/// <returns>	The active selection mode enum. </returns>
	///-------------------------------------------------------------------------------------------------

	virtual eActiveWorldTileSelection getActiveSelectionMode() = 0;

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Gets selection bounding boxes. </summary>
	///
	/// <remarks>	Richard, 4/15/2017. </remarks>
	///
	/// <returns>	The selection bounding boxes. </returns>
	///-------------------------------------------------------------------------------------------------

	virtual std::vector<sBoundingBox> getSelectionAABBs() = 0;

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Generates objects at selection. </summary>
	///
	/// <remarks>	Richard, 4/15/2017. </remarks>
	///-------------------------------------------------------------------------------------------------

	virtual void generateObjectAtSlection() = 0;

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Generates a convex hull. </summary>
	///
	/// <remarks>	Richard, 4/15/2017. </remarks>
	///
	/// <param name="meshName">			Name of the mesh. </param>
	/// <param name="triangleFaces">	[in,out] If non-null, the triangle faces. </param>
	///
	/// <returns>	True if it succeeds, false if it fails. </returns>
	///-------------------------------------------------------------------------------------------------

	virtual bool generateConvexHull(std::string meshName, std::vector<sTriangleFace*> triangleFaces) = 0;

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Generates the physics mesh. </summary>
	///
	/// <remarks>	Richard, 4/15/2017. </remarks>
	///
	/// <param name="meshName">   	Name of the mesh. </param>
	/// <param name="indices">	  	[in,out] If non-null, the indices. </param>
	/// <param name="vertices">   	[in,out] If non-null, the vertices. </param>
	/// <param name="numVertices">	Number of vertices. </param>
	/// <param name="numIndices"> 	Number of indices. </param>
	///
	/// <returns>	True if it succeeds, false if it fails. </returns>
	///-------------------------------------------------------------------------------------------------

	virtual bool generatePhysicsMesh(std::string meshName, unsigned int* indices, sMeshVertex* vertices, int numVertices, int numIndices) = 0;


	virtual bool removeObjectsAtSelection() = 0;
};

