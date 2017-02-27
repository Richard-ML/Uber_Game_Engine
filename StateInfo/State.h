#pragma once
#include "stdafx.h"
#include "..\Include\rapidxml\rapidxml.hpp"
//Change state data (used by core state manager to dispatch updates to each
// engine which implements the same interface!
class iGeomerty {
public:
	virtual void setPosition(const glm::vec3 position) = 0;
	virtual void setMass(const float mass) = 0;
	virtual void setScale(const float scale) = 0;
	virtual void setTransform(const glm::mat4 transform) = 0;

	virtual void getPosition( glm::vec3 &position) = 0;
	virtual void getMass( float &mass) = 0;
	virtual void getScale( float &scale) = 0;
	virtual void getTransform( glm::mat4 &transform) = 0;

	virtual glm::vec3 getPosition() = 0;
	virtual float getMass() = 0;
	virtual float getScale() = 0;
	virtual glm::mat4 getTransform() = 0;
};

// Change of behaviour states etc.. (like angry) (dead) (static non-moving)
class iBehaviour {
public:
	virtual void setIsColliding(bool isColliding) = 0; //
	virtual void
		setIsMoving(bool isMoving) = 0; // Would be used by both physics and animation
	// TODO: Getters
};

// Apply velocity increment orientation etc.. Used by control components that
// use the inputManager for callbacks based on specific input to move around
// entities.
// Also used by the AI engine to move around entities using internal logic
class iControl {
	// adjust("Turn"/"")Velocity(glm::vec3) etc..
};

class iData {
	/* Each engine can provide a function pointer for saving to xml for each of its internal objects. 
	 * @param getComponentNode is a pointer to a function which returns an xml node containing an objects current information.
	 *		  NOTE: Each componentNode is a child of an GameEntity node. Meaning the component can share basic information with other
	 *        components.
	 */
	virtual void registerComponentXMLDataCallback(rapidxml::xml_node<> getComponentNode(void*)) = 0;
	// Callback used to retrieve a component's information in the form of an XML node. More efficient than the initial load at startup process that I am about to show you.. 
	// WHICH WE DO NOT NEED TO CHANGE!! SO DON'T WORRY ABOUT THIS PART THAT I AM ABOUT TO SHOW YOU. . .
	
	// All we need to worry about is writing a callback function (method inside of (physics/graphics) object classes which returns its information as an XML NODE..
	// Once we do this.. I will add a method to the GameEngine (saveGame() which calls rapidxml::xmlnode<>* getGameEntityInfo(id); for each game entity.. SAVE GAME COMPLETE
	// Then write a function loadGame(int difficulty) that will delete all of the GameEntities then redo the initial loading of the game entities.

};

// See cState for implementation!
class iState : public iGeomerty, iBehaviour, iControl, iData {
public:
	// Inherited via iGeomerty
	virtual void setPosition(const glm::vec3 position) = 0;
	virtual void setMass(const float mass) = 0;
	virtual void setScale(const float scale) = 0;
	virtual void setTransform(const glm::mat4 transform) = 0;

	// Inherited via iBehaviour
	virtual void setIsColliding(bool isColliding) = 0;
	virtual void setIsMoving(bool isMoving) = 0;

	virtual void registerComponentXMLDataCallback(rapidxml::xml_node<> getComponentNode(void*)) = 0;
	// TODO Getters! Don't forget to update iStateManager!!
};
