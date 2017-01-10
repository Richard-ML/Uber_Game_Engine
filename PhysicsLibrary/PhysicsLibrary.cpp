// PhysicsLibrary.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "cPhysicsLibrary.h"
namespace PhysicsLibrary
{
	cPhysicsLibrary *cPhysicsLibrary::s_cPhysicsLibrary =
		0; // Allocating pointer to static instance of cPhysicsLibrary (singleton)
}