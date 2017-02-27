#pragma once
#include "stdafx.h"
class cPlayerControlComponent {
public:
	iState* pState;
	void saveToXMLNode(rapidxml::xml_node<>& componentNode) {
		printf("Worked3!");
	}

};