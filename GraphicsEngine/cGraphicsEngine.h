#ifndef _cGraphicsEngine_HG_
#define _cGraphicsEngine_HG_
#include "stdafx.h"

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

	public:
		static GraphicsEngine_API  cGraphicsEngine *instance();
		static GraphicsEngine_API  void loadCubeMap(rapidxml::xml_node<> *cubeNode);
		GraphicsEngine_API void update(float deltaTime);
	};

}
#endif