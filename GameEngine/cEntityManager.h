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
	int loadGameFromXML(std::string filename);
	static cEntityManager *instance();

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