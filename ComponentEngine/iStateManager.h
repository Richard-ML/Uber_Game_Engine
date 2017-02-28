#pragma once
#include "stdafx.h"
#include <memory>
// Data that is exchanged between all engines! The iState interface is changed
// to provide access to this data!
struct sState {
  bool isMoving;
  glm::vec3 turnVelocity;
  glm::vec3 velocity;
  float mass;
  glm::mat4 transform;
  bool isColliding;
  glm::vec3 position;
  float scale;
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
  virtual std::string getGameEntityXML(std::string stateNodeID) =0;
};


// A way for the state manager to update the sState of each *iState
// When any iState changes a value via setter.
// This handle does not make a call back to the iStateNode
// Like the regular iState setters. (prevents endless loop)
class iStateNodeHandle {
private:
  // Inherited via iState
  virtual void _setPosition(const glm::vec3 position) = 0;
  virtual void _setMass(const float mass) = 0;
  virtual void _setScale(const float scale) = 0;
  virtual void _setTransform(const glm::mat4 transform) = 0;
  virtual void _setIsColliding(bool isColliding) = 0;
  virtual void _setIsMoving(bool isMoving) = 0;
};

//// Core state interface: Parent to state nodes, stores a proxy of the latest
//// data from all other interfaces! :D
class iStateNode : public iGeomerty, iBehaviour, iControl {
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

class cState : public iState, iStateNodeHandle {
private:
	////////////////////////////////////////////////////////////////////////////
  friend class cStateNode; // Set of methods for stateNode to use that will not
                           // result in an endless circular loop..
  friend class cStateManager; // Make it easier to set m_parentNode Inherited
                              // via iStateNodeHandle
 // (rapidxml::xml_node<>*) *getComponentNode (void);
  std::function<std::string()> getComponentNode;
  virtual void _setPosition(const glm::vec3 position) {
    this->m_localStateData.position = position;
        };
	virtual void _setMass(const float mass) {
		this->m_localStateData.mass = mass;
	};
	virtual void _setScale(const float scale) {
		this->m_localStateData.scale = scale;
	};
	virtual void _setTransform(const glm::mat4 transform) {
		this->m_localStateData.transform = transform;
	};
	virtual void _setIsColliding(bool isColliding) {
		this->m_localStateData.isColliding = isColliding;
	};
	virtual void _setIsMoving(bool isMoving) {
		this->m_localStateData.isMoving = isMoving;
	};
	virtual void registerComponentXMLDataCallback(std::function<std::string()> getComponentNode) {
		this->getComponentNode = getComponentNode;
	}
	/////////////////////////////////////////////////////////////////////////////
	// Ignore this.. (Nothing to see here!)
	//std::queue<std::packaged_task<void()>> m_task_queue;
	//
	//template<class Function, class ...Args>
	//std::future<typename std::result_of<Function(Args...)>::type>
	//	dispatch(Function &&f, Args &&...args) {
	//	std::packaged_task<typename std::result_of<Function(Args...)>::type()> task(
	//		std::bind(f, args...));
	//	auto res = task.get_future();
	//	m_task_queue.push(std::packaged_task<void()>(std::move(task)));
	//	return res;
	//}
	////////////////////////////////////////////////////////////////////////////
	iStateNode *m_parentNode;
	sState m_localStateData;

public:
	// Inherited via iState
	virtual void setPosition(const glm::vec3 position) override {
		// TODO: FIRE AND FORGET!!
		// std::thread(m_parentNode->setPosition, position).detach();
		m_parentNode->setPosition(position);
		this->m_localStateData.position = position;
	}
	virtual void setMass(const float mass) override {
		m_parentNode->setMass(mass);
		this->m_localStateData.mass = mass;
	}
	virtual void setScale(const float scale) override {
		m_parentNode->setScale(scale);
		this->m_localStateData.scale = scale;
	}
	virtual void setTransform(const glm::mat4 transform) override {
		m_parentNode->setTransform(transform);
		this->m_localStateData.transform = transform;
	}
	virtual void setIsColliding(bool isColliding) override {
		m_parentNode->setIsColliding(isColliding);
		this->m_localStateData.isColliding = isColliding;
	}
	virtual void setIsMoving(bool isMoving) override {
		m_parentNode->setIsMoving(isMoving);
		this->m_localStateData.isMoving = isMoving;
	}
	// Someday we will have a job pool for each thread. So that threads can 
	// Update values when their thread is ready..
	//void doTasks() {
	//	//http://stackoverflow.com/questions/7548480/how-do-i-create-a-packaged-task-with-parameters
	//	//https://github.com/jakaspeh/concurrency/blob/master/packagedTask.cpp
	//	//while (!m_task_queue.empty())
	//	//{
	//	//	 m_task_queue.front().get_future().get();
	//	//	 m_task_queue.pop();
	//	//}
	//}


	// Alternative set of iState getters with no arguments / parameters that
	// simply returns a copy of whatever the local sState value is!Careful
	// when using these.The value could change externally, and this data
	// would become stale!Use get() whenever you use state data!(Won't
	// really matter if data is stale besides in the physics engine)
	virtual void getPosition( glm::vec3 &position) override { position = m_localStateData.position; }
	virtual void getMass(float &mass) override {	mass = m_localStateData.mass; }
	virtual void getScale(float &scale) override { scale = m_localStateData.scale; }
	virtual void getTransform(glm::mat4 &transform) override { transform = m_localStateData.transform; }
    virtual glm::vec3 getPosition() { return m_localStateData.position; }
    virtual float getMass() { return m_localStateData.mass; }
	virtual float getScale() { return m_localStateData.scale; }
	virtual glm::mat4 getTransform() { return m_localStateData.transform; }
};

// Ignore this.. (Nothing to see here!)
//template <typename F, typename A>
//class cTask {
//	typedef typename std::result_of<F(A)>::type result_type;
//	std::future<result_type> result;
//public:
//	cTask(F &&f, A &&a)
//	{
//		std::packaged_task<result_type(A)> task(f);
//		result = task.get_future();
//	}
//	runTask() {
//		result.get();
//	}
//	
//};

// A cStateNode contains a collection of iStates. It is used to keep all of the iStates that
// are subscribed to said iStateNode synced up
class cStateNode : public iStateNode {
private:
  friend class cState;
  friend class cStateManager;
  // https://msdn.microsoft.com/en-us/library/dd492418.aspx  Could be used to send updates in parallel
  std::vector<iState *> m_childStates;
  std::string uniqueID;
  sState _localStateData;

public:
  // Inherited via iStateNode
  virtual void setPosition(const glm::vec3 position) override {
    this->_localStateData.position = position;
	this->_localStateData.transform[3] = glm::vec4(position, this->_localStateData.transform[3].w);
	for each(iState *
		state in m_childStates)
	{
		cState* curState = dynamic_cast<cState *>(state);
		curState->_setPosition(position);
		curState->_setTransform(this->_localStateData.transform);
	   //g_pGraphicsEngine->dispatch(&curState->_setPosition, position);
	}
  }
  virtual void setMass(const float mass) override {
    this->_localStateData.mass = mass;
	for each(iState *
		state in m_childStates)
	{
		cState* curState = dynamic_cast<cState *>(state);
		curState->_setMass(mass);
		//curState->dispatch(&curState->_setMass, mass);
	}
  }
  virtual void setScale(const float scale) override {
    this->_localStateData.scale = scale;
	for each(iState *
		state in m_childStates)
	{
		cState* curState = dynamic_cast<cState *>(state);
		curState->_setScale(scale);
		//curState->dispatch(*curState->_setScale, scale);
	}
  }
  virtual void setTransform(const glm::mat4 transform) override {
    this->_localStateData.transform = transform;
	this->_localStateData.position = glm::vec3(transform[3]);
	for each(iState *
		state in m_childStates)
	{
		cState* curState = dynamic_cast<cState *>(state);
		curState->_setTransform(transform);
		curState->_setPosition(this->_localStateData.position);
		//curState->dispatch(&curState->_setTransform, transform);
	}
  }
  virtual void setIsColliding(bool isColliding) override {
    this->_localStateData.isColliding = isColliding;
	for each(iState *
		state in m_childStates)
	{
		cState* curState = dynamic_cast<cState *>(state);
		curState->_setIsColliding(isColliding);
		//curState->dispatch(&cState::_setIsColliding , isColliding);
	}
  }
  virtual void setIsMoving(bool isMoving) override {
    this->_localStateData.isMoving = isMoving;
	for each(iState *
		state in m_childStates)
	{
		cState* curState = dynamic_cast<cState *>(state);
		curState->_setIsMoving(isMoving);
		//curState->dispatch(&curState->_setIsMoving, isMoving);
	}
  }
  // NOTE: iState* only know about the parent state node internally. These getters are 
  // not directly accessible from within the other engines (besides GameEngine) but is not
  // used there anyways..
  // Used to copy latest data from the state node to new objects that subscribe to a state. 
  virtual void getPosition(glm::vec3 &position) override { position = _localStateData.position; }
  virtual void getMass(float &mass) override { mass = _localStateData.mass; }
  virtual void getScale(float &scale) override { scale = _localStateData.scale; }
  virtual void getTransform(glm::mat4 &transform) override { transform = _localStateData.transform; }

  virtual glm::vec3 getPosition() { return _localStateData.position; }
  virtual float getMass() { return _localStateData.mass; }
  virtual float getScale() { return _localStateData.scale; }
  virtual glm::mat4 getTransform() { return _localStateData.transform; }

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
		stateHandle->m_localStateData = stateNodeHandle->_localStateData;
		return state;
	}
	virtual std::string getGameEntityXML(std::string stateNodeID) {
		cStateNode *stateNode = dynamic_cast<cStateNode *>(m_MapIDTOStateNode[stateNodeID]);
		int ncTestCounter = 0;
		// XML Game entity with all of its components. 
		std::string xmlStringResult = "<GameEntity>";
		std::string requestedXMLString;
		for each(iState* state in stateNode->m_childStates)
		{
			//xmlStringResult = ncTestCounter; xmlStringResult += "\n";
			requestedXMLString =dynamic_cast<cState *>(state)->getComponentNode();
			printf(requestedXMLString.c_str());
			ncTestCounter++;
		}
		xmlStringResult += requestedXMLString;
		xmlStringResult += "<GameEntity>";
		return xmlStringResult;
	};
	cStateManager() {};
};

