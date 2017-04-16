#ifndef _cPhysicsEngine_HG_
#define _cPhysicsEngine_HG_
#include "stdafx.h"
#include <Windows.h>
#include "iRigidBody.h"
#include <string>
#include <time.h>
#ifdef PhysicsEngine_EXPORTS
#define PhysicsEngine_API __declspec(dllexport)
#else
#define PhysicsEngine_API __declspec(dllimport)
#endif // PhysicsEngine_EXPORTS

namespace PhysicsEngine {
	class cPhysicsEngine_Impl; // Purpose: Encapsulate private member variables and
							   // internal routines. Reduces make-time,
							   // compile-time, and the Fragile Binary Interface
							   // Problem.

	class cPhysicsEngine {
		friend class _btRigidBody;
		/// <summary>	The physics engine. </summary>
		static cPhysicsEngine *s_cPhysicsEngine;
		// Boilerplate
		friend class cPhysicsEngine_Impl; // The PIMPL idiom aka Compilation Firewall
										  //NOTE: Position shares lock with transform since both variables must be updated
		//struct sSpinLock {
		//	//volatile LONG isLocked = 0; // 0 unlocked : 1 locked
		//	LOCK lock;
		//};
		//
		//// Lock used when an rigid-body is being created from another thread.
		//// vec_rigidbodies must be locked to prevent incompatible iterators within the Physics thread.
	    //sSpinLock * g_pLock;
		///-------------------------------------------------------------------------------------------------
		/// <summary>	Gets the implementation. </summary>
		///
		/// <remarks>	Richard, 4/12/2017. </remarks>
		///
		/// <returns>	Null if it fails, else a pointer to a const cPhysicsEngine_Impl. </returns>
		///-------------------------------------------------------------------------------------------------

		const cPhysicsEngine_Impl *impl() const;

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Gets the implementation. </summary>
		///
		/// <remarks>	Richard, 4/12/2017. </remarks>
		///
		/// <returns>	Null if it fails, else a pointer to a cPhysicsEngine_Impl. </returns>
		///-------------------------------------------------------------------------------------------------

		cPhysicsEngine_Impl *impl();

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Default constructor. </summary>
		///
		/// <remarks>	Richard, 4/12/2017. </remarks>
		///-------------------------------------------------------------------------------------------------

		cPhysicsEngine(); // Constructor is private therefore a new instance can not be made
						  // externally. Only available to members or friends of this class..

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Destructor. </summary>
		///
		/// <remarks>	Richard, 4/12/2017. </remarks>
		///-------------------------------------------------------------------------------------------------

		~cPhysicsEngine() {}
		// Not defined to prevent copying of the only instance of the class.
		cPhysicsEngine(const cPhysicsEngine &) {}; // Disallow copy constructor
		cPhysicsEngine &operator=(const cPhysicsEngine &physicsManager) {
		} // Disallow assignment operator
		static DWORD __cdecl physicsThread(void* lpParam);
	

	public:
		//int loadGameFromXML(std::string filename);
	//	void gLock(int varNum);
	//	void gUnlock(int varNum);
		///-------------------------------------------------------------------------------------------------
		/// <summary>	Gets the instance. </summary>
		///
		/// <remarks>	Richard, 4/12/2017. </remarks>
		///
		/// <returns>	Null if it fails, else a pointer to a cPhysicsEngine. </returns>
		///-------------------------------------------------------------------------------------------------

		static PhysicsEngine_API  cPhysicsEngine *instance();

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Loads physics component. </summary>
		///
		/// <remarks>	Richard, 4/12/2017. </remarks>
		///
		/// <param name="componentNode">	[in,out] If non-null, the component node. </param>
		/// <param name="state">			[in,out] If non-null, the state. </param>
		///
		/// <returns>	True if it succeeds, false if it fails. </returns>
		///-------------------------------------------------------------------------------------------------

		static PhysicsEngine_API bool loadPhysicsComponent(rapidxml::xml_node<> *componentNode, iState* state);

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Initializes the game state handle. </summary>
		///
		/// <remarks>	Richard, 4/12/2017. </remarks>
		///
		/// <param name="pGameState">	[in,out] If non-null, state of the game. </param>
		///-------------------------------------------------------------------------------------------------

		PhysicsEngine_API void initializeGameStateHandle(iGameState* pGameState);

	    ///-------------------------------------------------------------------------------------------------
	    /// <summary>	Initializes the world handle. </summary>
	    ///
	    /// <remarks>	Richard, 4/12/2017. </remarks>
	    ///
	    /// <param name="pWorld">	[in,out] If non-null, the world. </param>
	    ///-------------------------------------------------------------------------------------------------

	    PhysicsEngine_API void initializeWorldHandle(iWorld* pWorld);

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Initializes the debug renderer handle. </summary>
		///
		/// <remarks>	Richard, 4/12/2017. </remarks>
		///
		/// <param name="pDebugRenderer">	[in,out] If non-null, the debug renderer. </param>
		///-------------------------------------------------------------------------------------------------

		PhysicsEngine_API void initializeDebugRendererHandle(iDebugRenderer* pDebugRenderer);

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Adds the physics object to 'state'. </summary>
		///
		/// <remarks>	Richard, 4/12/2017. </remarks>
		///
		/// <param name="position">	The position. </param>
		/// <param name="state">   	[in,out] If non-null, the state. </param>
		///
		/// <returns>	True if it succeeds, false if it fails. </returns>
		///-------------------------------------------------------------------------------------------------

		static PhysicsEngine_API bool addPhysicsObject(glm::vec3 position, iState* state);

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Generates a convex hull. </summary>
		///
		/// <remarks>	Richard, 4/15/2017. </remarks>
		///
		/// <param name="meshName">			Name of the mesh. </param>
		/// <param name="triangleFaces">	[in,out] If non-null, the triangle faces. </param>
		///
		/// <returns>	True if it succeeds, false if it fails. </returns>
		///-------------------------------------------------------------------------------------------------

		PhysicsEngine_API bool generateConvexHull(std::string meshName, std::vector<sTriangleFace*> triangleFaces);

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Generates a physics mesh. </summary>
		///
		/// <remarks>	Richard, 4/15/2017. </remarks>
		///
		/// <param name="meshName">   	Name of the mesh. </param>
		/// <param name="indices">	  	[in,out] If non-null, the indices. </param>
		/// <param name="vertices">   	[in,out] If non-null, the vertices. </param>
		/// <param name="numVertices">	Number of vertices. </param>
		/// <param name="numIndices"> 	Number of indices. </param>
		///
		/// <returns>	True if it succeeds, false if it fails. </returns>
		///-------------------------------------------------------------------------------------------------

		PhysicsEngine_API bool generatePhysicsMesh(std::string meshName, unsigned int * indices, sMeshVertex * vertices, const unsigned int numVertices, const unsigned int numIndices);

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Clears the game objects. </summary>
		///
		/// <remarks>	Richard, 4/15/2017. </remarks>
		///-------------------------------------------------------------------------------------------------

		PhysicsEngine_API void clearGameObjects();



		PhysicsEngine_API void cleanup();
	};

}
#endif