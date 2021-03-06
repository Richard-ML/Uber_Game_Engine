#pragma once
#include "stdafx.h"
class cRenderManager;

///-------------------------------------------------------------------------------------------------
/// <summary>	A debug renderer. </summary>
///
/// <remarks>	Richard, 4/12/2017. </remarks>
///-------------------------------------------------------------------------------------------------

class cDebugRenderer : public iDebugRenderer {
private:
	friend class cRenderManager;
	enum { SHAPE_SPHERE, SHAPE_CUBE };
	struct sDebugShape {
		glm::vec3 position;
		float duration;
		glm::vec3 scale;
		glm::vec3 color;
	};
	std::vector<sDebugShape> m_vec_debugShapes;
	struct sSpinLock {
		LOCK lock;
	}m_lock[1]; // 1 lock(s). One should be used per variable

	virtual void lock(int varNum) {

#if !defined(SKIP_LOCKING)  
		while (_InterlockedCompareExchange(&m_lock[varNum].lock, LOCKED, UNLOCKED) == UNLOCKED) {
			// spin!  
		}
		// At this point, the lock is acquired. ;)
#endif  
	}

	virtual void unlock(int varNum) {
#if !defined(SKIP_LOCKING)  
		_InterlockedCompareExchange(&m_lock[varNum].lock, UNLOCKED, LOCKED);
#endif  
	}

public:

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Creates a cube. </summary>
	///
	/// <remarks>	Richard, 4/12/2017. </remarks>
	///
	/// <param name="position">	The position. </param>
	/// <param name="scale">   	The scale. </param>
	/// <param name="duration">	The duration. </param>
	/// <param name="color">   	The color. </param>
	///-------------------------------------------------------------------------------------------------

	virtual void createCube(glm::vec3 position, glm::vec3 scale, float duration, glm::vec3 color) {
		sDebugShape debugShape;
		debugShape.duration = duration;
		debugShape.position = position;
		debugShape.scale = scale;
		debugShape.color = color;
		lock(0);
		m_vec_debugShapes.push_back(debugShape);
		unlock(0);
	}
};