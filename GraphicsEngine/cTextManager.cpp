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

//Create a font of height h from the file path.
void cTextManager::font_data::loadFont(const char * path, unsigned int h)
{
	//Create and initilize a freetype font library.
	FT_Library library;
	if (FT_Init_FreeType(&library))
		throw std::runtime_error("FT_Init_FreeType failed");
}
