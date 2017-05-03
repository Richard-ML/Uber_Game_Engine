#pragma once

///-------------------------------------------------------------------------------------------------
/// <summary>	Manager for Text. </summary>
///
/// <remarks>
/// Richard, 4/12/2017.
/// Based on tutorial http://nehe.gamedev.net/tutorial/freetype_fonts_in_opengl/24001/
/// </remarks>
///-------------------------------------------------------------------------------------------------

class cTextManager {
	static cTextManager *s_cTextManager;
	// Boilerplate
	friend class cTextManager_Impl; // The PIMPL idiom aka Compilation Firewall
	const cTextManager_Impl *impl() const;
	cTextManager_Impl *impl();

public:

	static cTextManager *instance();

	void updateTextUniforms();

	///-------------------------------------------------------------------------------------------------
	/// <summary>	FreeType Font Information </summary>
	///
	/// <remarks>	Richard, 4/15/2017. </remarks>
	///-------------------------------------------------------------------------------------------------

	struct sFontData {
		// The height of the font.
		float height;
		// The texture id's.
		GLuint * textures;
		// The first display list id.
		GLuint list_base;

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Creates display list. </summary>
		///
		/// <remarks>	Richard, 4/15/2017. </remarks>
		///
		/// <param name="face">	The face. </param>
		///-------------------------------------------------------------------------------------------------

		void createDisplayList(FT_Face face);
	} font; // Currently only one font..


	void printString(std::string fontName, float x, float y, std::string text);

	// Create a font of height h from the file path.
	void loadFont(std::string fontName, const char * path, unsigned int height);

	//Print out text at window coordinates x,y, using the font ft_font.
	//The current model view matrix will also be applied to the text. 
	void printText(const sFontData &ft_font, float x, float y, const char *fmt, ...); // 

private:
	cTextManager() {
	} // Constructor is private therefore a new instance can not be made
	  // externally. Only available to members or friends of this class..
	~cTextManager() {}
	// Not defined to prevent copying of the only instance of the class.
	cTextManager(const cTextManager &); // Disallow copy constructor
	cTextManager &operator=(const cTextManager &meshManager) {
	} // Disallow assignment operator
};

///-------------------------------------------------------------------------------------------------
// End of cTextManager.h
///-------------------------------------------------------------------------------------------------

