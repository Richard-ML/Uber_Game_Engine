#pragma once
#include "stdafx.h"
#include "EnumerationConstants.h"
// Used by all Engines to render debug info

///-------------------------------------------------------------------------------------------------
/// <summary>	A debug renderer. </summary>
///
/// <remarks>	Richard, 4/12/2017. </remarks>
///-------------------------------------------------------------------------------------------------

class iDebugRenderer {
public:
	//spawn primitives with specific lifetimes, etc... 

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

	virtual void createCube(glm::vec3 position, glm::vec3 scale, float duration, glm::vec3 color) = 0;
};
