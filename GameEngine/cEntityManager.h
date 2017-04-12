#ifndef _cEntityManager_HG_
#define _cEntityManager_HG_
#include "stdafx.h"
class cEntityManager_Impl; // Purpose: Encapsulate private member variables and
						   // internal routines. Reduces make-time,
						   // compile-time, and the Fragile Binary Interface
						   // Problem.

class cEntityManager {
	static cEntityManager *s_cEntityManager;
	// Boilerplate
	friend class cEntityManager_Impl; // The PIMPL idiom aka Compilation Firewall
	const cEntityManager_Impl *impl() const;
	cEntityManager_Impl *impl();

public:

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Loads game from XML. </summary>
	///
	/// <remarks>	Richard, 4/12/2017. </remarks>
	///
	/// <param name="filename">	Filename of the file. </param>
	///
	/// <returns>	The game from XML. </returns>
	///-------------------------------------------------------------------------------------------------

	int loadGameFromXML(std::string filename);

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Gets the instance. </summary>
	///
	/// <remarks>	Richard, 4/12/2017. </remarks>
	///
	/// <returns>	Null if it fails, else a pointer to a cEntityManager. </returns>
	///-------------------------------------------------------------------------------------------------

	static cEntityManager *instance();

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Spawn objects at selected tile. </summary>
	///
	/// <remarks>	Richard, 4/12/2017. </remarks>
	///-------------------------------------------------------------------------------------------------

	void spawnObjectsAtSelectedTile();

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Saves a game to XML. </summary>
	///
	/// <remarks>	Richard, 4/12/2017. </remarks>
	///
	/// <param name="difficulty">	The difficulty. </param>
	///-------------------------------------------------------------------------------------------------

	void saveGameToXML(int difficulty);

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Loads game entities from XML. </summary>
	///
	/// <remarks>	Richard, 4/12/2017. </remarks>
	///
	/// <param name="difficulty">	The difficulty. </param>
	///-------------------------------------------------------------------------------------------------

	void loadGameEntitiesFromXML(int difficulty);

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Cleans up resources. </summary>
	///
	/// <remarks>	Richard, 4/12/2017. </remarks>
	///-------------------------------------------------------------------------------------------------

	void cleanup();
private:
	cEntityManager() {
	} // Constructor is private therefore a new instance can not be made
	  // externally. Only available to members or friends of this class..
	~cEntityManager() {}
	// Not defined to prevent copying of the only instance of the class.
	cEntityManager(const cEntityManager &); // Disallow copy constructor
	cEntityManager &operator=(const cEntityManager &entityManager) {
	} // Disallow assignment operator
};
#endif