#pragma once
#include "stdafx.h"
#include "iPhysicsWorld.h"
#include "_btRigidBody.h"
namespace PhysicsEngine {

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Bullet dynamic world wrapper. </summary>
	///
	/// <remarks>	Richard, 4/12/2017. </remarks>
	///-------------------------------------------------------------------------------------------------

	class _btWorld : public iPhysicsWorld {
		friend class _btRigidBody;
	private:// Todo: create getters and setters
	public:
		btDiscreteDynamicsWorld* m_btWorld = 0;
		btDefaultCollisionConfiguration* m_collisionConfiguration = 0;
		btCollisionDispatcher* m_dispatcher = 0;
		btBroadphaseInterface* m_overlappingPairCache = 0;
		btSequentialImpulseConstraintSolver* m_solver = 0;
	public:

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Default constructor. </summary>
		///
		/// <remarks>	Richard, 4/16/2017. </remarks>
		///-------------------------------------------------------------------------------------------------

		_btWorld();

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Destructor. </summary>
		///
		/// <remarks>	Richard, 4/16/2017. </remarks>
		///-------------------------------------------------------------------------------------------------

		virtual ~_btWorld();

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Steps the simulation based on the given delta time. </summary>
		///
		/// <remarks>	Richard, 4/12/2017. </remarks>
		///
		/// <param name="deltaTime">	The delta time. </param>
		///-------------------------------------------------------------------------------------------------

		virtual void step(float deltaTime) override;

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Callback, called when collision occurs. </summary>
		///
		/// <remarks>	Richard, 4/12/2017. </remarks>
		///
		/// <param name="cp">		  	[in,out] The cp. </param>
		/// <param name="colObj0Wrap">	The col object 0 wrap. </param>
		/// <param name="partId0">	  	The part identifier 0. </param>
		/// <param name="index0">	  	The index 0. </param>
		/// <param name="colObj1Wrap">	The col object 1 wrap. </param>
		/// <param name="partId1">	  	The first part identifier. </param>
		/// <param name="index1">	  	The first index. </param>
		///
		/// <returns>	True if it succeeds, false if it fails. </returns>
		///-------------------------------------------------------------------------------------------------

		static bool contact_callback(btManifoldPoint& cp, const btCollisionObjectWrapper* colObj0Wrap, int partId0, int index0, const btCollisionObjectWrapper* colObj1Wrap, int partId1, int index1);

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Removes objects colliding with world editor volumes. </summary>
		///
		/// <remarks>	Richard, 4/16/2017. </remarks>
		///
		/// <param name="cp">		  	[in,out] The cp. </param>
		/// <param name="colObj0Wrap">	The col object 0 wrap. </param>
		/// <param name="partId0">	  	The part identifier 0. </param>
		/// <param name="index0">	  	The index 0. </param>
		/// <param name="colObj1Wrap">	The col object 1 wrap. </param>
		/// <param name="partId1">	  	The first part identifier. </param>
		/// <param name="index1">	  	The first index. </param>
		///
		/// <returns>	True if it succeeds, false if it fails. </returns>
		///-------------------------------------------------------------------------------------------------

		static bool remove_callback(btManifoldPoint& cp, const btCollisionObjectWrapper* colObj0Wrap, int partId0, int index0, const btCollisionObjectWrapper* colObj1Wrap, int partId1, int index1);
	};

}