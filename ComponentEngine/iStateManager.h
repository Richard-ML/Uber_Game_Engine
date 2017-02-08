#pragma once
#include "stdafx.h"
#include <memory>
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

  // Anything can subscribe to a state.. A iStateNode will be created on their
  // behave.
  // A pointer to its context will be stored in the State Manager.. It will be
  // used to
  // When another external user changes the state via core interface all
  // subscribed iStateNodes will be
  // Notified via their core interface.. This way their local copy of state will
  // be updated when their specif thread is
  // ready..
  virtual iState *subscribe(std::string stateNodeID) = 0;
  // NOTE: The stateNodeID is sent to each engine with load data (xml,json) per
  // (ENTITY) from GameEngine!

  // TODO: Remove states etc..
};

// CORE INTERFACES EACH ENGINE WILL USE

// Change state data (used by core state manager to dispatch updates to each
// engine which implements the same interface!
class iGeomerty {
public:
  virtual void setPosition(const glm::vec3 position) = 0;
  virtual void setMass(const float mass) = 0;
  virtual void setScale(const float scale) = 0;
  virtual void setTransform(const glm::mat4 transform) = 0;
};

// Change of behaviour states etc.. (like angry) (dead) (static non-moving)
class iBehaviour {
public:
  virtual void setIsColliding(bool isColliding) = 0; //
  virtual void
  setIsMoving(bool isMoving) = 0; // Would be used by both physics and animation
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
};

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

// Core state interface: Parent to state nodes, stores a proxy of the latest
// data from all other interfaces! :D
class iStateNode : public iGeomerty, iBehaviour, iControl {
public:
	// Inherited via iGeomerty
	virtual void setPosition(const glm::vec3 position) = 0;
	virtual void setMass(const float mass) = 0;
	virtual void setScale(const float scale) = 0;
	virtual void setTransform(const glm::mat4 transform) = 0;

	// Inherited via iBehaviour
	virtual void setIsColliding(bool isColliding) = 0;
	virtual void setIsMoving(bool isMoving) = 0;
};

class cState : public iState, iStateNodeHandle {
private:
	////////////////////////////////////////////////////////////////////////////
  friend class cStateNode; // Set of methods for stateNode to use that will not
                           // result in an endless circular loop..
  friend class cStateManager; // Make it easier to set m_parentNode Inherited
                              // via iStateNodeHandle

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
	void doTasks() {
		//http://stackoverflow.com/questions/7548480/how-do-i-create-a-packaged-task-with-parameters
		//https://github.com/jakaspeh/concurrency/blob/master/packagedTask.cpp
		//while (!m_task_queue.empty())
		//{
		//	 m_task_queue.front().get_future().get();
		//	 m_task_queue.pop();
		//}
	}


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

class cStateNode : public iStateNode {
private:
  friend class cState;
  friend class cStateManager;
  // https://msdn.microsoft.com/en-us/library/dd492418.aspx  Could be used to send updates in parallel
  std::vector<iState *> m_childStates;
  std::string uniqueID;
  sState _localStateData;

public:
  // TODO: Could use a function wrapper? Or template?
  // Inherited via iStateNode
  virtual void setPosition(const glm::vec3 position) override {
    this->_localStateData.position = position;
	for each(iState *
		state in m_childStates)
	{
		
		cState* curState = dynamic_cast<cState *>(state);
		curState->_setPosition(position);
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
	for each(iState *
		state in m_childStates)
	{
		cState* curState = dynamic_cast<cState *>(state);
		curState->_setTransform(transform);
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


};



class cStateManager : public iStateManager {
	friend class cStateNode;

private:
	int m_nextID;
	std::vector<iStateNode *> m_childStates;
	std::map<std::string, iStateNode *> m_MapIDTOStateNode;
	
	// Inherited via iStateManager
public:
	virtual std::string registerState() override {
		// TODO: hash id?
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
};

