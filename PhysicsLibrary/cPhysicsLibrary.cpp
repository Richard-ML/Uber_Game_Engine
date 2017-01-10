#include "cPhysicsLibrary.h"

// cGameObject Component Based Design Prototype
//class cGameObject {
//public:
//	// Standard Components
//	std::vector<std::shared_ptr<cComponent>> vec_m_components;
//	cGameObject() {}
//	~cGameObject() {
//		// Delete all components
//	}
//};
// The PIMPL idiom aka Compilation Firewall
// Purpose: Encapsulate private member variables. Reduces make-time,
// compile-time, and the Fragile Binary Interface Problem.
namespace PhysicsLibrary {
	class cPhysicsLibrary_Impl : public cPhysicsLibrary {
		// Boilerplate
		friend class cPhysicsLibrary;

	public:
		//static std::vector<cGameObject *> vec_Entities;
		//static rapidxml::xml_node<> *spiderNode;
	};
	inline const cPhysicsLibrary_Impl *cPhysicsLibrary::impl() const {
		return static_cast<const cPhysicsLibrary_Impl *>(this);
	}
	inline cPhysicsLibrary_Impl *cPhysicsLibrary::impl() {
		return static_cast<cPhysicsLibrary_Impl *>(this);
	}
	// Initialize vectors
	//std::vector<cGameObject *> cEntityManager_Impl::vec_Entities;
	//rapidxml::xml_node<> *cEntityManager_Impl::spiderNode;
	cPhysicsLibrary * cPhysicsLibrary::instance() {
		if (!s_cPhysicsLibrary)
			s_cPhysicsLibrary = new cPhysicsLibrary();
		return s_cPhysicsLibrary;
	}
}