#pragma once
#include "stdafx.h"
// Data that is exchanged between all engines! The iState interface is changed
// to provide access to this data!
struct sState {
	bool isMoving = false;
	glm::vec3 turnVelocity;
	glm::vec3 velocity;
	float mass;
	glm::mat4 transform;
	bool isColliding;
	glm::vec3 position;
	float scale;
	glm::vec3 acceleration;

	// Remove these
	float cooldownDuration;
	glm::vec3 target;

	glm::vec3 impluse;
	sAABB aabb;
};
// Forward declarations
class iStateNode;
class iState;
class iStateNode;
class iBehaviour;
class iControl;
class iGeometry;
class cState;
class cStateNode;
class cStateManager;

class iStateManager {
	// Data-Members Should be in cStateManager of each engine.. (This is the
	// working set of data for each thread) Updates will be dispatched from the
	// ComponentEngine
public:
	virtual std::string registerState() = 0; // Returns ID of state object (USED
											 // ONCE PER ENTITY) each child
											 // component must subscribe to said
											 // state object to access their data

											 // Anything can subscribe to an iStateNode.. An iState will be created for them
											 // The internally uses the parent iStateNode to exchange state information
											 // with peer components
											 // A pointer to its context will be stored in the State Manager.. It will be

											 // TODO: each iState has its own local copy of the sState 
											 // This will be used when the iState enters/exits thread 
											 // -safe mode. (Multi-Threading not yet implemented) 
	virtual iState *subscribe(std::string stateNodeID) = 0;
	// NOTE: An iState* is sent to each component's corresponding
	// engine with load data (xmlNode,json)
	virtual std::string getGameEntityXML(std::string stateNodeID) = 0;
	virtual void clearStateInfo() = 0;
};

//// Core state interface
class iStateNode : public iGeomerty, public iBehaviour, public iControl {
public:
	// Inherited via iGeomerty
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

	// Inherited via iBehaviour
	virtual void setIsColliding(bool isColliding) = 0;
	virtual void setIsMoving(bool isMoving) = 0;

	// Inherited via iControl
};

class cState : public iState {
private:
	iStateNode *m_parentNode;
	//sState m_localStateData;
	bool m_bool_HasXMLCallback = false;
	////////////////////////////////////////////////////////////////////////////
	friend class cStateNode; // Set of methods for stateNode to use that will not
							 // result in an endless circular loop..
	friend class cStateManager; // Make it easier to set m_parentNode Inherited
								// via iStateNodeHandle
								// (rapidxml::xml_node<>*) *getComponentNode (void);
	std::function<std::string()> getComponentNode;
	virtual void registerComponentXMLDataCallback(std::function<std::string()> getComponentNode) {
		m_bool_HasXMLCallback = true;
		this->getComponentNode = getComponentNode;
	}

public:
	// Inherited via iState
	virtual void setPosition(const glm::vec3 position) override {
		m_parentNode->setPosition(position);
	}
	virtual void setMass(const float mass) override {
		m_parentNode->setMass(mass);
	}
	virtual void setScale(const float scale) override {
		m_parentNode->setScale(scale);
	}
	virtual void setTransform(const glm::mat4 transform) override {
		m_parentNode->setTransform(transform);
	}
	virtual void setIsColliding(bool isColliding) override {
		m_parentNode->setIsColliding(isColliding);
	}
	virtual void setIsMoving(bool isMoving) override {
		m_parentNode->setIsMoving(isMoving);
	}

	virtual void getPosition(glm::vec3 &position) override {
		this->m_parentNode->getPosition(position);
	}
	virtual void getMass(float &mass) override {
		this->m_parentNode->getMass(mass);
	}
	virtual void getScale(float &scale) override {
		this->m_parentNode->getScale(scale);
	}
	virtual void getTransform(glm::mat4 &transform) override {
		this->m_parentNode->getTransform(transform);
	}
	virtual glm::vec3 getPosition() {
		return this->m_parentNode->getPosition();
	}
	virtual float getMass() {
		return this->m_parentNode->getMass();
	}
	virtual float getScale() {
		return this->m_parentNode->getScale();
	}
	virtual glm::mat4 getTransform() {
		return this->m_parentNode->getTransform();
	}

	virtual void setAcceleration(glm::vec3 acceleration) {
		return this->m_parentNode->setAcceleration(acceleration);
	};
	virtual void setVelocity(glm::vec3 velocity) {
		return this->m_parentNode->setVelocity(velocity);
	};

	virtual glm::vec3 getAcceleration() {
		return this->m_parentNode->getAcceleration();
	};
	virtual glm::vec3 getVelocity() {
		return this->m_parentNode->getVelocity();
	};

	virtual void setAABB(sAABB aabb) {
		this->m_parentNode->setAABB(aabb);
	}
	virtual sAABB getAABB() {
		return this->m_parentNode->getAABB();
	}

	virtual bool getIsColliding() {
		return m_parentNode->getIsColliding();
	}
	virtual bool getIsMoving() {
		return m_parentNode->getIsMoving();
	}

	virtual void setImpluse(glm::vec3 impluse) {
		m_parentNode->setImpluse(impluse);

	}
	virtual glm::vec3 getImpluse() {
		return m_parentNode->getImpluse();
	}

	cState() {

	}

};
// if defined, will not do any locking on shared data use this is  
//#define SKIP_LOCKING  


// A cStateNode contains a collection of iStates (interface used in base objects in all engines).
class cStateNode : public iStateNode {
private:
	friend class cState;
	friend class cStateManager;
	// https://msdn.microsoft.com/en-us/library/dd492418.aspx  Could be used to send updates in parallel
	std::vector<iState *> m_childStates;
	std::string uniqueID;
	sState _localStateData;

	//NOTE: Position shares lock with transform since both variables must be updated
	struct sSpinLock {
		//volatile LONG isLocked = 0; // 0 unlocked : 1 locked
		LOCK lock;
	}m_lock[14]; // 10 locks. One should be used per variable
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
public:

	// Inherited via iStateNode
	virtual void setPosition(const glm::vec3 position) override {
		lock(0);
		this->_localStateData.position = position;
		this->_localStateData.transform[3] = glm::vec4(position, this->_localStateData.transform[3].w);
		unlock(0);
	}
	virtual void setMass(const float mass) override {
		lock(1);
		this->_localStateData.mass = mass;
		unlock(1);
	}
	virtual void setScale(const float scale) override {
		lock(2);
		this->_localStateData.scale = scale;
		unlock(2);
	}
	virtual void setTransform(const glm::mat4 transform) override {
		lock(0);
		this->_localStateData.transform = transform;
		this->_localStateData.position = glm::vec3(transform[3]);
		unlock(0);
	}
	virtual void setIsColliding(bool isColliding) override {
		lock(3);
		this->_localStateData.isColliding = isColliding;
		unlock(3);
	}
	virtual void setIsMoving(bool isMoving) override {
		lock(4);
		this->_localStateData.isMoving = isMoving;
		unlock(4);
	}
	// NOTE: iState* only know about the parent state node internally. These getters are 
	// not directly accessible from within the other engines (besides GameEngine) but is not
	// used there anyways..
	// Used to copy latest data from the state node to new objects that subscribe to a state. 
	virtual void getPosition(glm::vec3 &position) override { lock(0); position = _localStateData.position; unlock(0); }
	virtual void getMass(float &mass) override { lock(1); mass = _localStateData.mass; unlock(1); }
	virtual void getScale(float &scale) override { lock(2); scale = _localStateData.scale; unlock(2); }
	virtual void getTransform(glm::mat4 &transform) override { lock(0); transform = _localStateData.transform; unlock(0); }

	virtual glm::vec3 getPosition() { glm::vec3 position; lock(0); position = _localStateData.position; unlock(0); return position; }
	virtual float getMass() { float mass; lock(1); mass = _localStateData.mass; unlock(1); return mass; }
	virtual float getScale() { float scale; lock(2);  scale = _localStateData.scale; unlock(2); return scale; }
	virtual glm::mat4 getTransform() { glm::mat4 transform; lock(0);  transform = _localStateData.transform; unlock(0); return transform; }

	virtual void setAcceleration(glm::vec3 acceleration)
	{
		lock(11);
		_localStateData.acceleration = acceleration;
		unlock(11);
	}
	virtual void setVelocity(glm::vec3 velocity)
	{
		lock(7);
		_localStateData.velocity = velocity;
		unlock(7);
	}

	virtual glm::vec3 getAcceleration() {
		lock(11);
		glm::vec3 accelerationOut = _localStateData.acceleration;
		unlock(11);
		return accelerationOut;
	};
	virtual glm::vec3 getVelocity() {
		lock(7);
		glm::vec3 velocityOut = _localStateData.velocity;
		unlock(7);
		return velocityOut;
	};
	virtual void setAABB(sAABB aabb) {
		lock(12);
		_localStateData.aabb = aabb;
		unlock(12);
	}

	virtual sAABB getAABB() {
		sAABB aabb;
		lock(12);
		aabb = _localStateData.aabb;
		unlock(12);
		return aabb;
	}
	virtual bool getIsColliding() {
		bool isColliding;
		lock(3);
		isColliding = this->_localStateData.isColliding;
		unlock(3);
		return isColliding;
	}
	virtual bool getIsMoving() {
		bool isMoving;
		lock(4);
		isMoving = this->_localStateData.isMoving;
		unlock(4);
		return isMoving;
	}

	virtual void setImpluse(glm::vec3 impluse) {
	
		lock(13);
		this->_localStateData.impluse = impluse;
		unlock(13);
	}
	virtual glm::vec3 getImpluse() {
		lock(13);
		glm::vec3 impluseResult = this->_localStateData.impluse;
		unlock(13);
		return impluseResult;
	}
	cStateNode() {

	}
};

// This class holds a collection of stateNodes which keep a collection of iStates in sync
// When a change is made in an iState via setter.. The iStateNode is notified and
// sends the new data to all of the peer iStates of said iStateNode
class cStateManager : public iStateManager {
	friend class cStateNode;

private:
	int m_nextID;
	std::vector<iStateNode *> m_childStates;
	std::map<std::string, iStateNode *> m_MapIDTOStateNode;


	// Inherited via iStateManager
public:
	virtual std::string registerState() override {
		// TODO: Hash ID
		std::string uID = std::to_string(m_nextID);
		iStateNode *stateNode = new cStateNode();
		dynamic_cast<cStateNode *>(stateNode)->uniqueID = uID;
		m_childStates.push_back(stateNode);
		m_MapIDTOStateNode[uID] = stateNode;
		m_nextID++;
		return uID;
	}
	virtual iState *subscribe(std::string stateNodeID) override {
		iStateNode *stateNode = m_MapIDTOStateNode[stateNodeID];
		iState *state = new cState();
		cState *stateHandle = dynamic_cast<cState *>(state);
		cStateNode *stateNodeHandle = dynamic_cast<cStateNode *>(stateNode);
		stateNodeHandle->m_childStates.push_back(state);
		stateHandle->m_parentNode = stateNode;
		// Copy the version of state data from the cStateNode to the new State..
		//stateHandle->m_localStateData = stateNodeHandle->_localStateData;
		return state;
	}

	virtual std::string getGameEntityXML(std::string stateNodeID) {
		cStateNode *stateNode = dynamic_cast<cStateNode *>(m_MapIDTOStateNode[stateNodeID]);
		// XML Game entity with all of its components. 
		std::string xmlStringResult = "<GameEntity>";
		std::string requestedXMLString;

		for each(iState* state in stateNode->m_childStates)
		{
			cState * pState = dynamic_cast<cState *>(state);
			if (pState->m_bool_HasXMLCallback)
				requestedXMLString += pState->getComponentNode();

		}
		xmlStringResult += requestedXMLString;
		xmlStringResult += "</GameEntity>";
		return xmlStringResult;
	};
	virtual void clearStateInfo() {
		m_childStates.clear();
		m_MapIDTOStateNode.clear();
	};
	cStateManager() {};
};