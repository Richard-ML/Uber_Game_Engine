#pragma once
#include "stdafx.h"

struct sState {
	bool isMoving;
	glm::vec3 turnVelocity;
	glm::vec3 velocity;
	float mass;
	glm::mat4 transform;
	bool collision;
	glm::vec3 position;
};

class iStateManager {
	// Data-Members Should be in cStateManager of each engine.. (This is the working set of data for each thread) Updates will be dispatched from the ComponentEngine
private:


	virtual int registerState(sState initialState) = 0; // Returns ID of state object (USED ONCE PER ENTITY) each child component must subscribe to said state object to access their data

														// Anything can subscribe to a state.. A iStateNode will be created on their behave. 
														// A pointer to its context will be stored in the State Manager.. It will be used to 
														// When another external user changes the state via core interface all subscribed iStateNodes will be
														// Notified via their core interface.. This way their local copy of state will be updated when their specif thread is 
														// ready..
	//virtual iStateNode* subscribe(int stateID) = 0;


	// TODO: Remove states etc.. 
};

class iStateNodeManager {
private:
	//sState m_masterState;
	//std::vector<iStateNode*> m_childStates; // 


};

// Core state interface: Parent to state nodes, stores a proxy of the latest data from all other interfaces! :D
class iStateNode {
private:
	//sState m_state; // Local copy of state.. This will be updated by the state manager..

};




// CORE INTERFACES EACH ENGINE WILL USE

// Change state data (used by core state manager to dispatch updates to each engine which implements the same interface! 
class iGeomerty {
	virtual void setPosition(const glm::vec3 position) = 0;
	virtual void setMass(const float position) = 0;
	virtual void setScale(const float scale) = 0;
	virtual void setScale(const glm::vec3 position) = 0;
	virtual void setTransform(const glm::mat4 position) = 0;

};

// Change of behaviour states etc.. (like angry) (dead) (static non-moving)
class iBehaviour {
	virtual void setIsColliding(bool isColliding) = 0; //  
	virtual void setIsMoving(bool isMoving) = 0; // Would be used by both physics and animation
};

// Apply velocity increment orientation etc.. Used by control components that use the inputManager for callbacks based on specific input to move around entities.
// Also used by the AI engine to move around entities using internal logic
class iControl {
	// adjust("Turn"/"")Velocity(glm::vec3) etc..
};