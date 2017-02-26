#pragma once
#include "stdafx.h"
#include "iState.h"
class cGraphicsObject {
public:

 	iState* pState;
	std::string meshName;
	bool toggleOutline;
	// Whatever other internal graphics stuff you want!
};