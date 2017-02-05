#pragma once
#include "stdafx.h"
#include "global.h"
namespace ComponentManager {
	class iComponent {
		//virtual cComponent* getType() = 0;
		virtual void update() = 0;
		virtual bool loadFromXML(rapidxml::xml_node<> *componentNode) = 0;
		virtual void command(std::string method,
			std::string value) = 0; // Command pattern.
									// Enables components to communicate with other components
									// without knowing anything about their abstract members
	};
}