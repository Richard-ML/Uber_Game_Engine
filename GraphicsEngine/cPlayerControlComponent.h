#pragma once
#include "stdafx.h"

///-------------------------------------------------------------------------------------------------
/// <summary>	A player control component. </summary>
///
/// <remarks>	Richard, 4/12/2017. </remarks>
///-------------------------------------------------------------------------------------------------

class cPlayerControlComponent {
public:
	// The state.
	iState* pState;

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Saves to XML node. </summary>
	///
	/// <remarks>	Richard, 4/12/2017. </remarks>
	///
	/// <param name="xmlString">	[in,out] The XML string. </param>
	///-------------------------------------------------------------------------------------------------

	void saveToXMLNode(std::string& xmlString) {
		xmlString += "<PlayerControlComponent />";
	}

};