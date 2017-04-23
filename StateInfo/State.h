#pragma once
#include "stdafx.h"
#include "..\Include\rapidxml\rapidxml.hpp"
#include <functional>   // std::function, std::negate
#include "GameState.h"
//Change state data (used by core state manager to dispatch updates to each
// engine which implements the same interface!
class iGeomerty {
public:
	virtual void setPosition(const glm::vec3 position) = 0;
	virtual void setMass(const float mass) = 0;
	virtual void setScale(const float scale) = 0;
	virtual void setTransform(const glm::mat4 transform) = 0;

	virtual void getPosition(glm::vec3 &position) = 0;
	virtual void getMass(float &mass) = 0;
	virtual void getScale(float &scale) = 0;
	virtual void getTransform(glm::mat4 &transform) = 0;

	virtual glm::vec3 getPosition() = 0;
	virtual float getMass() = 0;
	virtual float getScale() = 0;
	virtual glm::mat4 getTransform() = 0;

	virtual void setBoundingBox(sBoundingBox boundingBox) = 0;
	virtual sBoundingBox getBoundingBox() = 0;
};

// Change of behaviour states etc.. (like angry) (dead) (static non-moving)
class iBehaviour {
public:
	virtual void setIsColliding(bool isColliding) = 0; //
	virtual void setIsMoving(bool isMoving) = 0; // Would be used by both physics and animation

	virtual void setBehavioralState(eCharacterBehavioralState behavioralState) = 0;
	virtual eCharacterBehavioralState getBehavioralState() = 0;

	virtual bool getIsColliding() = 0;
	virtual bool getIsMoving() = 0;

};

// Used by the AI engine to move entities using internal logic. Used by Graphics Engine to control the player’s character based on user input. 
class iControl {
public:
	// adjust turn velocity, move velocity, jump, crouch, attack, etc... 
	virtual void setAcceleration(glm::vec3 acceleration) = 0;
	virtual void setVelocity(glm::vec3 velocity) = 0;

	virtual void setImpluse(glm::vec3 impluse) = 0;
	virtual glm::vec3 getImpluse() = 0;

	virtual glm::vec3 getAcceleration() = 0;
	virtual glm::vec3 getVelocity() = 0;

};

class iData {
	/* Each engine can provide a function pointer for saving to xml for each of its internal objects.
	* @param getComponentNode is a pointer to a function which returns an xml node containing an objects current information.
	*		  NOTE: Each componentNode is a child of an GameEntity node. Meaning the component can share basic information with other
	*        components.
	*/
	virtual void registerComponentXMLDataCallback(std::function<std::string()> getComponentNode) = 0;
	virtual std::string getMeshName() = 0;
	virtual void setMeshName(std::string meshName) = 0;
	virtual void setPendingDeletion(bool pendingDeletion) = 0;
	virtual bool getPendingDeletion() = 0;

};

// See cState for implementation!
class iState : public iGeomerty, public iBehaviour, public iControl, public iData {
public:
	// Inherited via iGeomerty
	virtual void setPosition(const glm::vec3 position) = 0;
	virtual void setMass(const float mass) = 0;
	virtual void setScale(const float scale) = 0;
	virtual void setTransform(const glm::mat4 transform) = 0;

	// Inherited via iBehaviour
	virtual void setIsColliding(bool isColliding) = 0;
	virtual void setIsMoving(bool isMoving) = 0;

	virtual void registerComponentXMLDataCallback(std::function<std::string()> getComponentNode) = 0;

	virtual void setImpluse(glm::vec3 impluse) = 0;
	virtual glm::vec3 getImpluse() = 0;

	virtual void setAcceleration(glm::vec3 acceleration) = 0;
	virtual void setVelocity(glm::vec3 velocity) = 0;

	virtual glm::vec3 getAcceleration() = 0;
	virtual glm::vec3 getVelocity() = 0;

	virtual void setBoundingBox(sBoundingBox boundingBox) = 0;
	virtual sBoundingBox getBoundingBox() = 0;

	virtual bool getIsColliding() = 0;
	virtual bool getIsMoving() = 0;

	virtual std::string getMeshName() = 0;
	virtual void setMeshName(std::string meshName) = 0;

	virtual void setPendingDeletion(bool pendingDeletion) = 0;
	virtual bool getPendingDeletion() = 0;
	
	virtual void setBehavioralState(eCharacterBehavioralState behavioralState) = 0;
	virtual eCharacterBehavioralState getBehavioralState() = 0;
};
