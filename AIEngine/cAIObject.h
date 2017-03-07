#pragma once
#include "stdafx.h"
#include <Windows.h>
#include "cAIEngine.h"
#include <chrono>
#include <ctime>

	class cAIObject {
	public:
		virtual void startAIRoutine() = 0;

		// Creates an xml node containing all of it's information
		virtual  std::string saveToXMLNode() {
			std::string componentNode = "<AIComponent>";
			componentNode += "</AIComponent>";
			return componentNode;
		}
	};

	// TODO remove this it was for gems midterm 
	class cDalekAI{
		friend AIEngine::cAIEngine;
		//DWORD WINAPI DalekAIRoutine(void *lpParam);
	public:
		std::pair<int, int> currentTile;
		iState* pState;
		//void startAIRoutine();
		void update(float deltaTime);
		std::chrono::high_resolution_clock::time_point lastTime;

		cDalekAI() {
		};
	};

