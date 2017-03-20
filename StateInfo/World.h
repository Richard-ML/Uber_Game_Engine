#pragma once
#include "stdafx.h"
#include "EnumerationConstants.h"
class iWorldEditor {
public:
	virtual void selectTileAtPosition(glm::vec3 position) = 0;
	virtual void deselectTileAtPosition(glm::vec3 position) = 0;
	virtual void clearSelection() = 0;
	virtual void setActiveSelectionMode(eActiveWorldTileSelection tileSelection) = 0;
	virtual eActiveWorldTileSelection getActiveSelectionMode() = 0;
	virtual std::vector<sAABB> getSelectionAABBs() = 0;
	virtual void generateObjectAtSlection() = 0;
};

class iWorld : public iWorldEditor {
public:
	// Inherited via iWorldEditor
	virtual void selectTileAtPosition(glm::vec3 position) = 0;
	virtual void deselectTileAtPosition(glm::vec3 position) = 0;
	virtual void clearSelection() = 0;
	virtual void setActiveSelectionMode(eActiveWorldTileSelection tileSelection) = 0;
	virtual eActiveWorldTileSelection getActiveSelectionMode() = 0;
	virtual std::vector<sAABB> getSelectionAABBs() = 0;
	virtual void generateObjectAtSlection() = 0;
};

