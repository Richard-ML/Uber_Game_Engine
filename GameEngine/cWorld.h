#pragma once
#include "stdafx.h"
#include "World.h"
#include "EnumerationConstants.h"
#include "externals.h"
#include "global.h"
// World editor. Creates bounding boxes to represent tile sections. Can be used to spawn objects at specific locations in the world. More features coming soon! 
class cWorld : public iWorld {
	///-------------------------------------------------------------------------------------------------
	/// <summary>	A spin lock. </summary>
	///
	/// <remarks>	Richard, 4/12/2017. </remarks>
	///-------------------------------------------------------------------------------------------------

	struct sSpinLock {
		//volatile LONG isLocked = 0; // 0 unlocked : 1 locked
		LOCK lock;
	}m_lock[10]; // 10 locks. One should be used per variable

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Locks. </summary>
	///
	/// <remarks>	Richard, 4/12/2017. </remarks>
	///
	/// <param name="varNum">	The variable number. </param>
	///-------------------------------------------------------------------------------------------------

	virtual void lock(int varNum) {
#if !defined(SKIP_LOCKING)  
		while (_InterlockedExchange(&m_lock[varNum].lock, LOCKED) == UNLOCKED) {
			// spin!  
		}
		// At this point, the lock is acquired. ;)
#endif  
	}

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Unlocks. </summary>
	///
	/// <remarks>	Richard, 4/12/2017. </remarks>
	///
	/// <param name="varNum">	The variable number. </param>
	///-------------------------------------------------------------------------------------------------

	virtual void unlock(int varNum) {
#if !defined(SKIP_LOCKING)  
		_InterlockedExchange(&m_lock[varNum].lock, UNLOCKED);
#endif  
	}
	///-------------------------------------------------------------------------------------------------
	/// <summary>	A world tile. "Floor tile with 4 adjacent walls" </summary>
	///
	/// <remarks>	Richard, 4/12/2017. </remarks>
	///-------------------------------------------------------------------------------------------------

	struct sWorldTile {
		glm::vec3 position;
		glm::vec3 scale = glm::vec3(64.0f, 0.1f, 64.0f);
	};
	/// <summary>	The tile selection mode. </summary>
	eActiveWorldTileSelection m_tileSelectionMode;

	/// <summary>	The vector selected tiles. </summary>
	std::vector<sWorldTile> m_vec_SelectedTiles;
	/// <summary>	The world tiles[ 64][64]. </summary>
	sWorldTile m_worldTiles[64][64];
public:
	/// <summary>	NOTE: This is public, but only GameEngine uses it! :P. </summary>
	std::function<void()> generateObjectsAtSelectedTiles;

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Deselect tile at given position. </summary>
	///
	/// <remarks>	Richard, 4/12/2017. </remarks>
	///
	/// <param name="position">	The position. </param>
	///-------------------------------------------------------------------------------------------------

	virtual void deselectTileAtPosition(glm::vec3 position) {
		sWorldTile worldTile;
		worldTile.position = glm::vec3((glm::round(position.x / worldTile.scale.x) * 64.0f) + 32.0f, 0.0f, glm::round(glm::round(position.z / worldTile.scale.z) * 64.0f) + 8.0f);
		lock(0);
		for (unsigned int nc = 0; nc < m_vec_SelectedTiles.size(); nc++)
			if (worldTile.position == m_vec_SelectedTiles.at(nc).position)
			{
				m_vec_SelectedTiles.erase(m_vec_SelectedTiles.begin() + nc);
				break;
			}

		unlock(0);
	}

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Select tile at position. </summary>
	///
	/// <remarks>	Richard, 4/12/2017. </remarks>
	///
	/// <param name="position">	The position. </param>
	///-------------------------------------------------------------------------------------------------

	virtual void selectTileAtPosition(glm::vec3 position) {
		sWorldTile worldTile;
		worldTile.position = glm::vec3((glm::round(position.x / worldTile.scale.x) * 64.0f) + 32.0f, 0.0f, glm::round(glm::round(position.z / worldTile.scale.z) * 64.0f) + 8.0f);
		lock(0);
		for each(sWorldTile existingWorldTile in m_vec_SelectedTiles)
			if (worldTile.position == existingWorldTile.position)
				goto SKIP_ADD_TILE_ALREADY_SELECTED;
		m_vec_SelectedTiles.push_back(worldTile);
	SKIP_ADD_TILE_ALREADY_SELECTED:
		unlock(0);
	}

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Clears the selection. </summary>
	///
	/// <remarks>	Richard, 4/12/2017. </remarks>
	///-------------------------------------------------------------------------------------------------

	virtual void clearSelection() {
		lock(0);
		m_vec_SelectedTiles.clear();
		unlock(0);
	}

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Sets active selection mode. </summary>
	///
	/// <remarks>	Richard, 4/12/2017. </remarks>
	///
	/// <param name="tileSelection">	The tile selection. </param>
	///-------------------------------------------------------------------------------------------------

	virtual void setActiveSelectionMode(eActiveWorldTileSelection tileSelection) {
		lock(0);
		m_tileSelectionMode = tileSelection;
		unlock(0);
	}

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Gets active selection mode. </summary>
	///
	/// <remarks>	Richard, 4/12/2017. </remarks>
	///
	/// <returns>	The active selection mode. </returns>
	///-------------------------------------------------------------------------------------------------

	virtual eActiveWorldTileSelection getActiveSelectionMode() {
		eActiveWorldTileSelection tileSelection;
		lock(0);
		tileSelection = m_tileSelectionMode;
		unlock(0);
		return tileSelection;
	}

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Gets selection bounding boxes </summary>
	///
	/// <remarks>	Richard, 4/12/2017. </remarks>
	///
	/// <returns>	The selection bounding boxes. </returns>
	///-------------------------------------------------------------------------------------------------

	virtual std::vector<sBoundingBox> getSelectionAABBs() {
		std::vector<sBoundingBox> vec_Result;
		lock(0);
		switch (m_tileSelectionMode)
		{
		case FLOOR:
			for each(sWorldTile worldTile in m_vec_SelectedTiles)
			{
				sBoundingBox boundingBox;
				boundingBox.position.x = worldTile.position.x - (worldTile.scale.x / 2);
				boundingBox.position.z = worldTile.position.z - (worldTile.scale.z / 2);
				boundingBox.scale = worldTile.scale;
				vec_Result.push_back(boundingBox);
			}
			break;
		case WALL_LEFT:
			for each(sWorldTile worldTile in m_vec_SelectedTiles)
			{
				sBoundingBox boundingBox;
				boundingBox.position.x = worldTile.position.x - (worldTile.scale.x / 2) + 32.0f;
				boundingBox.position.z = worldTile.position.z - (worldTile.scale.z / 2);
				boundingBox.position.y = 16.0f;
				boundingBox.scale = glm::vec3(6.0f, 32.0f, 64.0f);
				vec_Result.push_back(boundingBox);
			}
			break;
		case WALL_RIGHT:
			for each(sWorldTile worldTile in m_vec_SelectedTiles)
			{
				sBoundingBox boundingBox;
				boundingBox.position.x = worldTile.position.x - (worldTile.scale.x / 2) - 32.0f;
				boundingBox.position.z = worldTile.position.z - (worldTile.scale.z / 2);
				boundingBox.position.y = 16.0f;
				boundingBox.scale = glm::vec3(6.0f, 32.0f, 64.0f);
				vec_Result.push_back(boundingBox);
			}
			break;
		case WALL_FORWARD:
			for each(sWorldTile worldTile in m_vec_SelectedTiles)
			{
				sBoundingBox boundingBox;
				boundingBox.position.x = worldTile.position.x - (worldTile.scale.x / 2);
				boundingBox.position.z = worldTile.position.z - (worldTile.scale.z / 2) + 32.0f;
				boundingBox.position.y = 16.0f;
				boundingBox.scale = glm::vec3(64.0f, 32.0f, 6.0f);
				vec_Result.push_back(boundingBox);
			}
			break;
		case WALL_BACKWARD:
			for each(sWorldTile worldTile in m_vec_SelectedTiles)
			{
				sBoundingBox boundingBox;
				boundingBox.position.x = worldTile.position.x - (worldTile.scale.x / 2);
				boundingBox.position.z = worldTile.position.z - (worldTile.scale.z / 2) - 32.0f;
				boundingBox.position.y = 16.0f;
				boundingBox.scale = glm::vec3(64.0f, 32.0f, 6.0f);
				vec_Result.push_back(boundingBox);
			}
			break;
		default:
			printf("ERROR: Invalid selection mode.\n");
			break;
		}

		unlock(0);
		return vec_Result;
	}

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Generates an object at selection. </summary>
	///
	/// <remarks>	Richard, 4/12/2017. </remarks>
	///-------------------------------------------------------------------------------------------------
	// TODO: FIX TYPO
	virtual void generateObjectAtSlection() {
		generateObjectsAtSelectedTiles(); // Magic
	}

	PhysicsEngine::cPhysicsEngine* pPhysicsEngine = 0;
	virtual bool generateConvexHull(std::string meshName, std::vector<sTriangleFace*> triangleFaces) {
		pPhysicsEngine->generateConvexHull(meshName, triangleFaces);
		return true;
	}


	virtual bool generatePhysicsMesh(std::string meshName, unsigned int * indices, sMeshVertex * vertices, int numVertices, int numIndices) {
		std::cout << "Num verts " << numVertices << std::endl;
		return pPhysicsEngine->generatePhysicsMesh(meshName, indices, vertices, numVertices, numIndices);
	}
};

