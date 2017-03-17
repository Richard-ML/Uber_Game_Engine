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
		static GraphicsEngine_API  cGraphicsEngine *instance();
		static GraphicsEngine_API  void loadCubemaps(rapidxml::xml_node<> *cubemapsNode);
		static GraphicsEngine_API void loadMipmapTextures(rapidxml::xml_node<>* mipmapTexturesNode);
		static GraphicsEngine_API void loadTextures(rapidxml::xml_node<>* texturesNode);
		static GraphicsEngine_API bool loadRenderableComponent(rapidxml::xml_node<> *componentNode, iState* state);
		static GraphicsEngine_API bool loadMeshes(rapidxml::xml_node<> *meshesNode);
		static GraphicsEngine_API bool loadFramebufferObjects(rapidxml::xml_node<>* framebuffersNode);
		GraphicsEngine_API void update(float deltaTime);
		GraphicsEngine_API void initializeGameStateHandle(iGameState* pGameState);
		//GraphicsEngine_API void initializeWorldEditorHandle(iWorldEditor* pWorldEditor);
		//GraphicsEngine_API void initializeDebugRendererHandle(iDebugRenderer* pDebugRenderer);

	};

}
#endif