#include "stdafx.h"
#include "cSoftBodyManager.h"
#include "global.h"


// The PIMPL idiom aka Compilation Firewall
// Purpose: Encapsulate private member variables. Reduces make-time,
// compile-time, and the Fragile Binary Interface Problem.
class cSoftBodyManager_Impl : public cSoftBodyManager {
	// Boilerplate
	friend class cSoftBodyManager;

public:
};
inline const cSoftBodyManager_Impl *cSoftBodyManager::impl() const {
	return static_cast<const cSoftBodyManager_Impl *>(this);
}
inline cSoftBodyManager_Impl *cSoftBodyManager::impl() {
	return static_cast<cSoftBodyManager_Impl *>(this);
}


cSoftBodyManager *cSoftBodyManager::instance() {
	if (!s_cSoftBodyManager)
		s_cSoftBodyManager = new cSoftBodyManager();
	return s_cSoftBodyManager;
}