#pragma once
#include "stdafx.h"
#include "global.h"
#include "iState.h"
class cGraphicsObject {
public:

 	iState* pState;
	std::string meshName;
	// Whatever other internal graphics stuff you want!
};