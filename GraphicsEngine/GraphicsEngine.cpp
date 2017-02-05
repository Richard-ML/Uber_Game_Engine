// GraphicsEngine.cpp : Defines the entry point for the console application.
//


// The graphics engine will have to be the entry point of the application! (Non-DLL) 

#include "stdafx.h"


int main()
{

	// Create instance of GameEngine............ Then do whatever else needs to be done to setup environment
	// Game Engine will contain a vector of game entities which it creates as it loads an XML or JSON document.. 
	// Each XMLNODE will be a component for instance.. And will be processed in the ComponentEngine.. Each entity will have a state created for it using the StateManager inside of the ComponentEngine
	// Components are simply building blocks now.. They no longer contain anything besides an ID.. They are simply used to initialize entities in the scene.

	// TODO: HERE

	// 1 : Create GLFW WINDOW..
	// 2 : Setup shader(s) and draw the Skybox..
	// 3 : Load Meshes..
	// 4 : Have an interface to draw said mesh at simple position.. glm::mat4 whatever..
	// I will integrate the the component engine so that we can load meshes and have their state/location modified externally 


    return 0;
}

