#ifndef _cTextureManager_HG_
#define _cTextureManager_HG_
#include "stdafx.h"
// Loads textures into OpenGL texture units
// Maps texture with textureID look up

class cTextureManager {
	// Manager for the textures.
	static cTextureManager *s_cTextureManager;

public:

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Gets the instance. </summary>
	///
	/// <remarks>	Richard, 4/12/2017. </remarks>
	///
	/// <returns>	Null if it fails, else a pointer to a cTextureManager. </returns>
	///-------------------------------------------------------------------------------------------------

	static cTextureManager *instance();

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Loads a texture. </summary>
	///
	/// <remarks>	Richard, 4/12/2017. </remarks>
	///
	/// <param name="textureNode">	[in,out] If non-null, the texture node. </param>
	///-------------------------------------------------------------------------------------------------

	void loadTexture(rapidxml::xml_node<> *textureNode);

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Loads texture mipmap. </summary>
	///
	/// <remarks>	Richard, 4/12/2017. </remarks>
	///
	/// <param name="textureNode">	[in,out] If non-null, the texture node. </param>
	///-------------------------------------------------------------------------------------------------

	void loadTextureMipmap(rapidxml::xml_node<> *textureNode);

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Loads world tiles from image. </summary>
	///
	/// <remarks>	Richard, 4/12/2017. </remarks>
	///
	/// <param name="worldNode">	[in,out] If non-null, the world node. </param>
	///-------------------------------------------------------------------------------------------------

	void loadWorldTilesFromImage(rapidxml::xml_node<> *worldNode);

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Loads cube map. </summary>
	///
	/// <remarks>	Richard, 4/12/2017. </remarks>
	///
	/// <param name="cubeNode">	[in,out] If non-null, the cube node. </param>
	///
	/// <returns>	The cube map. </returns>
	///-------------------------------------------------------------------------------------------------

	GLuint loadCubeMap(rapidxml::xml_node<> *cubeNode);
	// Identifier for the uniform texture.
	std::vector<GLuint> uniform_TextureID;
	// Texture name to uniform identifier.
	std::map<std::string, GLuint> mapTextureNameToID;
	std::map<std::string, glm::vec4> mapTextureNameToMipmapLevel;
	std::vector<std::vector <std::vector<glm::ivec3>>> worldTiles;
	// Identifier for the next texture.
	GLuint nextTextureID;
	int currentMipmapRow = 0;
	int currentMipmapCol = 0;
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