#include "stdafx.h"
#include "cWorld.h"

// Intersect sphere s with movement vector v with plane p. If intersecting
// return time t of collision and point q at which sphere hits plane
bool IntersectMovingSpherePlane(PhysicsEngine::iRigidBody* s, glm::vec3 v, PhysicsEngine::iRigidBody* p, float &t, glm::vec3 &q)
{
	PhysicsEngine::cSphere& sphereShape = *dynamic_cast<PhysicsEngine::cSphere*>(s->getCollisionShape());
	PhysicsEngine::cPlane& planeShape = *dynamic_cast<PhysicsEngine::cPlane*>(p->getCollisionShape());

	glm::vec3 sphereCentre;
	glm::vec3 planePosition;
	float radius = sphereShape.getRadius() * 2.0f;
	s->getPosition(sphereCentre);
	p->getPosition(planePosition);
	
	// Compute distance of sphere center to plane
	float dist = glm::dot(sphereCentre - planePosition, glm::round(planeShape.getNormal()));


	if (std::abs(dist) <= radius) {
		// The sphere is already overlapping the plane. Set time of
		// intersection to zero and q to sphere center
		t = 0.0f;
		q = sphereCentre;
		return 1;
	}
	else {
		float denom = glm::dot(glm::round(planeShape.getNormal()), v);
		if (denom * dist >= 0.0f) {
			// No intersection as sphere moving parallel to or away from plane
			return 0;
		}
		else {
			// Sphere is moving towards the plane

			// Use +r in computations if sphere in front of plane, else -r
			float r = dist > 0.0f ? radius : -radius;
			t = (r - dist) / denom;
			q = sphereCentre + t * v - r * glm::round(planeShape.getNormal());
			return 1;
		}
	}
}
int IntersectMovingSphereSphere(PhysicsEngine::iRigidBody* sphere1, PhysicsEngine::iRigidBody* sphere2, const glm::vec3& vA, const glm::vec3& vB, float &t)
{
	PhysicsEngine::cSphere& sphereShape1 = *dynamic_cast<PhysicsEngine::cSphere*>(sphere1->getCollisionShape());
	PhysicsEngine::cSphere& sphereShape2 = *dynamic_cast<PhysicsEngine::cSphere*>(sphere2->getCollisionShape());

	glm::vec3 sphere1Position;
	glm::vec3 sphere2Position;

	sphere1->getPosition(sphere1Position);
	sphere2->getPosition(sphere2Position);


	glm::vec3 s = sphere1Position - sphere2Position; // Vector between sphere centers
	glm::vec3 v = vB - vA;      
	float r = sphereShape1.getRadius() + sphereShape2.getRadius();
	float c = glm::dot(s, s) - r * r;
	if (c < 0.0f)
	{
		// Spheres initially overlapping so exit directly
		t = 0.0f;
		return 1;
	}

	float a = glm::dot(v, v);

	if (a < FLT_EPSILON)
		return 0; // Spheres not moving relative each other

	float b = glm::dot(v, s);
	if (b >= 0.0f)
		return 0;   // Spheres not moving towards each other
	float d = b * b - a * c;
	if (d < 0.0f)
		return 0;    // No real-valued root, spheres do not intersect
	t = (-b - sqrt(d)) / a;
	return -1;

}




struct sDerivative
{
	glm::vec3 velocity;
	glm::vec3 acceleration;
};



sDerivative evaluate(PhysicsEngine::iRigidBody *initial,
	float t,
	float dt,
	const sDerivative &d)
{
	glm::vec3 position;
	glm::vec3 velocity;
	glm::vec3 acceleration;

	initial->getPosition(position);
	initial->getVelocity(velocity);
	initial->getAcceleration(acceleration);

	position += d.velocity * dt;
	velocity += d.acceleration * dt;

	sDerivative output;
	output.velocity = velocity;
	
	//output.acceleration = glm::vec3(0.0f, -10.0f * (position.y - velocity.y), 0.0f);
	
	//output.acceleration = glm::vec3(-10.f) *  position - velocity; // O.O
	return output;
}

void PhysicsEngine::cWorld::step(float deltaTime)
{
	//printf("STEP! ");
	for (std::vector<iRigidBody*>::iterator iter = m_objects.begin(); iter != m_objects.end(); iter++)
	{
		if ((*iter)->getCollisionShape()->getShapeType() == eShapeType::SPHERE)
		{
			// TODO: Resolve potential collisions 
			glm::vec3 position;
			glm::vec3 velocity;
			glm::vec3 acceleration;
			float mass;
		
			(*iter)->getPosition(position);
			(*iter)->getVelocity(velocity);
			(*iter)->getMass(mass);
			(*iter)->getAcceleration(acceleration);
			glm::vec3 previousPos = position;
			velocity.y += (-9.8f*mass) * deltaTime;

			sDerivative a, b, c, d;
			
			a = evaluate(*iter, 0.0f, 0.0f, sDerivative());
			b = evaluate(*iter, 0.0f, deltaTime*0.5f, a);
			c = evaluate(*iter, 0.0f, deltaTime*0.5f, b);
			d = evaluate(*iter, 0.0f, deltaTime, c);

			glm::vec3 dxdt = 1.0f / 6.0f *
				(a.velocity + (2.0f*(b.velocity + c.velocity) + d.velocity));

			glm::vec3 dvdt = 1.0f / 6.0f *
				(a.acceleration + 2.0f*(b.acceleration + c.acceleration) + d.acceleration);

			position += dxdt * deltaTime;
			velocity += dvdt * deltaTime;

			
			bool collision = false;
			for (std::vector<iRigidBody*>::iterator iter2 = m_objects.begin(); iter2 != m_objects.end(); iter2++)
			{
				if (iter != iter2) // Not the same object.. 
				{
					eShapeType objType = (*iter2)->getCollisionShape()->getShapeType();
					
					switch (objType)
					{
					case PhysicsEngine::SPHERE:
					{
						glm::vec3 vel2;
						(*iter2)->getVelocity(vel2);
						float timeOfCollision;

						if (IntersectMovingSphereSphere((*iter), (*iter2), velocity, vel2, timeOfCollision))
						{
							
							PhysicsEngine::cSphere& sphereShape1 = *dynamic_cast<PhysicsEngine::cSphere*>((*iter)->getCollisionShape());
							PhysicsEngine::cSphere& sphereShape2 = *dynamic_cast<PhysicsEngine::cSphere*>((*iter2)->getCollisionShape());
							collision = true;
							(*iter)->setCollision(collision);
							//position -= dxdt *  ((deltaTime)-(timeOfCollision) );
							

							float sphere1Mass, sphere2Mass;
							(*iter)->getMass(sphere1Mass);
							(*iter2)->getMass(sphere2Mass);
							float sumMass = sphere2Mass + sphere1Mass;
							glm::vec3 pos1, pos2;
							(*iter)->getPosition(pos1);
							(*iter2)->getPosition(pos2);

							//velocity *= 0.9f;

							while (true) {
								pos1 = pos1 - ((pos2 - pos1) / (sphere1Mass + 0.1f));
								pos2 = pos2 - ((pos1 - pos2) / (sumMass + 0.1f));
								if (glm::distance(pos2, pos1) > sumMass)
									break;
							}

							//(*iter)->pRigidBody->setPosition(pos1);
							(*iter2)->setPosition(pos2);

							glm::vec3 resultVel1, resultVel2;
							resultVel1 = (0.1f *sphere2Mass*(vel2 - velocity) + sphere1Mass*velocity + sphere2Mass*vel2) / sumMass;
							resultVel2 = (0.1f *sphere1Mass*(velocity - vel2) + sphere1Mass*velocity + sphere2Mass*vel2) / sumMass;
		
							velocity = glm::clamp(resultVel1, glm::vec3(-30), glm::vec3(30 * mass));
							(*iter2)->setVelocity(resultVel2);
							(*iter)->setVelocity(resultVel1);
						}
					}
						break;
					case PhysicsEngine::PLANE:
					{
						glm::vec3 collisionPoint;

						float timeOfCollision;
						if (IntersectMovingSpherePlane((*iter), velocity, (*iter2), timeOfCollision, collisionPoint))
						{
							if (timeOfCollision/2 < deltaTime)
							{
								//velocity = velocity * glm::vec3(0.98f);
								collision = true;
								(*iter)->setCollision(collision);
								//position += dxdt * (-timeOfCollision);
								glm::vec3 diff = collisionPoint - position;
								PhysicsEngine::cPlane& planeShape = *dynamic_cast<PhysicsEngine::cPlane*>((*iter2)->getCollisionShape());

								PhysicsEngine::cSphere& sphereShape = *dynamic_cast<PhysicsEngine::cSphere*>((*iter)->getCollisionShape());

								
								glm::vec3 reflection = glm::reflect(velocity, planeShape.getNormal());
								glm::vec3 norm = glm::normalize(collisionPoint- position);
								reflection += 0.2f * (velocity*(-planeShape.getNormal()));

								while (true)
								{
									if (glm::abs(glm::distance(collisionPoint, position)) <= (sphereShape.getRadius() /2) )
										position += norm;
									else
										break;
								}
								velocity = glm::clamp(velocity, glm::vec3(-30 * mass), glm::vec3(30 * mass));
								position = glm::clamp(position, glm::vec3(-1000.0f, -1.0f, -1000.0f), glm::vec3(1000.0f, 1000.0f, 1000.0f));
								(*iter)->setVelocity(reflection);
								(*iter)->setPosition(position);
							}
						}
					}
						break;
					
					}
				}
			}
			velocity = glm::clamp(velocity, glm::vec3(-30 * mass), glm::vec3(30 * mass));
			position = glm::clamp(position, glm::vec3(-1000.0f, 1.0f, -1000.0f), glm::vec3(1000.0f, 1000.0f, 1000.0f));
			if (collision == false) {
				(*iter)->setCollision(collision);
				(*iter)->setPosition(position);
				(*iter)->setVelocity(velocity);
				(*iter)->setAcceleration(acceleration);
			}
		

		}
	}
}

void PhysicsEngine::cWorld::addCollisionObject(iRigidBody* object)
{
	if (std::find(m_objects.begin(), m_objects.end(), object) == m_objects.end())
		m_objects.push_back(object); // If object is not already in the world. Add it to the world! :)
}

//void PhysicsEngine::cWorld::addRigidBody(cRigidBody * rigidBody)
//{
//	//if (!rigidBody) return; // If nullptr return...
//	if (std::find(m_rigidBodies.begin(), m_rigidBodies.end(), rigidBody) == m_rigidBodies.end())
//		m_rigidBodies.push_back(rigidBody);
//	
//}
namespace PhysicsEngine{
//std::vector<cCollisionObject*> cWorld::m_objects;
}