#pragma once
#include "stdafx.h"
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

// See cState for implementation!
class iState : public iGeomerty, iBehaviour, iControl {
public:
	// Inherited via iGeomerty
	virtual void setPosition(const glm::vec3 position) = 0;
	virtual void setMass(const float mass) = 0;
	virtual void setScale(const float scale) = 0;
	virtual void setTransform(const glm::mat4 transform) = 0;

	// Inherited via iBehaviour
	virtual void setIsColliding(bool isColliding) = 0;
	virtual void setIsMoving(bool isMoving) = 0;

	// TODO Getters! Don't forget to update iStateManager!!
};
