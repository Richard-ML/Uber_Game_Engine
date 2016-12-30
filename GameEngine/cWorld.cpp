#include "cWorld.h"
#include "global.h"
void cWorld::loadFromXML(rapidxml::xml_node<> *areaNode) {
  g_pAreaInfo->minPos.x = std::stof(areaNode->first_attribute("minX")->value());
  g_pAreaInfo->minPos.y = std::stof(areaNode->first_attribute("minY")->value());
  g_pAreaInfo->minPos.z = std::stof(areaNode->first_attribute("minZ")->value());

  g_pAreaInfo->maxPos.x = std::stof(areaNode->first_attribute("maxX")->value());
  g_pAreaInfo->maxPos.y = std::stof(areaNode->first_attribute("maxY")->value());
  g_pAreaInfo->maxPos.z = std::stof(areaNode->first_attribute("maxZ")->value());
}
