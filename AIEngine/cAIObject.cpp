#include "stdafx.h"
#include "cAIObject.h"
#include <ctime>
#include <chrono>
#include <Windows.h>
#include "cAIEngine.h"
/*
DWORD WINAPI DalekAIRoutine( LPVOID lpParam)
{
	cDalekAI * dalekAI =
		reinterpret_cast<cDalekAI *>(lpParam);

	Sleep(5000); // Give program some time to start

	do {

		std::chrono::duration<float> deltaTime =
			std::chrono::duration_cast<std::chrono::duration<float>>(
				std::chrono::high_resolution_clock::now() -
				dalekAI->lastTime); // Get the time that has passed

		dalekAI->pState->setCooldown(dalekAI->pState->getCooldown() - deltaTime.count());
		
		if (dalekAI->pState->getCooldown() > 0.0f)
		{
			dalekAI->pState->setTransform(glm::translate(dalekAI->pState->getTransform(), glm::vec3(0.0f, 0.0f, 1.0f)));
		}
		else
		{
			// Pick new target
			dalekAI->pState->setCooldown((rand() % 6) + 1);

			dalekAI->pState->setTarget(glm::vec3());
		}
		dalekAI->lastTime = std::chrono::high_resolution_clock::now();

		Sleep(60 ); // Someone else's turn now.. 

	} while (true);


	return 0;
}*/

/*void cDalekAI::startAIRoutine()
{

	this->lastTime =
		std::chrono::high_resolution_clock::now();

	DWORD  myThreadID;
	HANDLE myHandle = CreateThread(NULL, 0, // stack size
		DalekAIRoutine,
		reinterpret_cast<void*>(this), 0, &myThreadID);

	
}*/

void cDalekAI::update(float deltaTime)
{
	this->pState->setCooldown(this->pState->getCooldown() - deltaTime);

	if (this->pState->getCooldown() > 0.0f)
	{
		this->pState->setTransform(glm::translate(this->pState->getTransform(), glm::vec3(0.0f, 0.0f, 1.0f)));
	}
	else
	{
		// Pick new target
		this->pState->setCooldown((rand() % 6) + 1);

		this->pState->setTarget(glm::vec3());
	}
	this->lastTime = std::chrono::high_resolution_clock::now();
}
