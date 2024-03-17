#pragma once

#include <vulkan/vulkan.hpp>

#include <BulletCollision/BroadphaseCollision/btBroadphaseInterface.h>
#include <BulletCollision/CollisionDispatch/btDefaultCollisionConfiguration.h>
#include <BulletCollision/CollisionDispatch/btCollisionDispatcher.h>
#include <btBulletDynamicsCommon.h>
#include <flecs/flecs.h>

#include "spdlog/spdlog.h"
#include <glm/glm.hpp>

#include <graphics/cameras/flying_camera.hpp>

#include <servicelocators/servicelocator.hpp>

using namespace Entropy::ServiceLocators;
using namespace Entropy::Services;

namespace Entropy
{
    namespace Physics
    {

        class BulletDebugDrawer : public btIDebugDraw
        {
        public:
            BulletDebugDrawer(flecs::world *world, std::shared_ptr<ServiceLocator> serviceLocator)
            {
                _world = world;
                _serviceLocator = serviceLocator;
            }

            void drawLine(const btVector3 &from, const btVector3 &to, const btVector3 &color) override
            {
                auto gizmo = _world->entity();

                glm::vec3 f = glm::vec3((float)from.x(), (float)from.y(), (float)from.z());
                glm::vec3 t = glm::vec3((float)to.x(), (float)to.y(), (float)to.z());
                glm::vec4 c = glm::vec4(color.x(), color.y(), color.z(), color.w());

                if (c.a == 0.0)
                {
                    c = glm::vec4(1.0, 1.0, 1.0, 1.0);
                }
                gizmo.set<Entropy::Components::LineGizmo>({std::make_shared<Line>(_serviceLocator, f, t, c)});
                gizmo.set<Entropy::Components::Renderable>({id++, 1, true});
                gizmo.set<Entropy::Components::Color>({c});
            }

            void drawContactPoint(const btVector3 &PointOnB, const btVector3 &normalOnB, btScalar distance, int lifeTime, const btVector3 &color) override
            {
            }

            void drawSphere(const btVector3 &p, btScalar radius, const btVector3 &color) override
            {
            }

            void drawTriangle(const btVector3 &v0, const btVector3 &v1, const btVector3 &v2, const btVector3 &color, btScalar /*alpha*/) override
            {
            }

            void reportErrorWarning(const char *warningString) override
            {
            }

            void draw3dText(const btVector3 &location, const char *textString) override
            {
            }

            void setDebugMode(int debugMode) override
            {
            }

            int getDebugMode() const override
            {
                return 1;
            }

        private:
            flecs::world *_world;
            std::shared_ptr<ServiceLocator> _serviceLocator;
            uint32_t id = 3;
        };

        class Physics3D : public Service
        {
        public:
            Physics3D(flecs::world *world, std::shared_ptr<ServiceLocator> serviceLocator)
            {
                _serviceLocator = serviceLocator;
                // Build the broadphase
                _broadphase = new btDbvtBroadphase();

                // Set up the collision configuration and dispatcher
                _collisionConfiguration = new btDefaultCollisionConfiguration();
                _dispatcher = new btCollisionDispatcher(_collisionConfiguration);

                // The actual physics solver
                _solver = new btSequentialImpulseConstraintSolver;

                // The world.
                _dynamicsWorld = new btDiscreteDynamicsWorld(_dispatcher, _broadphase, _solver, _collisionConfiguration);
                _dynamicsWorld->setGravity(btVector3(0, -9.81f, 0));

                BulletDebugDrawer *dbgDrawer = new BulletDebugDrawer(world, serviceLocator);

                _dynamicsWorld->setDebugDrawer(dbgDrawer);

                _dynamicsWorld->getDebugDrawer()->setDebugMode(1);
            }

            btDiscreteDynamicsWorld *GetWorld() { return _dynamicsWorld; };

            // Function to perform mouse picking
            void pickObject(int width, int height, int mouseX, int mouseY, const glm::mat4 &projectionMatrix, const glm::mat4 &viewMatrix)
            {
                auto camera = _serviceLocator->GetService<Cam>();

                // Convert screen coordinates to a 3D ray
                glm::vec3 rayTo = getRayTo(width, height, mouseX, mouseY, projectionMatrix, viewMatrix);

                glm::vec3 rayFrom = camera->Position;

                // Define the endpoint of the ray using a scaling factor
                float rayLength = 1000.0f; // Adjust this value as needed
                glm::vec3 rayEndpoint = rayFrom + rayTo * rayLength;

                // Perform ray casting against the physics world
                btCollisionWorld::ClosestRayResultCallback rayCallback(btVector3(rayFrom.x, rayFrom.y, rayFrom.z), btVector3(rayEndpoint.x, rayEndpoint.y, rayEndpoint.z));
                _dynamicsWorld->rayTest(btVector3(rayFrom.x, rayFrom.y, rayFrom.z), btVector3(rayEndpoint.x, rayEndpoint.y, rayEndpoint.z), rayCallback);

                //_dynamicsWorld->debugDrawWorld();
                //_dynamicsWorld->getDebugDrawer()->drawLine(btVector3{rayFrom.x, rayFrom.y, rayFrom.z}, btVector3{rayEndpoint.x, rayEndpoint.y, rayEndpoint.z}, btVector4(0, 1, 0, 1));

                // Check if the ray hit anything
                if (rayCallback.hasHit())
                {

                    auto point = btVector3(0.0f, 0.0f, 0.0f);

                    // Get the hit object
                    btRigidBody *hitObject = (btRigidBody *)btRigidBody::upcast(rayCallback.m_collisionObject);
                    if (hitObject)
                    {
                        glm::vec3 newPos = glm::vec3(point.x(), point.y(), point.z());
                        _lastPlanePoint = newPos;

                        auto entity = (flecs::entity *)hitObject->getUserPointer();
                        if (entity != nullptr)
                        {
                            _lastSelectedEntity = entity;
                        }
                        else
                        {
                            point = rayCallback.m_hitPointWorld;
                            _lastSelectedEntity = nullptr;
                        }
                    }
                }
            }

            glm::vec3 GetPoint() { return _lastPlanePoint; };
            flecs::entity *GetLastSelectedEntity() { return _lastSelectedEntity; };

        private:
            glm::vec3 getRayTo(int width, int height, int mouseX, int mouseY, const glm::mat4 &projectionMatrix, const glm::mat4 &viewMatrix)
            {
                // Adjust y-coordinate to match Vulkan's coordinate system

                // Convert mouse coordinates to clip space
                glm::vec4 clipCoords(
                    (mouseX / (float)width) * 2.0f - 1.0f,
                    (mouseY / (float)height) * 2.0f - 1.0f,
                    -1.0f, // Clip space z-coordinate should be in range [-1, 1]
                    1.0f);

                // Convert clip space to eye space
                glm::vec4 eyeCoords = glm::inverse(projectionMatrix) * clipCoords;
                eyeCoords.z = -1.0f;
                eyeCoords.w = 0.0f;

                // Convert eye space to world space
                glm::vec4 worldCoords = glm::inverse(viewMatrix) * eyeCoords;

                // Normalize the direction vector
                glm::vec3 worldRay = glm::normalize(glm::vec3(worldCoords));

                return worldRay;
            }

            std::shared_ptr<ServiceLocator> _serviceLocator;
            btBroadphaseInterface *_broadphase;
            btDefaultCollisionConfiguration *_collisionConfiguration;
            btCollisionDispatcher *_dispatcher;
            btSequentialImpulseConstraintSolver *_solver;
            btDiscreteDynamicsWorld *_dynamicsWorld;

            glm::vec3 _lastPlanePoint;
            flecs::entity *_lastSelectedEntity = nullptr;
        };
    }
}
