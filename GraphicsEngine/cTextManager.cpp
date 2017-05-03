#include "stdafx.h"
#include "cTextManager.h"



// The PIMPL idiom aka Compilation Firewall
// Purpose: Encapsulate private member variables. Reduces make-time,
// compile-time, and the Fragile Binary Interface Problem.
class cTextManager_Impl : public cTextManager {
	// Boilerplate

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Manager for the Texts. </summary>
	///
	/// <remarks>	Richard, 4/12/2017. </remarks>
	///-------------------------------------------------------------------------------------------------

	friend class cTextManager;

public:
	//	static std::map<std::tuple<float, float, float>, aiText>
	//m_map_RGBToText;
};

///-------------------------------------------------------------------------------------------------
/// <summary>	Gets the implementation. </summary>
///
/// <remarks>	Richard, 4/12/2017. </remarks>
///
/// <returns>	Null if it fails, else a pointer to a const cTextManager_Impl. </returns>
///-------------------------------------------------------------------------------------------------

inline const cTextManager_Impl *cTextManager::impl() const {
	return static_cast<const cTextManager_Impl *>(this);
}

///-------------------------------------------------------------------------------------------------
/// <summary>	Gets the implementation. </summary>
///
/// <remarks>	Richard, 4/12/2017. </remarks>
///
/// <returns>	Null if it fails, else a pointer to a cTextManager_Impl. </returns>
///-------------------------------------------------------------------------------------------------

inline cTextManager_Impl *cTextManager::impl() {
	return static_cast<cTextManager_Impl *>(this);
}

///-------------------------------------------------------------------------------------------------
/// <summary>	Gets the instance. </summary>
///
/// <remarks>	Richard, 4/12/2017. </remarks>
///
/// <returns>	Null if it fails, else a pointer to a cTextManager. </returns>
///-------------------------------------------------------------------------------------------------

cTextManager *cTextManager::instance() {
	if (!s_cTextManager)
		s_cTextManager = new cTextManager();
	return s_cTextManager;
}

///-------------------------------------------------------------------------------------------------
/// <summary>	Loads a font. Create a font of height h from the file path. </summary>
///
/// <remarks>
/// Richard, 4/15/2017. 
/// Based on a quick and simple opengl font library that uses GNU freetype2,
/// written and distributed as part of a tutorial for nehe.gamedev.net. 
/// Sven Olsen, 2003.
/// http://nehe.gamedev.net/tutorial/freetype_fonts_in_opengl/24001/.
/// </remarks>
///
/// <exception cref="std::runtime_error">
/// Raised when a runtime error condition occurs.
/// </exception>
///
/// <param name="fontName">	Name of the font. </param>
/// <param name="path">	   	Full pathname of the file. </param>
/// <param name="height">  	The height. </param>
///-------------------------------------------------------------------------------------------------

void cTextManager::printString(std::string fontName, float x, float y, std::string text)
{
	
}

void cTextManager::loadFont(std::string fontName, const char * path, unsigned int height)
{
	//Allocate some memory to store the texture ids of characters.
	font.textures = new GLuint[128];
	font.height = height;
	// Create and initialize a freetype font library.
	FT_Library ft_library;
	if (FT_Init_FreeType(&ft_library))
		throw std::runtime_error("FT_Init_FreeType failed");

	// The face.
	FT_Face ft_face;

	if (FT_New_Face(ft_library, path, 0, &ft_face))
		throw std::runtime_error("FT_New_Face failed: There is likely a problem with the font file");
	
	FT_Set_Char_Size(ft_face, height << 6, height << 6, 96, 96);

	font.list_base = glGenLists(128);
	glGenTextures(128, font.textures);

	font.createDisplayList(ft_face);

	//We don't need the face information now that the display
	//lists have been created, so we free the associated resources.
	FT_Done_Face(ft_face);
	FT_Done_FreeType(ft_library);
	return;
}

void cTextManager::printText(const sFontData & ft_font, float x, float y, const char * fmt, ...)
{

}

///-------------------------------------------------------------------------------------------------
/// <summary>	This function gets the first power of 2 >= the int that it receives. </summary>
///
/// <remarks>
/// Richard, 4/15/2017.
/// Based on a quick and simple opengl font library that uses GNU freetype2,
/// written and distributed as part of a tutorial for nehe.gamedev.net.
/// Sven Olsen, 2003.
/// http://nehe.gamedev.net/tutorial/freetype_fonts_in_opengl/24001/
/// </remarks>
///
/// <param name="value">	An int to process. </param>
///
/// <returns>	An int of 2nd power. </returns>
///-------------------------------------------------------------------------------------------------

inline int next_p2(int value)
{
	int rval = 1;
	while (rval< value) rval <<= 1;
	return rval;
}

///-------------------------------------------------------------------------------------------------
/// <summary>	Creates display list. </summary>
///
/// <remarks>
/// Richard, 4/15/2017. 
/// Based on a quick and simple opengl font library that uses GNU freetype2,
/// written and distributed as part of a tutorial for nehe.gamedev.net. Sven Olsen, 2003.
/// http://nehe.gamedev.net/tutorial/freetype_fonts_in_opengl/24001
/// </remarks>
///
/// <exception cref="std::runtime_error">
/// Raised when a runtime error condition occurs.
/// </exception>
///
/// <param name="face">	The face. </param>
///-------------------------------------------------------------------------------------------------

void cTextManager::sFontData::createDisplayList(FT_Face face)
{
	// Create each of the fonts display lists.
	for (unsigned char nc = 0; nc < 128; nc++)
	{
		//Load the Glyph of current character.
		if (FT_Load_Glyph(face, FT_Get_Char_Index(face, nc), FT_LOAD_DEFAULT))
			throw std::runtime_error("FT_Load_Glyph failed");

		//Move the face's glyph into a Glyph object...
		FT_Glyph ft_glyph;
		if (FT_Get_Glyph(face->glyph, &ft_glyph))
			throw std::runtime_error("FT_Get_Glyph failed");

		//Convert the glyph to a bitmap.
		FT_Glyph_To_Bitmap(&ft_glyph, ft_render_mode_normal, 0, 1);
		FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph)ft_glyph;

		//This reference will make accessing the bitmap easier
		FT_Bitmap& bitmap = bitmap_glyph->bitmap;

		int width = next_p2(bitmap.width);
		int height = next_p2(bitmap.rows);

		//Allocate memory for the texture data.
		GLubyte* expanded_data = new GLubyte[2 * width * height];

		for (int j = 0; j <height; j++) {
			for (int i = 0; i < width; i++) {
				expanded_data[2 * (i + j*width)] = expanded_data[2 * (i + j*width) + 1] =
					(i >= bitmap.width || j >= bitmap.rows) ?
					0 : bitmap.buffer[i + bitmap.width*j];
			}
		}

		glBindTexture(GL_TEXTURE_2D, this->textures[nc]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		//Here we actually create the texture itself, notice
		//that we are using GL_LUMINANCE_ALPHA to indicate that
		//we are using 2 channel data.
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height,
			0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, expanded_data);

		delete[] expanded_data;

		glNewList(list_base + nc, GL_COMPILE);

		glBindTexture(GL_TEXTURE_2D, this->textures[nc]);

		glPushMatrix();


		glTranslatef(bitmap_glyph->left, 0, 0);
		glTranslatef(0, bitmap_glyph->top - bitmap.rows, 0);

		float	x = (float)bitmap.width / (float)width,
			y = (float)bitmap.rows / (float)height;

		glBegin(GL_QUADS);
		glTexCoord2d(0, 0); glVertex2f(0, bitmap.rows);
		glTexCoord2d(0, y); glVertex2f(0, 0);
		glTexCoord2d(x, y); glVertex2f(bitmap.width, 0);
		glTexCoord2d(x, 0); glVertex2f(bitmap.width, bitmap.rows);
		glEnd();
		glPopMatrix();
		glTranslatef(face->glyph->advance.x >> 6, 0, 0);


		//increment the raster position as if we were a bitmap font.
		//(only needed if you want to calculate text length)
		//glBitmap(0,0,0,0,face->glyph->advance.x >> 6,0,NULL);

		//Finish the display list
		glEndList();
	}
}
