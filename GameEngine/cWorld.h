#pragma once
#include "stdafx.h"
#include "World.h"
#include "EnumerationConstants.h"
// World editor. Creates bounding boxes to represent tile sections. Can be used to spawn objects at specific locations in the world. More features coming soon! 
class cWorld : public iWorld {
	struct sSpinLock {
		//volatile LONG isLocked = 0; // 0 unlocked : 1 locked
		LOCK lock;
	}m_lock[10]; // 10 locks. One should be used per variable
	virtual void lock(int varNum) {
#if !defined(SKIP_LOCKING)  
		while (_InterlockedExchange(&m_lock[varNum].lock, LOCKED) == UNLOCKED) {
			// spin!  
		}
		// At this point, the lock is acquired. ;)
#endif  
	}

	virtual void unlock(int varNum) {
#if !defined(SKIP_LOCKING)  
		_InterlockedExchange(&m_lock[varNum].lock, UNLOCKED);
#endif  
	}
	// Floor tile with 4 adjacent walls
	struct sWorldTile {
		glm::vec3 position;
		glm::vec3 scale = glm::vec3(64.0f, 0.1f, 64.0f);
	};
	eActiveWorldTileSelection m_tileSelectionMode;

	std::vector<sWorldTile> m_vec_SelectedTiles;
	sWorldTile m_worldTiles[64][64];
public:
	std::function<void()> generateObjectsAtSelectedTiles; // NOTE: This is public, but only GameEngine can see it! :P
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
	virtual void clearSelection() {
		lock(0);
		m_vec_SelectedTiles.clear();
		unlock(0);
	}
	virtual void setActiveSelectionMode(eActiveWorldTileSelection tileSelection) {
		lock(0);
		m_tileSelectionMode = tileSelection;
		unlock(0);
	}
	virtual eActiveWorldTileSelection getActiveSelectionMode() {
		eActiveWorldTileSelection tileSelection;
		lock(0);
		tileSelection = m_tileSelectionMode;
		unlock(0);
		return tileSelection;
	}
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
	virtual void generateObjectAtSlection() {
		generateObjectsAtSelectedTiles(); // Magic
	}
};
