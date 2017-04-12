#ifndef _cGraphicsEngine_HG_
#define _cGraphicsEngine_HG_
#include "stdafx.h"
#include "cGraphicsObject.h"
#ifdef GRAPHICSENGINE_EXPORTS
#define GraphicsEngine_API __declspec(dllexport)
#else
#define GraphicsEngine_API __declspec(dllimport)
#endif // GraphicsEngine_EXPORTS

namespace GraphicsEngine {
	class cGraphicsEngine_Impl; // Purpose: Encapsulate private member variables and
						  // internal routines. Reduces make-time,
						  // compile-time, and the Fragile Binary Interface
						  // Problem.

	class cGraphicsEngine {
		static cGraphicsEngine *s_cGraphicsEngine;
		// Boilerplate
		friend class cGraphicsEngine_Impl; // The PIMPL idiom aka Compilation Firewall
		const cGraphicsEngine_Impl *impl() const;
		cGraphicsEngine_Impl *impl();
		cGraphicsEngine() {}; // Constructor is private therefore a new instance can not be made
						// externally. Only available to members or friends of this class..
		~cGraphicsEngine() {
			// TODO: Clean up resources..
			glfwTerminate();
		
		
		
		}
		// Not defined to prevent copying of the only instance of the class.
		cGraphicsEngine(const cGraphicsEngine &) {}; // Disallow copy constructor
		cGraphicsEngine &operator=(const cGraphicsEngine &GraphicsManager) {
		} // Disallow assignment operator
		bool buffersInitialized = false;
		
	public:

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Gets the instance. </summary>
		///
		/// <remarks>	Richard, 4/12/2017. </remarks>
		///
		/// <returns>	Null if it fails, else a pointer to a cGraphicsEngine. </returns>
		///-------------------------------------------------------------------------------------------------

		static GraphicsEngine_API  cGraphicsEngine *instance();

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Loads cubemaps. </summary>
		///
		/// <remarks>	Richard, 4/12/2017. </remarks>
		///
		/// <param name="cubemapsNode">	[in,out] If non-null, the cubemaps node. </param>
		///-------------------------------------------------------------------------------------------------

		static GraphicsEngine_API  void loadCubemaps(rapidxml::xml_node<> *cubemapsNode);

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Loads mipmap textures. </summary>
		///
		/// <remarks>	Richard, 4/12/2017. </remarks>
		///
		/// <param name="mipmapTexturesNode">	[in,out] If non-null, the mipmap textures node. </param>
		///-------------------------------------------------------------------------------------------------

		static GraphicsEngine_API void loadMipmapTextures(rapidxml::xml_node<>* mipmapTexturesNode);

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Loads the textures. </summary>
		///
		/// <remarks>	Richard, 4/12/2017. </remarks>
		///
		/// <param name="texturesNode">	[in,out] If non-null, the textures node. </param>
		///-------------------------------------------------------------------------------------------------

		static GraphicsEngine_API void loadTextures(rapidxml::xml_node<>* texturesNode);

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Loads renderable component. </summary>
		///
		/// <remarks>	Richard, 4/12/2017. </remarks>
		///
		/// <param name="componentNode">	[in,out] If non-null, the component node. </param>
		/// <param name="state">			[in,out] If non-null, the state. </param>
		///
		/// <returns>	True if it succeeds, false if it fails. </returns>
		///-------------------------------------------------------------------------------------------------

		static GraphicsEngine_API bool loadRenderableComponent(rapidxml::xml_node<> *componentNode, iState* state);

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Loads the meshes. </summary>
		///
		/// <remarks>	Richard, 4/12/2017. </remarks>
		///
		/// <param name="meshesNode">	[in,out] If non-null, the meshes node. </param>
		///
		/// <returns>	True if it succeeds, false if it fails. </returns>
		///-------------------------------------------------------------------------------------------------

		static GraphicsEngine_API bool loadMeshes(rapidxml::xml_node<> *meshesNode);

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Loads framebuffer objects. </summary>
		///
		/// <remarks>	Richard, 4/12/2017. </remarks>
		///
		/// <param name="framebuffersNode">	[in,out] If non-null, the framebuffers node. </param>
		///
		/// <returns>	True if it succeeds, false if it fails. </returns>
		///-------------------------------------------------------------------------------------------------

		static GraphicsEngine_API bool loadFramebufferObjects(rapidxml::xml_node<>* framebuffersNode);

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Updates based on the given deltaTime. </summary>
		///
		/// <remarks>	Richard, 4/12/2017. </remarks>
		///
		/// <param name="deltaTime">	The delta time. </param>
		///-------------------------------------------------------------------------------------------------

		GraphicsEngine_API void update(float deltaTime);

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Initializes the game state handle. </summary>
		///
		/// <remarks>	Richard, 4/12/2017. </remarks>
		///
		/// <param name="pGameState">	[in,out] If non-null, state of the game. </param>
		///-------------------------------------------------------------------------------------------------

		GraphicsEngine_API void initializeGameStateHandle(iGameState* pGameState);

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Initializes the world handle. </summary>
		///
		/// <remarks>	Richard, 4/12/2017. </remarks>
		///
		/// <param name="pWorld">	[in,out] If non-null, the world. </param>
		///-------------------------------------------------------------------------------------------------

		GraphicsEngine_API void initializeWorldHandle(iWorld* pWorld);

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Gets debug renderer handle. </summary>
		///
		/// <remarks>	Richard, 4/12/2017. </remarks>
		///
		/// <returns>	Null if it fails, else the debug renderer handle. </returns>
		///-------------------------------------------------------------------------------------------------

		GraphicsEngine_API iDebugRenderer* getDebugRendererHandle();

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Clears the game objects. </summary>
		///
		/// <remarks>	Richard, 4/12/2017. </remarks>
		///-------------------------------------------------------------------------------------------------

		static GraphicsEngine_API void clearGameObjects();

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Adds an object with mesh based on 'meshName'. </summary>
		///
		/// <remarks>	Richard, 4/12/2017. </remarks>
		///
		/// <param name="state">   	[in,out] If non-null, the state. </param>
		/// <param name="meshName">	Name of the mesh. </param>
		///-------------------------------------------------------------------------------------------------

		static GraphicsEngine_API void addObject(iState* state, std::string meshName);

	};

}
#endif