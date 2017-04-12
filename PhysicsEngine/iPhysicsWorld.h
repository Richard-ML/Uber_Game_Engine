#ifndef _iPhysicsWorld_HG_
#define _iPhysicsWorld_HG_
#include "stdafx.h"

#ifdef PhysicsEngine_EXPORTS
#define PhysicsEngine_API __declspec(dllexport)
#else
#define PhysicsEngine_API __declspec(dllimport)
#endif // PhysicsEngine_EXPORTS
namespace PhysicsEngine {
	// Wrapper interface for the bullet's dynamic world
	class iPhysicsWorld {
	public:

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Steps the simulation based on time passed. </summary>
		///
		/// <remarks>	Richard, 4/12/2017. </remarks>
		///
		/// <param name="deltaTime">	Time passed. </param>
		///-------------------------------------------------------------------------------------------------

		inline virtual void step(float deltaTime) = 0;
	};

}
#endif