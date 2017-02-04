#include "stdafx.h"
#include "cBlackBoxTests.h"
namespace BlackBoxTests {
	// BLACK BOX TESTS
	TEST(PHYSICSENGINE, PLANESHAPE_NAME)
	{
		PhysicsEngine::iCollisionShape* planeShape = PhysicsEngine::cPhysicsEngine::instance()->createCollisionShape(PhysicsEngine::eShapeType::PLANE);
		EXPECT_GT(planeShape->getName(), "Plane");
	}
	TEST(PHYSICSENGINE, PLANESHAPE_TYPE)
	{
		PhysicsEngine::iCollisionShape* planeShape = PhysicsEngine::cPhysicsEngine::instance()->createCollisionShape(PhysicsEngine::eShapeType::PLANE);
		EXPECT_GT(planeShape->getShapeType(), PhysicsEngine::eShapeType::PLANE);
	}
	TEST(PHYSICSENGINE, SPHERESHAPE_NAME)
	{
		PhysicsEngine::iCollisionShape* sphereShape = PhysicsEngine::cPhysicsEngine::instance()->createCollisionShape(PhysicsEngine::eShapeType::SPHERE);
		EXPECT_GT(sphereShape->getName(), "Sphere");
	}
	TEST(PHYSICSENGINE, SPHERESHAPE_TYPE)
	{
		PhysicsEngine::iCollisionShape* sphereShape = PhysicsEngine::cPhysicsEngine::instance()->createCollisionShape(PhysicsEngine::eShapeType::SPHERE);
		EXPECT_GT(sphereShape->getShapeType(), PhysicsEngine::eShapeType::SPHERE);
	}
	TEST(PHYSICSENGINE, SPHERESHAPE_RADIUS)
	{
		PhysicsEngine::iCollisionShape* sphereShape = PhysicsEngine::cPhysicsEngine::instance()->createCollisionShape(PhysicsEngine::eShapeType::SPHERE);
		EXPECT_GT(sphereShape->getRadius(), 0.0f); // Check to see if value is 0... 
	}


	
	void runTests()
	{

		// WHITE BOX TESTS
		PhysicsEngine::iCollisionShape* planeShape = PhysicsEngine::cPhysicsEngine::instance()->createCollisionShape(PhysicsEngine::eShapeType::PLANE);
		PhysicsEngine::iCollisionShape* sphereShape = PhysicsEngine::cPhysicsEngine::instance()->createCollisionShape(PhysicsEngine::eShapeType::SPHERE);
		PhysicsEngine::sRigidBody rigidBodyInfo;

		EXPECT_TRUE(sphereShape->getShapeType() == PhysicsEngine::eShapeType::SPHERE);
		EXPECT_TRUE(planeShape->getShapeType() == PhysicsEngine::eShapeType::PLANE);


		rigidBodyInfo.mass = 15.0f;
		EXPECT_TRUE(rigidBodyInfo.mass == 15.0f); // Internal use of struct!!! 

		rigidBodyInfo.collision = true;
		EXPECT_TRUE(rigidBodyInfo.collision == true); // Internal use of struct!!! 

		rigidBodyInfo.collisionFlag = PhysicsEngine::eCollisionFlags::STATIC_OBJECT;
		EXPECT_TRUE(rigidBodyInfo.collisionFlag == PhysicsEngine::eCollisionFlags::STATIC_OBJECT); // Internal use of struct!!! 

		rigidBodyInfo.mass = 2.0f;

		EXPECT_FALSE(rigidBodyInfo.mass == 15.0f);

		sphereShape->setRadius(1.0f);
		EXPECT_TRUE(rigidBodyInfo.mass == 15.0f);


		PhysicsEngine::iRigidBody* rigidBody = PhysicsEngine::cPhysicsEngine::instance()->createRigidBody(sphereShape, rigidBodyInfo);
		float mass;
		rigidBody->getMass(mass);
		EXPECT_TRUE(mass == 15.0f);

	}

}