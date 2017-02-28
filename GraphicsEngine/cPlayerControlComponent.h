#pragma once
#include "stdafx.h"
class cPlayerControlComponent {
public:
	iState* pState;
	void saveToXMLNode(std::string& xmlString) {
		xmlString += "<PlayerControlComponent />";
	}

};