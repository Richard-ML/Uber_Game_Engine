#ifndef _cGraphicsEngine_HG_
#define _cGraphicsEngine_HG_
#include "global.h"
#include "stdafx.h"
#ifdef GraphicsEngine_EXPORTS
#define GraphicsEngine_API __declspec(dllexport)
#else
#define GraphicsEngine_API __declspec(dllimport)
#endif // GraphicsEngine_EXPORTS
namespace GraphicsEngine {
	class  cGraphicsEngine_Impl; // Purpose: Encapsulate private member variables and
							   // internal routines. Reduces make-time,
							   // compile-time, and the Fragile Binary Interface
							   // Problem.

	class GraphicsEngine_API cGraphicsEngine {
		static cGraphicsEngine *s_cGraphicsEngine;
		// Boilerplate
		friend class cGraphicsEngine_Impl; // The PIMPL idiom aka Compilation Firewall
		const cGraphicsEngine_Impl *impl() const;
		cGraphicsEngine_Impl *impl();

	public:
		
		static  cGraphicsEngine *instance();
		static  GLFWwindow * cGraphicsEngine::createWindow(); // Currently does not work. Because of memory access violation..

		// Managers..
		static  cMeshManager * s_pMeshManager;
		static  cShaderManager * s_pShaderManager;
		static  cTextureManager * s_pTextureManager;

	private:
		cGraphicsEngine() {
		} // Constructor is private therefore a new instance can not be made
		  // externally. Only available to members or friends of this class..
		~cGraphicsEngine() {}
		// Not defined to prevent copying of the only instance of the class.
		cGraphicsEngine(const cGraphicsEngine &); // Disallow copy constructor
		cGraphicsEngine &operator=(const cGraphicsEngine &GraphicsManager) {
		} // Disallow assignment operator
	};

}
#endif