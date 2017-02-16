#ifndef _cTextureManager_HG_
#define _cTextureManager_HG_
#include "stdafx.h"
// Loads textures into OpenGL texture units
// Maps texture with textureID look up

class cTextureManager {
	static cTextureManager *s_cTextureManager;

public:
	static cTextureManager *instance();
	void loadTexture(rapidxml::xml_node<> *textureNode);
	void loadTextureMipmap(rapidxml::xml_node<> *textureNode);
	void loadWorldTilesFromImage(rapidxml::xml_node<> *worldNode);
	GLuint loadCubeMap(rapidxml::xml_node<> *cubeNode);
	std::vector<GLuint> uniform_TextureID;
	std::map<std::string, GLuint> mapTextureNameToID;
	std::map<std::string, GLuint> mapTextureNameToMipmapLevel;
	std::vector<std::vector <std::vector<glm::vec3>>> worldTiles;
	GLuint nextTextureID;
	int currentMipmapLevel = 0;
private:
	cTextureManager() {
	} // Constructor is private therefore a new instance can not be made
	  // externally. Only available to members or friends of this class..
	~cTextureManager() {}
	// Not defined to prevent copying of the only instance of the class.
	cTextureManager(const cTextureManager &); // Disallow copy constructor
	cTextureManager &operator=(const cTextureManager &textureManager) {
	} // Disallow assignment operator
};

#endif