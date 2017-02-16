#pragma once
#include "stdafx.h"
#include "iCollisionShape.h"
#include "_btRigidBody.h"
namespace PhysicsEngine {
	class _btCollisionShape : public iCollisionShape {
		friend class _btRigidBody;
		btCollisionShape* m_collisionShape = 0;
	public:
		_btCollisionShape(eShapeType shapeType);
		~_btCollisionShape() {
			delete m_collisionShape;
		}
		// Inherited via iCollisionShape
		virtual void getBoundingSphere(glm::vec3 &position, float& radius) const;
		virtual void setRadius(float radius) override;
		virtual float getRadius() override;
		virtual const char * getName() const override;
		virtual void setNormal(glm::vec3 normal) override;

	};
}